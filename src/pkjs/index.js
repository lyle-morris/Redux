var CONFIG_URL = 'https://lyle-morris.github.io/Hosting/apps/redux/qa/app-config.html';
var CONFIG_VERSION = 'redux-qa-v2';
var SETTINGS_KEY = 'redux_qa_settings_v1';

var DEFAULTS = {
  verticalLayout: true,
  layout: 'vertical_3',
  slot1Metric: 'calendar',
  slot2Metric: 'weather',
  slot3Metric: 'steps',
  themeMode: 'preset',
  theme: 'blue',
  showBatteryIndicator: true,
  watchfaceBackground: '#000000',
  boxBackgroundColor: '#ffffff',
  boxTopBorderColor: '#000000',
  boxBottomBorderColor: '#000000',
  trayBackgroundColor: '#00aaff',
  dividerColor: '#ffffff',
  timeTextColor: '#ffffff',
  dateTextColor: '#ffffff',
  slot1TextColor: '#000000',
  slot2TextColor: '#000000',
  slot3TextColor: '#000000',
  batteryIndicatorColor: '#00aaff'
};

function first(settings, camel, snake, fallback) {
  if(settings[camel] !== undefined) return settings[camel];
  if(settings[snake] !== undefined) return settings[snake];
  return fallback;
}

function normalize(raw) {
  raw = raw || {};
  var vertical = !!first(raw, 'verticalLayout', 'vertical_layout', DEFAULTS.verticalLayout);
  var slot3 = first(raw, 'slot3Metric', 'slot_3_metric', DEFAULTS.slot3Metric);
  var layout = first(raw, 'layout', 'layout', vertical ? 'vertical_3' : (slot3 === 'none' ? 'horizontal_2' : 'horizontal_3'));
  if(layout !== 'vertical_3' && layout !== 'horizontal_3' && layout !== 'horizontal_2') layout = DEFAULTS.layout;

  return {
    verticalLayout: layout === 'vertical_3',
    layout: layout,
    slot1Metric: first(raw, 'slot1Metric', 'slot_1_metric', DEFAULTS.slot1Metric),
    slot2Metric: first(raw, 'slot2Metric', 'slot_2_metric', DEFAULTS.slot2Metric),
    slot3Metric: slot3,
    themeMode: first(raw, 'themeMode', 'theme_mode', DEFAULTS.themeMode),
    theme: first(raw, 'theme', 'theme', DEFAULTS.theme),
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

function buildPayload(settings) {
  settings = normalize(settings);
  var layouts = {horizontal_2: 0, horizontal_3: 1, vertical_3: 2};
  var metrics = {calendar: 0, weather: 1, battery: 2, calories: 3, activity: 4, sleep: 5, heart: 6, steps: 7, distance: 8, none: 9};
  var themes = {orange: 0, blue: 1, purple: 2, yellow: 3, green: 4, red: 5, pink: 6, brown: 7, white: 8, black: 9};
  return {
    0: enumValue(settings.layout, layouts, 2),
    1: enumValue(settings.slot1Metric, metrics, 0),
    2: enumValue(settings.slot2Metric, metrics, 1),
    3: enumValue(settings.slot3Metric, metrics, 7),
    4: settings.themeMode === 'custom' ? 1 : 0,
    5: enumValue(settings.theme, themes, 1),
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
    19: 1
  };
}

function sendSettings(settings) {
  settings = normalize(settings);
  console.log('Redux sending settings: ' + JSON.stringify(settings));
  Pebble.sendAppMessage(buildPayload(settings), function() {
    console.log('Redux settings sent');
  }, function(error) {
    console.log('Redux settings send failed: ' + JSON.stringify(error));
  });
}

function parseResponse(response) {
  if(!response || response === 'CANCELLED') return null;
  var value = String(response).replace(/^pebblejs:\/\/close#/, '').replace(/^#/, '');
  try { return normalize(JSON.parse(value)); }
  catch(error) { return normalize(JSON.parse(decodeURIComponent(value))); }
}

Pebble.addEventListener('ready', function() {
  console.log('Redux companion ready');
  sendSettings(loadSettings());
});

Pebble.addEventListener('showConfiguration', function() {
  var settings = encodeURIComponent(JSON.stringify(loadSettings()));
  Pebble.openURL(CONFIG_URL + '?v=' + encodeURIComponent(CONFIG_VERSION) + '&settings=' + settings);
});

Pebble.addEventListener('webviewclosed', function(event) {
  try {
    var settings = parseResponse(event && event.response);
    if(!settings) return;
    sendSettings(saveSettings(settings));
  } catch(error) {
    console.log('Redux config response ignored: ' + error.message);
  }
});
