#pragma once

// Portable acquisition logic. The sketch protects every call with a mutex;
// no Arduino or network code is needed to test the parser and capture window.
#include <cerrno>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

namespace signal_capture {

constexpr uint32_t kDurationUs = 5000000UL;
constexpr size_t kMaxSamples = 4096;
constexpr size_t kMaxChannels = 4;
constexpr size_t kMaxLine = 192;

enum class State : uint8_t { Idle, Arming, Recording, Done };

inline const char* stateName(State state) {
  switch (state) {
    case State::Arming: return "arming";
    case State::Recording: return "recording";
    case State::Done: return "done";
    default: return "idle";
  }
}

struct Sample {
  uint32_t timeUs;
  float values[kMaxChannels];
};

struct Status {
  uint32_t captureId;
  State state;
  uint32_t elapsedMs;
  size_t samples;
  uint8_t channels;
  uint32_t invalidLines;
  uint32_t droppedSamples;
  uint32_t uartOverflows;
  bool truncated;
};

class CaptureEngine {
 public:
  bool request() {
    if (state_ == State::Arming || state_ == State::Recording) return false;
    ++captureId_;
    if (captureId_ == 0) ++captureId_;
    state_ = State::Arming;
    count_ = 0;
    channels_ = 0;
    invalidLines_ = droppedSamples_ = uartOverflows_ = 0;
    resetLine(true);
    return true;
  }

  // Called by core 1 only after it has discarded all queued UART bytes.
  bool start(uint32_t nowUs) {
    if (state_ != State::Arming) return false;
    startedUs_ = nowUs;
    state_ = State::Recording;
    // The first LF synchronises to a line boundary. It may finish a frame
    // that was already in flight when the button was clicked.
    resetLine(true);
    return true;
  }

  void tick(uint32_t nowUs) {
    if (state_ == State::Recording && uint32_t(nowUs - startedUs_) >= kDurationUs) {
      state_ = State::Done;
      resetLine(false);
    }
  }

  void uartOverflow() {
    if (state_ == State::Recording) ++uartOverflows_;
    resetLine(true);
  }

  void transportError() {
    if (state_ == State::Recording) ++invalidLines_;
    resetLine(true);
  }

  void feed(char byte, uint32_t nowUs) {
    tick(nowUs);
    if (state_ != State::Recording) return;
    if (byte == '\n') {
      if (discard_) {
        if (lineOverflow_) ++invalidLines_;
        resetLine(false);
        return;
      }
      if (length_ > 0 && line_[length_ - 1] == '\r') --length_;
      line_[length_] = '\0';
      bool blank = true;
      for (size_t i = 0; i < length_; ++i) {
        if (!space(line_[i])) { blank = false; break; }
      }
      if (!blank) acceptLine(nowUs);
      resetLine(false);
      return;
    }
    if (discard_) return;
    if (length_ >= kMaxLine) {
      discard_ = true;
      lineOverflow_ = true;
      length_ = 0;
      return;
    }
    // A NUL in a transport frame must not silently hide the suffix.
    if (byte == '\0') {
      discard_ = true;
      lineOverflow_ = true;
      length_ = 0;
      return;
    }
    line_[length_++] = byte;
  }

  Status snapshot(uint32_t nowUs) const {
    uint32_t elapsed = 0;
    if (state_ == State::Done) elapsed = kDurationUs;
    else if (state_ == State::Recording) {
      elapsed = uint32_t(nowUs - startedUs_);
      if (elapsed > kDurationUs) elapsed = kDurationUs;
    }
    return {captureId_, state_, elapsed / 1000, count_, channels_, invalidLines_,
            droppedSamples_, uartOverflows_, droppedSamples_ != 0 || uartOverflows_ != 0};
  }

  bool copyPage(uint32_t id, size_t offset, size_t limit, Sample* destination,
                size_t& copied, Status& status, uint32_t nowUs) const {
    status = snapshot(nowUs);
    copied = 0;
    if (id != captureId_ || state_ != State::Done || offset > count_) return false;
    copied = count_ - offset;
    if (copied > limit) copied = limit;
    if (copied) std::memcpy(destination, samples_ + offset, copied * sizeof(Sample));
    return true;
  }

 private:
  static bool space(char c) { return c == ' ' || c == '\t'; }
  static bool delimiter(char c) { return c == ',' || c == ';'; }
  static bool numberCharacter(char c) {
    return (c >= '0' && c <= '9') || c == '+' || c == '-' || c == '.' || c == 'e' || c == 'E';
  }

  static bool parse(const char* input, float* values, uint8_t& count) {
    count = 0;
    const char* cursor = input;
    while (space(*cursor)) ++cursor;
    if (!*cursor) return false;
    while (*cursor) {
      if (count >= kMaxChannels || delimiter(*cursor)) return false;
      errno = 0;
      char* end = nullptr;
      const float value = std::strtof(cursor, &end);
      if (end == cursor || errno == ERANGE || !std::isfinite(value)) return false;
      for (const char* p = cursor; p != end; ++p) {
        if (!numberCharacter(*p)) return false;
      }
      values[count++] = value;
      cursor = end;
      bool separatedBySpace = false;
      while (space(*cursor)) { ++cursor; separatedBySpace = true; }
      if (!*cursor) return true;
      if (delimiter(*cursor)) {
        ++cursor;
        while (space(*cursor)) ++cursor;
        if (!*cursor || delimiter(*cursor)) return false;
      } else if (!separatedBySpace) {
        return false;
      }
    }
    return count != 0;
  }

  void acceptLine(uint32_t nowUs) {
    float values[kMaxChannels] = {};
    uint8_t count = 0;
    if (!parse(line_, values, count) || (channels_ != 0 && count != channels_)) {
      ++invalidLines_;
      return;
    }
    if (channels_ == 0) channels_ = count;
    if (count_ >= kMaxSamples) {
      ++droppedSamples_;
      return;
    }
    Sample& sample = samples_[count_++];
    sample.timeUs = uint32_t(nowUs - startedUs_);
    for (size_t i = 0; i < kMaxChannels; ++i) sample.values[i] = values[i];
  }

  void resetLine(bool discard) {
    length_ = 0;
    discard_ = discard;
    lineOverflow_ = false;
  }

  State state_ = State::Idle;
  uint32_t captureId_ = 0;
  uint32_t startedUs_ = 0;
  size_t count_ = 0;
  uint8_t channels_ = 0;
  uint32_t invalidLines_ = 0;
  uint32_t droppedSamples_ = 0;
  uint32_t uartOverflows_ = 0;
  char line_[kMaxLine + 1] = {};
  size_t length_ = 0;
  bool discard_ = false;
  bool lineOverflow_ = false;
  Sample samples_[kMaxSamples] = {};
};

}  // namespace signal_capture
