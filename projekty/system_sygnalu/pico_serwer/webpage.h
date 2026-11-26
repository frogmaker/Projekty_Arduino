#pragma once
#include <Arduino.h>

// Strona i wykres dzialaja lokalnie, bez internetu i zewnetrznych bibliotek.
static const char INDEX_HTML[] PROGMEM = R"HTML(<!doctype html>
<html lang="pl">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<meta name="theme-color" content="#113c48">
<meta name="description" content="Odbierz sygnał z urządzenia, zarejestruj następne pięć sekund i obejrzyj przebieg w przeglądarce.">
<title>System do obsługi i wyświetlania sygnalu</title>
<style>
:root{color-scheme:light;--paper:#f2f6f8;--card:#fff;--ink:#162e43;--muted:#637788;--line:#dce6eb;--teal:#007e78;--mint:#e2f3ee;--blue:#296dc1;--orange:#d78327;--purple:#8856bc;--bad:#ac3c32;--shadow:0 18px 48px #18364c07}*{box-sizing:border-box}[hidden]{display:none!important}body{margin:0;background:var(--paper);color:var(--ink);font:15px/1.6 system-ui,-apple-system,BlinkMacSystemFont,"Segoe UI",sans-serif;-webkit-font-smoothing:antialiased}button{font:inherit;cursor:pointer}button:disabled{cursor:wait}button:focus-visible,a:focus-visible,canvas:focus-visible,summary:focus-visible{outline:3px solid #d49a31;outline-offset:4px}a{color:var(--teal);text-underline-offset:3px}.wrap{width:min(1120px,calc(100% - 48px));margin:auto}.skip{position:absolute;top:-80px;left:12px;z-index:10;background:var(--ink);color:white;padding:10px 16px}.skip:focus{top:12px}header{border-bottom:1px solid var(--line);background:#ffffff8c}.topbar{min-height:80px;display:flex;align-items:center;justify-content:space-between;gap:16px}.brand{display:flex;gap:12px;align-items:center}.mark{width:39px;height:39px;display:grid;place-items:center;background:#113c48;border-radius:11px;color:#b7ece0}.brand-title{font-size:13px;font-weight:750;letter-spacing:.1em;line-height:1.3}.brand-sub{font-size:11px;color:var(--muted);letter-spacing:.02em}.connection{display:flex;align-items:center;gap:7px;color:var(--muted);font-size:12px;white-space:nowrap}.connection-dot{width:7px;height:7px;border-radius:50%;background:#b4c1ca}.connection.online .connection-dot{background:var(--teal)}.connection.offline .connection-dot{background:var(--bad)}main{padding-bottom:32px}.hero{padding:38px 0 27px;display:flex;justify-content:space-between;align-items:flex-end;gap:35px}.eyebrow{margin:0 0 9px;font-size:11px;font-weight:750;letter-spacing:.15em;text-transform:uppercase;color:var(--teal)}h1{font-size:clamp(28px,3.1vw,40px);line-height:1.15;letter-spacing:-.04em;max-width:750px;margin:0 0 14px;font-weight:750}.intro{max-width:720px;margin:0;color:var(--muted);font-size:14px}.window-badge{flex-shrink:0;border-left:1px solid #cfdde3;padding:0 0 3px 27px;text-align:right}.window-badge strong{display:block;color:var(--teal);font-size:42px;line-height:1.1;letter-spacing:-.05em}.window-badge span{font-size:10px;letter-spacing:.12em;color:var(--muted);text-transform:uppercase}.panel{background:var(--card);border:1px solid var(--line);border-radius:20px;box-shadow:var(--shadow);overflow:hidden}.panel-heading{display:flex;justify-content:space-between;align-items:center;gap:20px;padding:23px 27px 20px;border-bottom:1px solid var(--line)}.panel-heading h2{margin:0 0 3px;font-size:18px;letter-spacing:-.025em}.panel-heading p{margin:0;color:var(--muted);font-size:12px}.capture-button{display:flex;align-items:center;justify-content:center;gap:10px;min-width:164px;min-height:46px;padding:11px 20px;background:var(--teal);color:white;border:0;border-radius:10px;font-size:14px;font-weight:700;white-space:nowrap;box-shadow:0 4px 12px #007e7819}.capture-button:hover:not(:disabled){background:#006960}.capture-button:disabled{background:#86aaa7;box-shadow:none}.record-dot{width:9px;height:9px;border-radius:50%;background:currentColor}.capture-button[aria-busy="true"] .record-dot{animation:pulse 1.1s ease-in-out infinite}@keyframes pulse{50%{opacity:.35;transform:scale(.75)}}.recording-bar{padding:19px 27px 16px;background:#fbfdfd;border-bottom:1px solid var(--line)}.status-row{display:flex;align-items:center;justify-content:space-between;gap:15px;margin-bottom:9px}.status-label{font-size:13px;font-weight:650}.elapsed{font-variant-numeric:tabular-nums;font-size:12px;color:var(--muted)}progress{display:block;width:100%;height:5px;border:0;border-radius:10px;overflow:hidden;background:#e4ecef;accent-color:var(--teal)}progress::-webkit-progress-bar{background:#e4ecef}progress::-webkit-progress-value{background:var(--teal);border-radius:10px}.notice{margin:15px 27px 0;padding:11px 14px;background:#fff7e6;border:1px solid #f0dcad;border-radius:9px;color:#775a1c;font-size:12px}.notice.error{background:#fff0ee;border-color:#ebc6c0;color:#8f352e}.notice button{border:0;padding:0;margin-left:8px;background:transparent;color:inherit;font-size:inherit;font-weight:750;text-decoration:underline;text-underline-offset:3px}.chart-toolbar{display:flex;flex-wrap:wrap;align-items:center;justify-content:space-between;gap:12px;padding:18px 27px 5px}.legend{display:flex;flex-wrap:wrap;gap:7px}.legend-empty{font-size:11px;color:var(--muted)}.channel{display:inline-flex;align-items:center;gap:7px;padding:5px 10px;border:1px solid var(--line);border-radius:30px;background:white;color:var(--ink);font-size:11px;font-weight:650}.channel-dot{width:7px;height:7px;border-radius:50%;background:var(--channel)}.channel[aria-pressed="false"]{color:#8998a3;background:#f6f8f9}.channel[aria-pressed="false"] .channel-dot{background:#b5c0c8}.capture-id{font-size:11px;color:var(--muted);font-variant-numeric:tabular-nums}.plot-wrap{height:338px;position:relative;margin:0 11px}.plot-wrap canvas{display:block;width:100%;height:100%;touch-action:pan-y}.empty-state{position:absolute;inset:45px 50px 60px 70px;display:flex;flex-direction:column;align-items:center;justify-content:center;text-align:center;pointer-events:none}.empty-symbol{display:grid;place-items:center;width:43px;height:43px;border-radius:50%;background:#edf5f5;color:var(--teal);margin-bottom:11px}.empty-state strong{font-size:14px;font-weight:650}.empty-state span{font-size:12px;color:var(--muted);margin-top:5px;max-width:340px}.chart-foot{border-top:1px solid var(--line);padding:12px 27px;display:flex;align-items:flex-start;justify-content:space-between;gap:20px;font-size:11px;color:var(--muted);min-height:46px}.chart-foot p{margin:0}.point-readout{font-variant-numeric:tabular-nums;text-align:right;color:#31566b;max-width:60%}.facts{display:grid;grid-template-columns:repeat(3,1fr);margin-top:16px;border:1px solid var(--line);border-radius:13px;background:#ffffff8c}.fact{padding:17px 22px}.fact+.fact{border-left:1px solid var(--line)}.fact-label{font-size:10px;color:var(--muted);text-transform:uppercase;letter-spacing:.1em}.fact-value{display:block;font-size:22px;font-weight:700;letter-spacing:-.03em;line-height:1.4;font-variant-numeric:tabular-nums}.fact small{font-size:11px;color:var(--muted)}.quality{padding:11px 14px;border:1px solid #f0dcad;background:#fff8e8;border-radius:10px;color:#775a1c;font-size:12px;margin-top:14px}.guide{display:grid;grid-template-columns:repeat(3,1fr);gap:28px;padding:28px 3px 22px}.guide h3{margin:0 0 6px;font-size:12px;font-weight:700}.guide h3 span{font-size:10px;margin-right:8px;color:var(--teal)}.guide p{font-size:12px;color:var(--muted);margin:0}.details{border-top:1px solid var(--line);padding:16px 0 0;font-size:12px;color:var(--muted)}.details summary{cursor:pointer;color:#34586c;font-weight:650}.details p{max-width:850px}.details code{padding:2px 5px;border-radius:4px;background:#e5edf0;font-size:11px;color:#315569}.details strong{color:var(--ink)}footer{border-top:1px solid var(--line);padding:20px 0 25px;font-size:11px;color:var(--muted)}.footer-inner{display:flex;justify-content:space-between;gap:20px}.footer-inner p{margin:0}.noscript{padding:15px;margin:15px 0;background:#fff4df;border:1px solid #e7d2a6;border-radius:9px}
@media(max-width:700px){.wrap{width:calc(100% - 30px)}.topbar{min-height:70px}.brand-sub{font-size:10px}.brand-title{font-size:11px}.connection{font-size:10px}.hero{padding:27px 0 21px;gap:15px;align-items:center}.window-badge{padding-left:17px}.window-badge strong{font-size:34px}.window-badge span{font-size:8px}h1{font-size:28px}.intro{font-size:12px}.panel-heading{padding:18px;gap:14px}.panel-heading h2{font-size:16px}.panel-heading p{font-size:11px}.capture-button{font-size:12px;min-width:135px;padding:10px 13px;min-height:43px}.recording-bar{padding:15px 18px}.chart-toolbar{padding:15px 18px 4px}.plot-wrap{height:300px;margin:0 3px}.chart-foot{padding:12px 18px;display:block}.point-readout{display:block;text-align:left;margin-top:5px;max-width:100%}.facts{margin-top:13px}.fact{padding:14px 15px}.fact-value{font-size:21px}.fact-label{font-size:8px}.fact small{font-size:10px}.guide{gap:18px;padding-top:22px}.guide p{font-size:11px}.guide h3{font-size:11px}.notice{margin:12px 18px 0;font-size:11px}.empty-state{inset:40px 25px 60px 65px}.empty-state strong{font-size:13px}.empty-state span{font-size:11px}}
@media(max-width:450px){.brand-sub{max-width:150px;line-height:1.3}.mark{width:33px;height:33px;border-radius:9px}.brand{gap:8px}.hero{display:block}.window-badge{display:none}h1{font-size:29px;max-width:390px}.panel-heading{display:block}.capture-button{margin-top:15px;width:100%}.chart-toolbar{align-items:flex-start}.capture-id{font-size:10px}.plot-wrap{height:276px}.legend{gap:5px}.channel{padding:4px 8px;font-size:10px}.status-label{font-size:12px}.elapsed{font-size:11px}.fact{padding:13px 10px}.fact-value{font-size:20px}.guide{grid-template-columns:1fr;gap:14px}.guide h3{margin-bottom:3px}.guide p{font-size:12px}.footer-inner{display:block}.footer-inner p+p{margin-top:5px}}
@media(prefers-reduced-motion:reduce){*,*:before,*:after{animation:none!important;scroll-behavior:auto!important}}
</style>
</head>
<body>
<a class="skip" href="#rejestracja">Przejdź do rejestracji</a>
<header><div class="wrap topbar"><div class="brand"><span class="mark" aria-hidden="true"><svg width="24" height="22" viewBox="0 0 24 22" fill="none"><path d="M1 12h4l3-8 5 15 3-10 3 3h4" stroke="currentColor" stroke-width="1.7" stroke-linecap="round" stroke-linejoin="round"/></svg></span><div><div class="brand-title">PICO / SYGNAŁ</div><div class="brand-sub">Lokalne stanowisko pomiarowe</div></div></div><div id="connection" class="connection" role="status"><span class="connection-dot" aria-hidden="true"></span><span id="connectionText">Łączenie z Pico…</span></div></div></header>
<main class="wrap">
<section class="hero"><div><p class="eyebrow">Odbiór · rejestracja · wizualizacja</p><h1>System do obsługi i wyświetlania sygnalu</h1><p class="intro">Zarejestruj następne pięć sekund danych z podłączonego urządzenia. Obejrzyj przebieg, porównaj kanały i rozpocznij kolejny pomiar, kiedy zechcesz.</p></div><div class="window-badge" aria-label="Okno rejestracji: 5 sekund"><strong>5 s</strong><span>okno rejestracji</span></div></section>
<noscript><p class="noscript">Do obsługi przycisku i rysowania wykresu potrzebny jest JavaScript. Włącz go w ustawieniach przeglądarki.</p></noscript>
<section class="panel" id="rejestracja" aria-labelledby="chartTitle">
<div class="panel-heading"><div><h2 id="chartTitle">Rejestracja sygnału</h2><p>Nowe dane po kliknięciu. Gotowy wykres pozostaje do następnego pomiaru.</p></div><button id="captureButton" class="capture-button" type="button" disabled aria-busy="false"><span class="record-dot" aria-hidden="true"></span>zbieraj dane</button></div>
<div class="recording-bar"><div class="status-row"><span id="captureStatus" class="status-label" role="status" aria-live="polite">Sprawdzanie stanu urządzenia…</span><span id="elapsed" class="elapsed">0,0 / 5,0 s</span></div><progress id="captureProgress" max="5000" value="0" aria-label="Postęp pięciosekundowej rejestracji"></progress></div>
<div id="notice" class="notice" hidden role="alert"><span id="noticeText"></span><button id="retryButton" type="button">spróbuj ponownie</button></div>
<div class="chart-toolbar"><div id="legend" class="legend" aria-label="Widoczne kanały"><span class="legend-empty">Kanały pojawią się po rejestracji</span></div><span id="captureId" class="capture-id">Bez rejestracji</span></div>
<div class="plot-wrap"><canvas id="chart" tabindex="0" role="img" aria-label="Wykres sygnału. Brak zarejestrowanych danych.">Wykres odebranego sygnału. Szczegóły stanu i liczby próbek są dostępne pod wykresem.</canvas><div id="chartEmpty" class="empty-state"><div class="empty-symbol" aria-hidden="true"><svg width="22" height="21" viewBox="0 0 22 21" fill="none"><path d="M2 2v16h18M5 12l4-5 4 7 5-9" stroke="currentColor" stroke-width="1.5" stroke-linecap="round" stroke-linejoin="round"/></svg></div><strong id="emptyTitle">Czekamy na pierwszy pomiar</strong><span id="emptyDetail">Po połączeniu kliknij „zbieraj dane”, aby zarejestrować następne 5 sekund sygnału.</span></div></div>
<div class="chart-foot"><p>Linie łączą odebrane próbki. Oś X: czas odbioru przez Pico.</p><output id="pointReadout" class="point-readout">Najedź na wykres lub wybierz go klawiszem Tab.</output></div>
</section>
<section class="facts" aria-label="Informacje o rejestracji"><div class="fact"><span class="fact-label">Odebrane próbki</span><strong id="sampleCount" class="fact-value">—</strong><small id="sampleDetail">Dane pojawią się po rozpoczęciu</small></div><div class="fact"><span class="fact-label">Kanały sygnału</span><strong id="channelCount" class="fact-value">—</strong><small>Wspólna oś wartości źródła</small></div><div class="fact"><span class="fact-label">Okno danych</span><strong class="fact-value">5 sekund</strong><small id="windowDetail">Rejestracja uruchamiana przyciskiem</small></div></section>
<div id="quality" class="quality" hidden role="status"></div>
<section class="guide" aria-label="Jak działa system"><div><h3><span>01</span>Podłącz źródło</h3><p>Urządzenie pomiarowe wysyła wartości liczbowe przez UART. EKG jest jednym z możliwych źródeł.</p></div><div><h3><span>02</span>Zbierz nowe dane</h3><p>Przycisk rozpoczyna nowe okno rejestracji. Pico zapisuje dane odebrane w następnych pięciu sekundach.</p></div><div><h3><span>03</span>Obejrzyj przebieg</h3><p>Włączaj kanały w legendzie. Strzałki ← → na aktywnym wykresie wybierają kolejne próbki.</p></div></section>
<details class="details"><summary>Źródło danych i znaczenie osi</summary><p>Odbiornik przyjmuje od 1 do 4 kanałów liczbowych w wierszu. Jednostki zależą od podłączonego urządzenia: mogą to być zliczenia ADC, temperatura lub inne wartości. Wykres nie przelicza ich automatycznie na napięcie. <strong>Czas na osi X jest liczony od rozpoczęcia odbioru przez Pico.</strong> Format transmisji i połączenia opisano w instrukcji projektu.</p><p>Panel działa w lokalnej sieci Pico i nie potrzebuje internetu. Przykład z elektrodami EKG służy edukacji; system nie stanowi narzędzia diagnostycznego.</p></details>
</main><footer><div class="wrap footer-inner"><p>Pico 2 W · lokalny serwer WWW · dane z podłączonego urządzenia</p><p>1–4 kanały / jedno okno 5 s</p></div></footer>
<script>
'use strict';
(function () {
  const DURATION = 5000, MAX_POINTS = 4096, MAX_CHANNELS = 4, PAGE_SIZE = 256;
  const COLORS = ['#007e78', '#296dc1', '#d78327', '#8856bc'];
  const STATES = new Set(['idle', 'arming', 'recording', 'done']);
  const byId = id => document.getElementById(id);
  const el = {button:byId('captureButton'), connection:byId('connection'), connectionText:byId('connectionText'), status:byId('captureStatus'), elapsed:byId('elapsed'), progress:byId('captureProgress'), notice:byId('notice'), noticeText:byId('noticeText'), retry:byId('retryButton'), legend:byId('legend'), captureId:byId('captureId'), canvas:byId('chart'), empty:byId('chartEmpty'), emptyTitle:byId('emptyTitle'), emptyDetail:byId('emptyDetail'), readout:byId('pointReadout'), samples:byId('sampleCount'), sampleDetail:byId('sampleDetail'), channels:byId('channelCount'), windowDetail:byId('windowDetail'), quality:byId('quality')};
  const model = {status:null, connected:false, posting:false, loading:false, dataError:false, points:[], channels:0, visible:[], loadedId:null, generation:0, loadToken:0, nextDataAttempt:0, selected:-1};
  let pollTimer = 0, polling = false, drawFrame = 0, plotBounds = null;

  function integer(value, min, max) { return Number.isSafeInteger(value) && value >= min && value <= max; }
  function validateStatus(value) {
    if (!value || !integer(value.capture_id,0,0xffffffff) || !STATES.has(value.state) || value.duration_ms !== DURATION || !integer(value.elapsed_ms,0,DURATION) || value.max_samples !== MAX_POINTS || !integer(value.samples,0,MAX_POINTS) || !integer(value.channels,0,MAX_CHANNELS) || typeof value.truncated !== 'boolean') throw new Error('Nieprawidłowa odpowiedź o stanie urządzenia.');
    for (const key of ['invalid_lines','dropped_samples','uart_overflows']) if (!integer(value[key],0,0xffffffff)) throw new Error('Nieprawidłowe liczniki odbioru danych.');
    if (value.samples > 0 && value.channels < 1) throw new Error('Brak informacji o kanałach sygnału.');
    return value;
  }
  function validatePage(value, id, offset, expectedTotal, expectedChannels, previousTime) {
    if (!value || value.capture_id !== id || value.state !== 'done' || value.offset !== offset || !integer(value.total,0,MAX_POINTS) || !integer(value.channels,0,MAX_CHANNELS) || !Array.isArray(value.points) || value.points.length > PAGE_SIZE || offset + value.points.length > value.total || (value.total > 0 && (value.channels < 1 || value.points.length === 0))) throw new Error('Nieprawidłowy fragment danych wykresu.');
    if (expectedTotal !== null && (value.total !== expectedTotal || value.channels !== expectedChannels)) throw new Error('Parametry danych zmieniły się podczas pobierania.');
    let last = previousTime;
    for (const point of value.points) {
      if (!Array.isArray(point) || point.length !== value.channels + 1 || !Number.isFinite(point[0]) || point[0] < 0 || point[0] >= DURATION || point[0] < last) throw new Error('Nieprawidłowy czas lub liczba kanałów próbki.');
      for (let i = 1; i < point.length; i++) if (typeof point[i] !== 'number' || !Number.isFinite(point[i]) || Math.abs(point[i]) > 3.4028235e38) throw new Error('Próbka zawiera nieprawidłową wartość.');
      last = point[0];
    }
    return value;
  }
  async function requestJson(path, options) {
    const controller = new AbortController();
    const timeout = setTimeout(() => controller.abort(), 7000);
    try {
      const response = await fetch(path, Object.assign({cache:'no-store'}, options || {}, {signal:controller.signal}));
      if (!response.ok) { const error = new Error('Błąd odpowiedzi HTTP ' + response.status + '.'); error.status = response.status; throw error; }
      return await response.json();
    } finally { clearTimeout(timeout); }
  }
  function showNotice(text, danger) { el.noticeText.textContent = text; el.notice.classList.toggle('error', Boolean(danger)); el.notice.hidden = false; }
  function clearNotice() { el.notice.hidden = true; }
  function connection(ok) {
    model.connected = ok;
    el.connection.classList.toggle('online',ok); el.connection.classList.toggle('offline',!ok);
    el.connectionText.textContent = ok ? 'Połączono z Pico' : 'Brak połączenia';
    updateButton();
  }
  function updateButton() {
    const active = model.status && (model.status.state === 'arming' || model.status.state === 'recording');
    el.button.disabled = !model.connected || model.posting || model.loading || Boolean(active);
    el.button.setAttribute('aria-busy', String(model.posting || Boolean(active)));
  }
  function resetPlot() {
    model.points = []; model.channels = 0; model.visible = []; model.loadedId = null; model.selected = -1;
    el.legend.replaceChildren(); const text = document.createElement('span'); text.className = 'legend-empty'; text.textContent = 'Kanały pojawią się po rejestracji'; el.legend.append(text);
    el.readout.textContent = 'Najedź na wykres lub wybierz go klawiszem Tab.';
    requestDraw();
  }
  function updateStatus(status) {
    if (!model.status || model.status.capture_id !== status.capture_id || (model.status.state === 'done' && ['arming','recording'].includes(status.state)) || (model.status.state !== 'idle' && status.state === 'idle')) {
      model.generation++; model.nextDataAttempt = 0; model.dataError = false; clearNotice(); resetPlot();
    }
    model.status = status;
    el.captureId.textContent = status.capture_id ? 'Rejestracja #' + status.capture_id : 'Bez rejestracji';
    el.elapsed.textContent = (status.elapsed_ms/1000).toFixed(1).replace('.',',') + ' / 5,0 s';
    el.progress.value = status.elapsed_ms;
    el.samples.textContent = status.state === 'idle' ? '—' : status.samples.toLocaleString('pl-PL');
    el.channels.textContent = status.channels || '—';
    el.sampleDetail.textContent = status.state === 'idle' ? 'Dane pojawią się po rozpoczęciu' : 'Limit bufora: ' + MAX_POINTS.toLocaleString('pl-PL') + ' próbek';
    const labels = {idle:'Gotowe do rejestracji', arming:'Przygotowanie nowej rejestracji…', recording:'Zbieranie nowych danych…', done: status.samples ? 'Rejestracja zakończona' : 'Rejestracja zakończona — brak danych'};
    el.status.textContent = model.loading ? 'Pobieranie zarejestrowanych danych…' : labels[status.state];
    el.windowDetail.textContent = status.state === 'done' ? 'Gotowy fragment pozostaje na wykresie' : 'Rejestracja uruchamiana przyciskiem';
    const warnings = [];
    if (status.invalid_lines) warnings.push('Nieprawidłowe wiersze: ' + status.invalid_lines);
    if (status.dropped_samples) warnings.push('Pominięte próbki: ' + status.dropped_samples);
    if (status.uart_overflows) warnings.push('Przepełnienia UART: ' + status.uart_overflows);
    if (status.truncated) warnings.push('Osiągnięto limit bufora — zapis jest niepełny');
    el.quality.hidden = warnings.length === 0;
    el.quality.textContent = warnings.length ? warnings.join(' · ') + '. Sprawdź format i szybkość nadawania źródła.' : '';
    updateButton(); requestDraw();
  }
  function schedulePoll(delay) { clearTimeout(pollTimer); pollTimer = setTimeout(pollStatus, delay); }
  async function pollStatus() {
    if (polling || model.posting || model.loading) { schedulePoll(300); return; }
    polling = true;
    const generation = model.generation;
    try {
      const status = validateStatus(await requestJson('/api/status'));
      if (generation !== model.generation || model.posting) return;
      connection(true); if (!model.dataError) clearNotice(); updateStatus(status);
      if (status.state === 'done' && model.loadedId !== status.capture_id && Date.now() >= model.nextDataAttempt) loadData(status.capture_id, model.generation);
    } catch (error) {
      if (generation === model.generation && !model.posting) { connection(false); showNotice('Nie udało się odczytać stanu Pico. Sprawdź połączenie Wi-Fi i spróbuj ponownie.', true); }
    } finally {
      polling = false;
      const active = model.status && (model.status.state === 'arming' || model.status.state === 'recording');
      schedulePoll(model.connected ? (active ? 200 : 1200) : 1500);
    }
  }
  async function startCapture() {
    if (el.button.disabled) return;
    model.posting = true; model.generation++; model.dataError = false; clearNotice(); updateButton();
    try {
      const accepted = await requestJson('/api/capture', {method:'POST', headers:{'Content-Type':'text/plain'}, body:''});
      if (!accepted || !integer(accepted.capture_id,1,0xffffffff) || !['arming','recording'].includes(accepted.state)) throw new Error('Nieprawidłowe potwierdzenie rozpoczęcia rejestracji.');
      connection(true);
      updateStatus({capture_id:accepted.capture_id,state:accepted.state,duration_ms:DURATION,elapsed_ms:0,samples:0,channels:0,max_samples:MAX_POINTS,invalid_lines:0,dropped_samples:0,uart_overflows:0,truncated:false});
    } catch (error) {
      if (error.status === 409) showNotice('Trwa już rejestracja uruchomiona w innym oknie. Odczytuję aktualny stan.', false);
      else { connection(false); showNotice('Nie udało się potwierdzić startu. Sprawdź połączenie z Pico; panel odczyta stan przed kolejną próbą.', true); }
    } finally { model.posting = false; updateButton(); schedulePoll(0); }
  }
  async function loadData(id, generation) {
    if (model.loading) return;
    const token = ++model.loadToken;
    model.loading = true; updateButton(); el.status.textContent = 'Pobieranie zarejestrowanych danych…';
    try {
      const points = [];
      let total = null, channels = null, offset = 0, lastTime = -1;
      do {
        const page = validatePage(await requestJson('/api/data?id=' + id + '&offset=' + offset + '&limit=' + PAGE_SIZE), id, offset, total, channels, lastTime);
        if (generation !== model.generation) return;
        if (total === null) { total = page.total; channels = page.channels; }
        for (const point of page.points) points.push(point);
        offset += page.points.length;
        if (points.length) lastTime = points[points.length-1][0];
      } while (offset < total);
      const finalStatus = validateStatus(await requestJson('/api/status'));
      if (generation !== model.generation) return;
      if (finalStatus.capture_id !== id || finalStatus.state !== 'done' || finalStatus.samples !== total || finalStatus.channels !== channels) {
        updateStatus(finalStatus);
        throw new Error('Rejestracja zmieniła się podczas pobierania.');
      }
      model.points = points; model.channels = channels; model.visible = Array(channels).fill(true); model.loadedId = id; model.selected = -1; model.dataError = false;
      connection(true); clearNotice(); updateStatus(finalStatus); buildLegend(); requestDraw();
    } catch (error) {
      if (generation === model.generation) {
        model.dataError = true;
        model.nextDataAttempt = Date.now() + 2000;
        showNotice(error.status === 409 ? 'Dane zmieniły się podczas pobierania. Odczytuję aktualną rejestrację.' : 'Nie udało się pobrać poprawnych danych wykresu. Sprawdź połączenie lub spróbuj ponownie.', error.status !== 409);
      }
    } finally {
      if (token === model.loadToken) {
        model.loading = false; updateButton();
        if (model.status && model.status.state === 'done') el.status.textContent = model.loadedId === id ? (model.points.length ? 'Rejestracja zakończona' : 'Rejestracja zakończona — brak danych') : 'Oczekiwanie na dane wykresu…';
        schedulePoll(250);
      }
    }
  }
  function buildLegend() {
    el.legend.replaceChildren();
    if (!model.channels) { const text = document.createElement('span'); text.className = 'legend-empty'; text.textContent = 'Brak odebranych kanałów'; el.legend.append(text); return; }
    for (let channel = 0; channel < model.channels; channel++) {
      const button = document.createElement('button'); button.type = 'button'; button.className = 'channel'; button.style.setProperty('--channel',COLORS[channel]); button.setAttribute('aria-pressed','true');
      const dot = document.createElement('span'); dot.className = 'channel-dot'; dot.setAttribute('aria-hidden','true'); button.append(dot,document.createTextNode('Kanał ' + (channel+1)));
      button.addEventListener('click',() => { model.visible[channel] = !model.visible[channel]; button.setAttribute('aria-pressed',String(model.visible[channel])); updateReadout(); requestDraw(); });
      el.legend.append(button);
    }
  }
  function numberLabel(value) {
    if (!Number.isFinite(value)) return '—';
    const magnitude = Math.abs(value);
    if (magnitude >= 1000000 || (magnitude > 0 && magnitude < .001)) return value.toExponential(1).replace('.',',');
    return Number(value.toPrecision(5)).toLocaleString('pl-PL',{maximumFractionDigits:4});
  }
  function yScale() {
    let low = Infinity, high = -Infinity;
    for (const point of model.points) for (let channel = 0; channel < model.channels; channel++) if (model.visible[channel]) { low = Math.min(low,point[channel+1]); high = Math.max(high,point[channel+1]); }
    if (!Number.isFinite(low)) return {low:-1,high:1,step:.5};
    const padding = low === high ? Math.max(Math.abs(low)*.1,1) : (high-low)*.08;
    low -= padding; high += padding;
    const rough = (high-low)/4, power = Math.pow(10,Math.floor(Math.log10(rough))), fraction = rough/power;
    const step = (fraction <= 1 ? 1 : fraction <= 2 ? 2 : fraction <= 2.5 ? 2.5 : fraction <= 5 ? 5 : 10)*power;
    return {low:Math.floor(low/step)*step,high:Math.ceil(high/step)*step,step:step};
  }
  function emptyMessage() {
    if (model.points.length && model.visible.some(Boolean)) return null;
    if (model.points.length) return ['Wybierz kanał do wyświetlenia','Włącz przynajmniej jeden kanał w legendzie nad wykresem.'];
    if (model.loading) return ['Pobieranie danych…','Gotowy fragment za chwilę pojawi się na wykresie.'];
    const state = model.status && model.status.state;
    if (state === 'arming' || state === 'recording') return ['Trwa nowa rejestracja','Pico zbiera dane z następnych pięciu sekund. Wykres pojawi się po zakończeniu.'];
    if (state === 'done' && model.loadedId === model.status.capture_id) return ['Brak danych źródła','W tym oknie nie odebrano poprawnych próbek. Sprawdź nadawanie, format UART i połączenia.'];
    if (state === 'done') return ['Oczekiwanie na dane wykresu','Panel pobierze kompletną rejestrację po odzyskaniu połączenia.'];
    return ['Czekamy na pierwszy pomiar','Kliknij „zbieraj dane”, aby zarejestrować następne 5 sekund sygnału.'];
  }
  function requestDraw() { if (!drawFrame) drawFrame = requestAnimationFrame(() => { drawFrame = 0; draw(); }); }
  function draw() {
    const rect = el.canvas.getBoundingClientRect(), width = Math.max(1,rect.width), height = Math.max(1,rect.height), ratio = Math.min(window.devicePixelRatio || 1,2);
    const pixelWidth = Math.round(width*ratio), pixelHeight = Math.round(height*ratio);
    if (el.canvas.width !== pixelWidth || el.canvas.height !== pixelHeight) { el.canvas.width = pixelWidth; el.canvas.height = pixelHeight; }
    const ctx = el.canvas.getContext('2d'); if (!ctx) return;
    ctx.setTransform(ratio,0,0,ratio,0,0); ctx.clearRect(0,0,width,height);
    const left = width < 380 ? 61 : 69, right = width-18, top = 25, bottom = height-44;
    const scale = yScale(), x = value => left + value/DURATION*(right-left), y = value => bottom-(value-scale.low)/(scale.high-scale.low)*(bottom-top);
    plotBounds = {left:left,right:right,top:top,bottom:bottom};
    ctx.font = '11px system-ui, sans-serif'; ctx.lineWidth = 1; ctx.textBaseline = 'middle';
    for (let i = 0; i <= 5; i++) { const px = x(i*1000); ctx.beginPath(); ctx.strokeStyle = '#e7eef1'; ctx.moveTo(px,top); ctx.lineTo(px,bottom); ctx.stroke(); ctx.fillStyle = '#6c7f8e'; ctx.textAlign = 'center'; ctx.fillText(String(i),px,bottom+18); }
    const tickCount = Math.min(8,Math.round((scale.high-scale.low)/scale.step));
    for (let i = 0; i <= tickCount; i++) { const value = scale.low+i*scale.step, py = y(value); ctx.beginPath(); ctx.strokeStyle = Math.abs(value)<scale.step*.001 ? '#cedce3' : '#e7eef1'; ctx.moveTo(left,py); ctx.lineTo(right,py); ctx.stroke(); ctx.fillStyle = '#6c7f8e'; ctx.textAlign = 'right'; ctx.fillText(numberLabel(value),left-9,py); }
    ctx.fillStyle = '#6c7f8e'; ctx.textAlign = 'left'; ctx.fillText('wartość',left,11); ctx.textAlign = 'right'; ctx.fillText('czas [s]',right,height-6);
    const message = emptyMessage(); el.empty.hidden = !message;
    if (message) { el.emptyTitle.textContent = message[0]; el.emptyDetail.textContent = message[1]; }
    el.canvas.dataset.pointCount = String(model.points.length);
    el.canvas.setAttribute('aria-label','Wykres sygnału, okno od 0 do 5 sekund, ' + model.points.length + ' próbek i ' + model.channels + ' kanałów. Strzałki wybierają próbki, Home i End początek lub koniec.');
    if (!model.points.length) return;
    ctx.save(); ctx.beginPath(); ctx.rect(left,top,right-left,bottom-top); ctx.clip();
    for (let channel = 0; channel < model.channels; channel++) {
      if (!model.visible[channel]) continue;
      ctx.strokeStyle = COLORS[channel]; ctx.fillStyle = COLORS[channel]; ctx.lineWidth = 1.65; ctx.lineJoin = 'round'; ctx.lineCap = 'round'; ctx.beginPath();
      for (let i = 0; i < model.points.length; i++) { const point = model.points[i], px = x(point[0]), py = y(point[channel+1]); if (i === 0) ctx.moveTo(px,py); else ctx.lineTo(px,py); }
      ctx.stroke();
      if (model.points.length === 1) { const point = model.points[0]; ctx.beginPath(); ctx.arc(x(point[0]),y(point[channel+1]),3.5,0,Math.PI*2); ctx.fill(); }
    }
    if (model.selected >= 0 && model.selected < model.points.length) {
      const point = model.points[model.selected], px = x(point[0]); ctx.strokeStyle = '#7d96a3'; ctx.lineWidth = 1; ctx.setLineDash([4,4]); ctx.beginPath(); ctx.moveTo(px,top); ctx.lineTo(px,bottom); ctx.stroke(); ctx.setLineDash([]);
      for (let channel = 0; channel < model.channels; channel++) if (model.visible[channel]) { ctx.fillStyle = COLORS[channel]; ctx.beginPath(); ctx.arc(px,y(point[channel+1]),3.4,0,Math.PI*2); ctx.fill(); }
    }
    ctx.restore();
  }
  function updateReadout() {
    if (model.selected < 0 || model.selected >= model.points.length) return;
    const point = model.points[model.selected], values = [];
    for (let channel = 0; channel < model.channels; channel++) if (model.visible[channel]) values.push('K' + (channel+1) + ': ' + numberLabel(point[channel+1]));
    el.readout.textContent = (point[0]/1000).toFixed(3).replace('.',',') + ' s' + (values.length ? ' · ' + values.join(' · ') : ' · brak widocznych kanałów');
  }
  function chooseByPointer(event) {
    if (!plotBounds || !model.points.length) return;
    const rect = el.canvas.getBoundingClientRect(), px = event.clientX-rect.left;
    const target = Math.max(0,Math.min(DURATION,(px-plotBounds.left)/(plotBounds.right-plotBounds.left)*DURATION));
    let low = 0, high = model.points.length-1;
    while (low < high) { const middle = (low+high)>>1; if (model.points[middle][0] < target) low = middle+1; else high = middle; }
    if (low > 0 && Math.abs(model.points[low-1][0]-target) < Math.abs(model.points[low][0]-target)) low--;
    model.selected = low; updateReadout(); requestDraw();
  }
  el.button.addEventListener('click',startCapture);
  el.retry.addEventListener('click',() => { model.nextDataAttempt = 0; clearNotice(); schedulePoll(0); });
  el.canvas.addEventListener('pointermove',chooseByPointer);
  el.canvas.addEventListener('pointerdown',chooseByPointer);
  el.canvas.addEventListener('pointerleave',() => { if (document.activeElement !== el.canvas) { model.selected = -1; el.readout.textContent = 'Najedź na wykres lub wybierz go klawiszem Tab.'; requestDraw(); } });
  el.canvas.addEventListener('focus',() => { if (model.points.length && model.selected < 0) { model.selected = 0; updateReadout(); requestDraw(); } });
  el.canvas.addEventListener('keydown',event => {
    if (!model.points.length || !['ArrowLeft','ArrowRight','Home','End'].includes(event.key)) return;
    event.preventDefault();
    if (event.key === 'Home') model.selected = 0;
    else if (event.key === 'End') model.selected = model.points.length-1;
    else model.selected = Math.max(0,Math.min(model.points.length-1,(model.selected < 0 ? 0 : model.selected)+(event.key === 'ArrowLeft' ? -1 : 1)));
    updateReadout(); requestDraw();
  });
  window.addEventListener('resize',requestDraw);
  document.addEventListener('visibilitychange',() => { if (!document.hidden) schedulePoll(0); });
  if (typeof ResizeObserver !== 'undefined') new ResizeObserver(requestDraw).observe(el.canvas);
  requestDraw(); schedulePoll(0);
}());
</script>
</body>
</html>)HTML";
