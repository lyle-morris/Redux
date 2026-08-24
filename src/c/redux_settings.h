#pragma once
#include <pebble.h>

// QA isolation: keep preset reverse-icon code paths compiling while the
// reverse PNG resources are reintroduced one at a time. These aliases point
// to the already-proven white assets and can be removed once the offending
// CloudPebble resource is identified.
#define RESOURCE_ID_IMAGE_REVERSE_CALENDAR_54X46 RESOURCE_ID_IMAGE_CALENDAR_54X46
#define RESOURCE_ID_IMAGE_REVERSE_BATTERY_100_54X46 RESOURCE_ID_IMAGE_BATTERY_100_54X46
#define RESOURCE_ID_IMAGE_REVERSE_STEPS_54X46 RESOURCE_ID_IMAGE_STEPS_54X46
#define RESOURCE_ID_IMAGE_REVERSE_CALENDAR_64X64 RESOURCE_ID_IMAGE_CALENDAR_64X64
#define RESOURCE_ID_IMAGE_REVERSE_WEATHER_PARTLY_CLOUDY_64X64 RESOURCE_ID_IMAGE_WEATHER_PARTLY_CLOUDY_64X64

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
