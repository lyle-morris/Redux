#pragma once
#include <pebble.h>
#include <stddef.h>

// QA isolation: keep preset reverse-icon code paths compiling while the
// reverse PNG resources are reintroduced one at a time. These aliases point
// to the already-proven white assets and can be removed once the offending
// CloudPebble resource is identified.
#define RESOURCE_ID_IMAGE_REVERSE_CALENDAR_54X46 RESOURCE_ID_IMAGE_CALENDAR_54X46
#define RESOURCE_ID_IMAGE_REVERSE_BATTERY_100_54X46 RESOURCE_ID_IMAGE_BATTERY_100_54X46
#define RESOURCE_ID_IMAGE_REVERSE_STEPS_54X46 RESOURCE_ID_IMAGE_STEPS_54X46
#define RESOURCE_ID_IMAGE_REVERSE_CALENDAR_64X64 RESOURCE_ID_IMAGE_CALENDAR_64X64
#define RESOURCE_ID_IMAGE_REVERSE_WEATHER_PARTLY_CLOUDY_64X64 RESOURCE_ID_IMAGE_WEATHER_PARTLY_CLOUDY_64X64

typedef enum {
  ReduxMetricCalendar = 0,
  ReduxMetricWeather = 1,
  ReduxMetricBattery = 2,
  ReduxMetricCalories = 3,
  ReduxMetricActivity = 4,
  ReduxMetricSleep = 5,
  ReduxMetricHeart = 6,
  ReduxMetricSteps = 7,
  ReduxMetricDistance = 8,
  ReduxMetricNone = 9,
} ReduxMetric;

typedef enum {
  ReduxLanguageEnglish = 0,
  ReduxLanguageSpanish = 1,
  ReduxLanguageFrench = 2,
  ReduxLanguageGerman = 3,
  ReduxLanguagePortuguese = 4,
  ReduxLanguageItalian = 5,
  ReduxLanguageDutch = 6,
  ReduxLanguageDanish = 7,
  ReduxLanguageNorwegianBokmal = 8,
  ReduxLanguageSwedish = 9,
  ReduxLanguageFinnish = 10,
  ReduxLanguageIcelandic = 11,
  ReduxLanguageCatalan = 12,
  ReduxLanguageBasque = 13,
  ReduxLanguageGalician = 14,
  ReduxLanguagePolish = 15,
  ReduxLanguageCzech = 16,
  ReduxLanguageSlovak = 17,
  ReduxLanguageSlovenian = 18,
  ReduxLanguageCroatian = 19,
  ReduxLanguageBosnian = 20,
  ReduxLanguageSerbianLatin = 21,
  ReduxLanguageRomanian = 22,
  ReduxLanguageHungarian = 23,
  ReduxLanguageEstonian = 24,
  ReduxLanguageLatvian = 25,
  ReduxLanguageLithuanian = 26,
  ReduxLanguageTurkish = 27,
  ReduxLanguageAlbanian = 28,
  ReduxLanguageMaltese = 29,
  ReduxLanguageCount = 30,
} ReduxLanguage;

typedef struct {
  uint8_t theme_mode;
  uint8_t theme;
  uint8_t slot_metric[3];
  bool show_battery_indicator;
  bool show_leading_zero;
  bool hour24;
  bool celsius;
  bool show_bluetooth;
  bool weekday_instead_of_month;
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
uint8_t redux_valid_metric(uint8_t metric, uint8_t fallback);
uint8_t redux_valid_language(uint8_t language);
void redux_format_calendar_label(struct tm *tick_time, char *buffer, size_t size);
void redux_format_localized_date(struct tm *tick_time, char *buffer, size_t size);
GColor redux_color(uint32_t value);
GColor redux_preset_color(void);
GColor redux_preset_divider_color(void);
GColor redux_preset_time_panel_color(void);
GColor redux_preset_time_text_color(void);
GColor redux_preset_slot_text_color(void);
GColor redux_preset_battery_color(void);
bool redux_preset_uses_reverse_icons(void);
GColor redux_contrast_color(GColor color);