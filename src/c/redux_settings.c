#include "redux_settings.h"

ReduxSettings g_redux_settings;

void redux_settings_set_defaults(void) {
  g_redux_settings = (ReduxSettings) {
    .theme_mode = 0, .theme = 1, .show_battery_indicator = true,
    .show_leading_zero = true, .hour24 = false, .celsius = false,
    .show_bluetooth = false, .language = 0,
    .watchface_background = 0x000000, .box_background = 0xffffff,
    .box_top_border = 0x000000, .box_bottom_border = 0x000000,
    .tray_background = 0x00aaff, .divider = 0xffffff,
    .time_text = 0xffffff, .date_text = 0xffffff,
    .slot_text = {0x000000, 0x000000, 0x000000},
    .battery_indicator = 0x00aaff
  };
}

GColor redux_color(uint32_t value) { return GColorFromHEX(value & 0xffffff); }

GColor redux_preset_color(void) {
  static const uint32_t colors[] = {0xff5500,0x00aaff,0x5500ff,0xffaa00,0x55aa00,0xff0000,0xff0055,0xaa5500,0xffffff,0x000000};
  return redux_color(colors[g_redux_settings.theme < 10 ? g_redux_settings.theme : 1]);
}

GColor redux_contrast_color(GColor color) {
  return (color.argb == GColorWhite.argb || color.argb == GColorYellow.argb || color.argb == GColorChromeYellow.argb || color.argb == GColorCyan.argb) ? GColorBlack : GColorWhite;
}
