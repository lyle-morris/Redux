var CONFIG_URL = 'https://lyle-morris.github.io/Hosting/apps/redux/app-config-2.1.0.html';
var CONFIG_VERSION = 'redux-2.1.0-config-20260828c';
var SETTINGS_KEY = 'redux_qa_settings_v2';

var WEATHER_TEMP_KEY = 26;
var LOCATION_TIME_OFFSET_KEY = 27;
var USE_LOCATION_TIME_KEY = 28;
var REQUEST_WEATHER_KEY = 29;
var WEATHER_REFRESH_DEBOUNCE_MS = 60000;
var WEATHER_CACHE_ID = 'redux_weather_cache_id';
var WEATHER_TEMP_STORAGE = 'redux_weather_temp';
var WEATHER_OFFSET_STORAGE = 'redux_location_time_offset';
var FALLBACK_LATITUDE = 34.0232;
var FALLBACK_LONGITUDE = -84.3616;

var weatherState = {temp:null, offset:null};
var lastWeatherRequestAt = 0;

var DEFAULTS = {
  showLeadingZero: true,
  hour24: false,
  celsius: false,
  showBluetooth: false,
  manualLocation: false,
  manualPostalCode: '',
  manualCity: '',
  manualCountry: 'US',
  language: 'en',
  analyticsEnabled: true,
  verticalLayout: false,
  layout: 'horizontal_2',
  slot1Metric: 'calendar',
  slot2Metric: 'weather',
  slot3Metric: 'none',
  themeMode: 'preset',
  theme: 'orange',
  showBatteryIndicator: true,
  watchfaceBackground: '#000000',
  boxBackgroundColor: '#ffffff',
  boxTopBorderColor: '#000000',
  boxBottomBorderColor: '#000000',
  trayBackgroundColor: '#ff5500',
  dividerColor: '#000000',
  timeTextColor: '#000000',
  dateTextColor: '#000000',
  slot1TextColor: '#000000',
  slot2TextColor: '#000000',
  slot3TextColor: '#000000',
  batteryIndicatorColor: '#ff5500'
};

function first(settings, camel, snake, fallback) {
  if(settings[camel] !== undefined) return settings[camel];
  if(settings[snake] !== undefined) return settings[snake];
  return fallback;
}

function clean(value) {
  return value === undefined || value === null ? '' : String(value).replace(/^\s+|\s+$/g, '');
}

