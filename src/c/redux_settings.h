#pragma once
#include <pebble.h>

typedef struct {
  uint8_t theme_mode;
  uint8_t theme;
  bool show_battery_indicator;
  bool show_leading_zero;
  bool hour24;
  bool celsius;
  bool show_bluetooth;
  uint8_t language;
  uint32_t watchface_background;
  uint32_t box_background;
  uint32_t box_top_border;
  uint32_t box_bottom_border;
  uint32_t tray_background;
  uint32_t divider;
  uint32_t time_text;
  uint32_t date_text;
  uint32_t slot_text[3];
  uint32_t battery_indicator;
} ReduxSettings;

extern ReduxSettings g_redux_settings;
void redux_settings_set_defaults(void);
GColor redux_color(uint32_t value);
GColor redux_preset_color(void);
GColor redux_preset_divider_color(void);
GColor redux_preset_time_panel_color(void);
GColor redux_preset_time_text_color(void);
GColor redux_preset_slot_text_color(void);
GColor redux_preset_battery_color(void);
bool redux_preset_uses_reverse_icons(void);
GColor redux_contrast_color(GColor color);
