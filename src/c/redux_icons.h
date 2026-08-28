#pragma once

#include <pebble.h>
#include "battery_assets.h"
#include "redux_settings.h"

static inline bool redux_use_reverse_icons(void) {
  return !g_redux_settings.theme_mode && redux_preset_uses_reverse_icons();
}

static inline uint32_t redux_battery_resource_54_variant(BatteryChargeState state, bool reverse) {
  if(!reverse) return redux_battery_resource_54(state);
  if(state.is_charging) return RESOURCE_ID_IMAGE_REVERSE_BATTERY_CHARGING_54X46;
  switch(redux_battery_icon_bucket(state.charge_percent)) {
    case 0: return RESOURCE_ID_IMAGE_REVERSE_BATTERY_0_54X46;
    case 10: return RESOURCE_ID_IMAGE_REVERSE_BATTERY_10_54X46;
    case 20: return RESOURCE_ID_IMAGE_REVERSE_BATTERY_20_54X46;
    case 30: return RESOURCE_ID_IMAGE_REVERSE_BATTERY_30_54X46;
    case 40: return RESOURCE_ID_IMAGE_REVERSE_BATTERY_40_54X46;
    case 50: return RESOURCE_ID_IMAGE_REVERSE_BATTERY_50_54X46;
    case 60: return RESOURCE_ID_IMAGE_REVERSE_BATTERY_60_54X46;
    case 70: return RESOURCE_ID_IMAGE_REVERSE_BATTERY_70_54X46;
    case 80: return RESOURCE_ID_IMAGE_REVERSE_BATTERY_80_54X46;
    default: return RESOURCE_ID_IMAGE_REVERSE_BATTERY_100_54X46;
  }
}

static inline uint32_t redux_battery_resource_64_variant(BatteryChargeState state, bool reverse) {
  if(!reverse) return redux_battery_resource_64(state);
  if(state.is_charging) return RESOURCE_ID_IMAGE_REVERSE_BATTERY_CHARGING_64X64;
  switch(redux_battery_icon_bucket(state.charge_percent)) {
    case 0: return RESOURCE_ID_IMAGE_REVERSE_BATTERY_0_64X64;
    case 10: return RESOURCE_ID_IMAGE_REVERSE_BATTERY_10_64X64;
    case 20: return RESOURCE_ID_IMAGE_REVERSE_BATTERY_20_64X64;
    case 30: return RESOURCE_ID_IMAGE_REVERSE_BATTERY_30_64X64;
    case 40: return RESOURCE_ID_IMAGE_REVERSE_BATTERY_40_64X64;
    case 50: return RESOURCE_ID_IMAGE_REVERSE_BATTERY_50_64X64;
    case 60: return RESOURCE_ID_IMAGE_REVERSE_BATTERY_60_64X64;
    case 70: return RESOURCE_ID_IMAGE_REVERSE_BATTERY_70_64X64;
    case 80: return RESOURCE_ID_IMAGE_REVERSE_BATTERY_80_64X64;
    default: return RESOURCE_ID_IMAGE_REVERSE_BATTERY_100_64X64;
  }
}

static inline uint32_t redux_metric_resource_54(uint8_t metric) {
  bool reverse = redux_use_reverse_icons();
  switch(metric) {
    case ReduxMetricCalendar: return reverse ? RESOURCE_ID_IMAGE_REVERSE_CALENDAR_54X46 : RESOURCE_ID_IMAGE_CALENDAR_54X46;
    case ReduxMetricWeather: return reverse ? RESOURCE_ID_IMAGE_REVERSE_WEATHER_PARTLY_CLOUDY_54X46 : RESOURCE_ID_IMAGE_WEATHER_PARTLY_CLOUDY_54X46;
    case ReduxMetricBattery: return redux_battery_resource_54_variant(battery_state_service_peek(), reverse);
    case ReduxMetricCalories: return reverse ? RESOURCE_ID_IMAGE_REVERSE_CALORIES_54X46 : RESOURCE_ID_IMAGE_CALORIES_54X46;
    case ReduxMetricActivity: return reverse ? RESOURCE_ID_IMAGE_REVERSE_ACTIVITY_TIME_54X46 : RESOURCE_ID_IMAGE_ACTIVITY_TIME_54X46;
    case ReduxMetricSleep: return reverse ? RESOURCE_ID_IMAGE_REVERSE_SLEEP_SCORE_54X46 : RESOURCE_ID_IMAGE_SLEEP_SCORE_54X46;
    case ReduxMetricHeart: return reverse ? RESOURCE_ID_IMAGE_REVERSE_HEART_RATE_54X46 : RESOURCE_ID_IMAGE_HEART_RATE_54X46;
    case ReduxMetricSteps: return reverse ? RESOURCE_ID_IMAGE_REVERSE_STEPS_54X46 : RESOURCE_ID_IMAGE_STEPS_54X46;
    case ReduxMetricDistance: return reverse ? RESOURCE_ID_IMAGE_REVERSE_DISTANCE_54X46 : RESOURCE_ID_IMAGE_DISTANCE_54X46;
    default: return 0;
  }
}

static inline uint32_t redux_metric_resource_64(uint8_t metric) {
  bool reverse = redux_use_reverse_icons();
  switch(metric) {
    case ReduxMetricCalendar: return reverse ? RESOURCE_ID_IMAGE_REVERSE_CALENDAR_64X64 : RESOURCE_ID_IMAGE_CALENDAR_64X64;
    case ReduxMetricWeather: return reverse ? RESOURCE_ID_IMAGE_REVERSE_WEATHER_PARTLY_CLOUDY_64X64 : RESOURCE_ID_IMAGE_WEATHER_PARTLY_CLOUDY_64X64;
    case ReduxMetricBattery: return redux_battery_resource_64_variant(battery_state_service_peek(), reverse);
    case ReduxMetricCalories: return reverse ? RESOURCE_ID_IMAGE_REVERSE_CALORIES_64X64 : RESOURCE_ID_IMAGE_CALORIES_64X64;
    case ReduxMetricActivity: return reverse ? RESOURCE_ID_IMAGE_REVERSE_ACTIVITY_TIME_64X64 : RESOURCE_ID_IMAGE_ACTIVITY_TIME_64X64;
    case ReduxMetricSleep: return reverse ? RESOURCE_ID_IMAGE_REVERSE_SLEEP_TIME_64X64 : RESOURCE_ID_IMAGE_SLEEP_SCORE_64X64;
    case ReduxMetricHeart: return reverse ? RESOURCE_ID_IMAGE_REVERSE_HEART_RATE_64X64 : RESOURCE_ID_IMAGE_HEART_RATE_64X64;
    case ReduxMetricSteps: return reverse ? RESOURCE_ID_IMAGE_REVERSE_STEPS_64X64 : RESOURCE_ID_IMAGE_STEPS_64X64;
    case ReduxMetricDistance: return reverse ? RESOURCE_ID_IMAGE_REVERSE_DISTANCE_64X64 : RESOURCE_ID_IMAGE_DISTANCE_64X64;
    default: return 0;
  }
}