function normalize(raw) {
  raw = raw || {};
  var vertical = !!first(raw, 'verticalLayout', 'vertical_layout', DEFAULTS.verticalLayout);
  var slot3 = first(raw, 'slot3Metric', 'slot_3_metric', DEFAULTS.slot3Metric);
  var inferredLayout = vertical
    ? (slot3 === 'none' ? 'vertical_2' : 'vertical_3')
    : (slot3 === 'none' ? 'horizontal_2' : 'horizontal_3');
  var layout = first(raw, 'layout', 'layout', inferredLayout);
  var theme = first(raw, 'theme', 'theme', DEFAULTS.theme);
  var postal = clean(first(raw, 'manualPostalCode', 'manual_postal_code', DEFAULTS.manualPostalCode));
  var city = clean(first(raw, 'manualCity', 'manual_city', DEFAULTS.manualCity));
  var country = clean(first(raw, 'manualCountry', 'manual_country', DEFAULTS.manualCountry)).toUpperCase();
  if(!/^[A-Z]{2}$/.test(country)) country = DEFAULTS.manualCountry;
  if(layout !== 'vertical_2' && layout !== 'vertical_3' && layout !== 'horizontal_3' && layout !== 'horizontal_2') {
    layout = DEFAULTS.layout;
  }
  if(theme === 'pink') theme = 'red';

  return {
    showLeadingZero: !!first(raw, 'showLeadingZero', 'show_leading_zero', DEFAULTS.showLeadingZero),
    hour24: !!first(raw, 'hour24', 'use_24_hour', DEFAULTS.hour24),
    celsius: !!first(raw, 'celsius', 'use_celsius', DEFAULTS.celsius),
    showBluetooth: !!first(raw, 'showBluetooth', 'show_bluetooth', DEFAULTS.showBluetooth),
    manualLocation: !!(postal || city),
    manualPostalCode: postal,
    manualCity: city,
    manualCountry: country,
    language: first(raw, 'language', 'language', DEFAULTS.language),
    analyticsEnabled: !!first(raw, 'analyticsEnabled', 'analytics_enabled', DEFAULTS.analyticsEnabled),
    verticalLayout: layout === 'vertical_2' || layout === 'vertical_3',
    layout: layout,
    slot1Metric: first(raw, 'slot1Metric', 'slot_1_metric', DEFAULTS.slot1Metric),
    slot2Metric: first(raw, 'slot2Metric', 'slot_2_metric', DEFAULTS.slot2Metric),
    slot3Metric: layout === 'horizontal_2' || layout === 'vertical_2' ? 'none' : slot3,
    themeMode: first(raw, 'themeMode', 'theme_mode', DEFAULTS.themeMode),
    theme: theme,
    showBatteryIndicator: !!first(raw, 'showBatteryIndicator', 'show_battery_indicator', DEFAULTS.showBatteryIndicator),
    watchfaceBackground: first(raw, 'watchfaceBackground', 'watchface_background', DEFAULTS.watchfaceBackground),
    boxBackgroundColor: first(raw, 'boxBackgroundColor', 'box_background_color', DEFAULTS.boxBackgroundColor),
    boxTopBorderColor: first(raw, 'boxTopBorderColor', 'box_top_border_color', DEFAULTS.boxTopBorderColor),
    boxBottomBorderColor: first(raw, 'boxBottomBorderColor', 'box_bottom_border_color', DEFAULTS.boxBottomBorderColor),
    trayBackgroundColor: first(raw, 'trayBackgroundColor', 'tray_background_color', DEFAULTS.trayBackgroundColor),
    dividerColor: first(raw, 'dividerColor', 'divider_color', DEFAULTS.dividerColor),
    timeTextColor: first(raw, 'timeTextColor', 'time_text_color', DEFAULTS.timeTextColor),
    dateTextColor: first(raw, 'dateTextColor', 'date_text_color', DEFAULTS.dateTextColor),
    slot1TextColor: first(raw, 'slot1TextColor', 'slot_1_text_color', DEFAULTS.slot1TextColor),
    slot2TextColor: first(raw, 'slot2TextColor', 'slot_2_text_color', DEFAULTS.slot2TextColor),
    slot3TextColor: first(raw, 'slot3TextColor', 'slot_3_text_color', DEFAULTS.slot3TextColor),
    batteryIndicatorColor: first(raw, 'batteryIndicatorColor', 'battery_indicator_color', DEFAULTS.batteryIndicatorColor)
  };
}

function loadSettings() {
  try { return normalize(JSON.parse(localStorage.getItem(SETTINGS_KEY) || '{}')); }
  catch(error) { return normalize(DEFAULTS); }
}

function saveSettings(settings) {
  settings = normalize(settings);
  localStorage.setItem(SETTINGS_KEY, JSON.stringify(settings));
  return settings;
}

function enumValue(value, values, fallback) {
  return values[value] === undefined ? fallback : values[value];
}

function colorValue(value, fallback) {
  var color = /^#[0-9a-f]{6}$/i.test(String(value)) ? String(value) : fallback;
  return parseInt(color.substring(1), 16);
}

function weatherUnit(settings) {
  return settings.celsius ? 'celsius' : 'fahrenheit';
}

function weatherLocationKey(settings) {
  settings = normalize(settings);
  if(!settings.manualLocation) return 'device';
  return [settings.manualPostalCode, settings.manualCity, settings.manualCountry].join('|').toLowerCase();
}

function weatherCacheId(settings) {
  return weatherLocationKey(settings) + ':' + weatherUnit(settings);
}

function hydrateWeather(settings) {
  weatherState.temp = null;
  weatherState.offset = null;
  try {
    if(localStorage.getItem(WEATHER_CACHE_ID) !== weatherCacheId(settings)) return;
    var temp = parseInt(localStorage.getItem(WEATHER_TEMP_STORAGE), 10);
    if(!isNaN(temp)) weatherState.temp = temp;
    if(settings.manualLocation) {
      var offset = parseInt(localStorage.getItem(WEATHER_OFFSET_STORAGE), 10);
      if(!isNaN(offset) && offset >= -50400 && offset <= 50400) weatherState.offset = offset;
    }
  } catch(error) {
    console.log('Redux weather cache load failed: ' + error.message);
  }
}

function saveWeather(settings, temp, offset) {
  weatherState.temp = Math.round(temp);
  weatherState.offset = settings.manualLocation && typeof offset === 'number' ? Math.max(-50400, Math.min(50400, Math.round(offset))) : null;
  try {
    localStorage.setItem(WEATHER_CACHE_ID, weatherCacheId(settings));
    localStorage.setItem(WEATHER_TEMP_STORAGE, String(weatherState.temp));
    if(weatherState.offset !== null) localStorage.setItem(WEATHER_OFFSET_STORAGE, String(weatherState.offset));
    else localStorage.removeItem(WEATHER_OFFSET_STORAGE);
  } catch(error) {
    console.log('Redux weather cache save failed: ' + error.message);
  }
}

