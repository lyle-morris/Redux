#include "redux_settings.h"

ReduxSettings g_redux_settings;

enum {
  ReduxThemeOrange = 0,
  ReduxThemeBlue = 1,
  ReduxThemePurple = 2,
  ReduxThemeYellow = 3,
  ReduxThemeGreen = 4,
  ReduxThemeRed = 5,
  ReduxThemePink = 6,
  ReduxThemeBrown = 7,
  ReduxThemeWhite = 8,
  ReduxThemeBlack = 9,
};

static uint8_t valid_theme(void) {
  return g_redux_settings.theme < 10 ? g_redux_settings.theme : ReduxThemeOrange;
}

void redux_settings_set_defaults(void) {
  g_redux_settings = (ReduxSettings) {
    .theme_mode = 0, .theme = ReduxThemeOrange, .show_battery_indicator = true,
    .show_leading_zero = true, .hour24 = false, .celsius = false,
    .show_bluetooth = false, .language = 0,
    .watchface_background = 0x000000, .box_background = 0xffffff,
    .box_top_border = 0x000000, .box_bottom_border = 0x000000,
    .tray_background = 0xff5500, .divider = 0x000000,
    .time_text = 0x000000, .date_text = 0x000000,
    .slot_text = {0x000000, 0x000000, 0x000000},
    .battery_indicator = 0xff5500
  };
}

GColor redux_color(uint32_t value) { return GColorFromHEX(value & 0xffffff); }

GColor redux_preset_color(void) {
  static const uint32_t colors[] = {
    0xff5500, // Orange
    0x00aaff, // Blue
    0x5500ff, // Purple
    0xffaa00, // Yellow
    0x55aa00, // Green
    0xff0000, // Red
    0xff0055, // Pink
    0xaa5500, // Brown
    0xffffff, // White
    0x000000  // Black
  };
  return redux_color(colors[valid_theme()]);
}

GColor redux_preset_divider_color(void) {
  return valid_theme() == ReduxThemeBlack ? GColorWhite : GColorBlack;
}

GColor redux_preset_time_panel_color(void) {
  return valid_theme() == ReduxThemeBlack ? GColorBlack : GColorWhite;
}

GColor redux_preset_time_text_color(void) {
  return valid_theme() == ReduxThemeBlack ? GColorWhite : GColorBlack;
}

GColor redux_preset_slot_text_color(void) {
  uint8_t theme = valid_theme();
  return (theme == ReduxThemePurple || theme == ReduxThemeBlack)
    ? GColorWhite
    : GColorBlack;
}

GColor redux_preset_battery_color(void) {
  return valid_theme() == ReduxThemeBlack ? GColorWhite : redux_preset_color();
}

bool redux_preset_uses_reverse_icons(void) {
  uint8_t theme = valid_theme();
  return theme == ReduxThemePurple || theme == ReduxThemeBlack;
}

GColor redux_contrast_color(GColor color) {
  return (color.argb == GColorWhite.argb || color.argb == GColorYellow.argb || color.argb == GColorChromeYellow.argb || color.argb == GColorCyan.argb) ? GColorBlack : GColorWhite;
}