function buildPayload(settings) {
  settings = normalize(settings);
  var layouts = {horizontal_2: 0, horizontal_3: 1, vertical_3: 2, vertical_2: 3};
  var metrics = {calendar: 0, weather: 1, battery: 2, calories: 3, activity: 4, sleep: 5, heart: 6, steps: 7, distance: 8, none: 9};
  var themes = {orange: 0, blue: 1, purple: 2, yellow: 3, green: 4, red: 5, brown: 7, white: 8, black: 9};
  var languages = {en:0,es:1,fr:2,de:3,pt:4,it:5,nl:6,da:7,nb:8,sv:9,fi:10,is:11,ca:12,eu:13,gl:14,pl:15,cs:16,sk:17,sl:18,hr:19,bs:20,'sr-Latn':21,ro:22,hu:23,et:24,lv:25,lt:26,tr:27,sq:28,mt:29};
  var payload = {
    0: enumValue(settings.layout, layouts, 0),
    1: enumValue(settings.slot1Metric, metrics, 0),
    2: enumValue(settings.slot2Metric, metrics, 1),
    3: enumValue(settings.slot3Metric, metrics, 9),
    4: settings.themeMode === 'custom' ? 1 : 0,
    5: enumValue(settings.theme, themes, 0),
    6: settings.showBatteryIndicator ? 1 : 0,
    7: colorValue(settings.watchfaceBackground, DEFAULTS.watchfaceBackground),
    8: colorValue(settings.boxBackgroundColor, DEFAULTS.boxBackgroundColor),
    9: colorValue(settings.boxTopBorderColor, DEFAULTS.boxTopBorderColor),
    10: colorValue(settings.boxBottomBorderColor, DEFAULTS.boxBottomBorderColor),
    11: colorValue(settings.trayBackgroundColor, DEFAULTS.trayBackgroundColor),
    12: colorValue(settings.dividerColor, DEFAULTS.dividerColor),
    13: colorValue(settings.timeTextColor, DEFAULTS.timeTextColor),
    14: colorValue(settings.dateTextColor, DEFAULTS.dateTextColor),
    15: colorValue(settings.slot1TextColor, DEFAULTS.slot1TextColor),
    16: colorValue(settings.slot2TextColor, DEFAULTS.slot2TextColor),
    17: colorValue(settings.slot3TextColor, DEFAULTS.slot3TextColor),
    18: colorValue(settings.batteryIndicatorColor, DEFAULTS.batteryIndicatorColor),
    19: 1,
    20: settings.showLeadingZero ? 1 : 0,
    21: settings.hour24 ? 1 : 0,
    22: settings.celsius ? 1 : 0,
    23: settings.showBluetooth ? 1 : 0,
    24: enumValue(settings.language, languages, 0),
    25: settings.analyticsEnabled ? 1 : 0,
    28: settings.manualLocation && typeof weatherState.offset === 'number' ? 1 : 0
  };
  if(typeof weatherState.temp === 'number') payload[WEATHER_TEMP_KEY] = weatherState.temp;
  if(settings.manualLocation && typeof weatherState.offset === 'number') payload[LOCATION_TIME_OFFSET_KEY] = weatherState.offset;
  return payload;
}

function sendSettings(settings) {
  settings = normalize(settings);
  var payload = buildPayload(settings);
  console.log('Redux sending settings: ' + JSON.stringify(settings));
  console.log('Redux language payload: language=' + payload[24]);
  console.log('Redux weather payload: temp=' + payload[WEATHER_TEMP_KEY] + ' offset=' + payload[LOCATION_TIME_OFFSET_KEY] + ' useLocationTime=' + payload[USE_LOCATION_TIME_KEY]);
  Pebble.sendAppMessage(payload, function() {
    console.log('Redux settings sent');
  }, function(error) {
    console.log('Redux settings send failed: ' + JSON.stringify(error));
  });
}

function requestJson(url, success, failure) {
  var request = new XMLHttpRequest();
  request.open('GET', url, true);
  request.onload = function() {
    if(request.status < 200 || request.status >= 300) { failure(); return; }
    try { success(JSON.parse(request.responseText)); }
    catch(error) { failure(); }
  };
  request.onerror = failure;
  request.ontimeout = failure;
  request.timeout = 15000;
  request.send();
}

function fetchCoordinates(settings, latitude, longitude, source) {
  var url = 'https://api.open-meteo.com/v1/forecast?latitude=' + encodeURIComponent(latitude) +
    '&longitude=' + encodeURIComponent(longitude) + '&current_weather=true&temperature_unit=' + weatherUnit(settings) + '&timezone=auto';
  console.log('Redux weather request (' + source + '): ' + url);
  requestJson(url, function(data) {
    if(!data || !data.current_weather || typeof data.current_weather.temperature !== 'number') return;
    saveWeather(settings, data.current_weather.temperature, typeof data.utc_offset_seconds === 'number' ? data.utc_offset_seconds : null);
    sendSettings(settings);
  }, function() {
    console.log('Redux weather request failed: ' + source);
  });
}

function postalFallback(settings) {
  var query = clean(settings.manualPostalCode).toUpperCase().replace(/ /g, '');
  var country = settings.manualCountry.toLowerCase();
  if(country !== 'us' && country !== 'ca') return;
  if(country === 'us') query = query.split('-')[0];
  if(country === 'ca') query = query.substring(0, 3);
  if(!query) return;
  requestJson('https://api.zippopotam.us/' + country + '/' + encodeURIComponent(query), function(data) {
    if(!data || !data.places || !data.places.length) return;
    fetchCoordinates(settings, data.places[0].latitude, data.places[0].longitude, 'manual-postal');
  }, function() {
    console.log('Redux postal fallback failed');
  });
}

function fetchManualWeather(settings) {
  var query = settings.manualPostalCode || settings.manualCity;
  if(!query) return;
  var url = 'https://geocoding-api.open-meteo.com/v1/search?name=' + encodeURIComponent(query) + '&count=1&language=en&format=json';
  if(settings.manualCountry) url += '&countryCode=' + encodeURIComponent(settings.manualCountry);
  requestJson(url, function(data) {
    if(data && data.results && data.results.length) {
      fetchCoordinates(settings, data.results[0].latitude, data.results[0].longitude, 'manual-geocode');
      return;
    }
    if(settings.manualPostalCode) postalFallback(settings);
  }, function() {
    if(settings.manualPostalCode) postalFallback(settings);
  });
}

function fetchWeather(settings) {
  settings = normalize(settings);
  if(settings.manualLocation) {
    fetchManualWeather(settings);
    return;
  }
  if(!navigator.geolocation) {
    fetchCoordinates(settings, FALLBACK_LATITUDE, FALLBACK_LONGITUDE, 'fallback-no-geolocation');
    return;
  }
  navigator.geolocation.getCurrentPosition(function(position) {
    fetchCoordinates(settings, position.coords.latitude, position.coords.longitude, 'device-geolocation');
  }, function() {
    fetchCoordinates(settings, FALLBACK_LATITUDE, FALLBACK_LONGITUDE, 'fallback-geolocation');
  }, {timeout:15000, maximumAge:600000});
}

function requestWeather(settings, reason, force) {
  var now = Date.now();
  if(!force && now - lastWeatherRequestAt < WEATHER_REFRESH_DEBOUNCE_MS) return;
  lastWeatherRequestAt = now;
  console.log('Redux weather refresh requested: ' + reason);
  fetchWeather(settings);
}

function parseResponse(response) {
  if(!response || response === 'CANCELLED') return null;
  var value = String(response).replace(/^pebblejs:\/\/close#/, '').replace(/^#/, '');
  try { return normalize(JSON.parse(value)); }
  catch(error) { return normalize(JSON.parse(decodeURIComponent(value))); }
}

Pebble.addEventListener('ready', function() {
  var settings = loadSettings();
  hydrateWeather(settings);
  console.log('Redux companion ready');
  sendSettings(settings);
  requestWeather(settings, 'companion-ready', true);
});

Pebble.addEventListener('appmessage', function(event) {
  var payload = event && event.payload ? event.payload : {};
  if(payload[REQUEST_WEATHER_KEY] === 1 || payload.request_weather === 1) {
    requestWeather(loadSettings(), 'watch-refresh', false);
  }
});

Pebble.addEventListener('showConfiguration', function() {
  var settings = encodeURIComponent(JSON.stringify(loadSettings()));
  var url = CONFIG_URL + '?v=' + encodeURIComponent(CONFIG_VERSION) + '&settings=' + settings;
  console.log('Redux opening config: ' + url);
  Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(event) {
  try {
    var settings = parseResponse(event && event.response);
    if(!settings) return;
    settings = saveSettings(settings);
    hydrateWeather(settings);
    sendSettings(settings);
    requestWeather(settings, 'settings-saved', true);
  } catch(error) {
    console.log('Redux config response ignored: ' + error.message);
  }
});