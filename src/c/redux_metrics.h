#pragma once

#include <pebble.h>
#include <stddef.h>
#include <stdio.h>
#include "battery_assets.h"
#include "redux_settings.h"

#define REDUX_METRIC_UNAVAILABLE "---"

static inline void redux_format_compact_count(HealthValue value, char *buffer, size_t size) {
  long count = (long)value;
  if(count < 0) {
    snprintf(buffer, size, "%s", REDUX_METRIC_UNAVAILABLE);
    return;
  }
  if(count > 999000) count = 999000;
  if(count >= 10000) snprintf(buffer, size, "%ldK", count / 1000);
  else snprintf(buffer, size, "%ld", count);
}

static inline void redux_format_duration_seconds(HealthValue seconds, char *buffer, size_t size) {
  if(seconds < 0) {
    snprintf(buffer, size, "%s", REDUX_METRIC_UNAVAILABLE);
    return;
  }
  long total_minutes = (long)seconds / 60;
  if(total_minutes > 5999) total_minutes = 5999;
  snprintf(buffer, size, "%02ld:%02ld", total_minutes / 60, total_minutes % 60);
}

static inline void redux_format_distance_meters(HealthValue value, char *buffer, size_t size) {
  long meters = (long)value;
  if(meters < 0) {
    snprintf(buffer, size, "%s", REDUX_METRIC_UNAVAILABLE);
    return;
  }
  if(meters < 1000) {
    snprintf(buffer, size, "%ldm", meters);
    return;
  }
  if(meters < 100000) {
    long tenths_km = (meters + 50) / 100;
    long whole_km = tenths_km / 10;
    long decimal_km = tenths_km % 10;
    if(decimal_km == 0) snprintf(buffer, size, "%ldK", whole_km);
    else snprintf(buffer, size, "%ld.%ldK", whole_km, decimal_km);
    return;
  }
  long km = (meters + 500) / 1000;
  if(km > 999) km = 999;
  snprintf(buffer, size, "%ldK", km);
}

static inline void redux_metric_value(uint8_t metric, struct tm *tick_time, char *buffer, size_t size) {
  switch(metric) {
    case ReduxMetricCalendar:
      redux_format_calendar_label(tick_time, buffer, size);
      break;
    case ReduxMetricWeather:
      snprintf(buffer, size, "%s", g_redux_settings.celsius ? "22°" : "72°");
      break;
    case ReduxMetricBattery:
      redux_battery_format_label(battery_state_service_peek(), buffer, size);
      break;
    case ReduxMetricCalories: {
      HealthValue calories = health_service_sum_today(HealthMetricActiveKCalories);
      redux_format_compact_count(calories, buffer, size);
      break;
    }
    case ReduxMetricActivity: {
      HealthValue active_seconds = health_service_sum_today(HealthMetricActiveSeconds);
      redux_format_duration_seconds(active_seconds, buffer, size);
      break;
    }
    case ReduxMetricSleep: {
      HealthValue sleep_seconds = health_service_sum_today(HealthMetricSleepSeconds);
      redux_format_duration_seconds(sleep_seconds, buffer, size);
      break;
    }
    case ReduxMetricHeart: {
      time_t now = time(NULL);
      HealthServiceAccessibilityMask accessibility = health_service_metric_accessible(HealthMetricHeartRateBPM, now, now);
      HealthValue bpm = 0;
      if(accessibility & HealthServiceAccessibilityMaskAvailable) {
        bpm = health_service_peek_current_value(HealthMetricHeartRateBPM);
      }
      if(bpm > 0) snprintf(buffer, size, "%ld", (long)bpm);
      else snprintf(buffer, size, "%s", REDUX_METRIC_UNAVAILABLE);
      break;
    }
    case ReduxMetricSteps: {
      HealthValue steps = health_service_sum_today(HealthMetricStepCount);
      redux_format_compact_count(steps, buffer, size);
      break;
    }
    case ReduxMetricDistance: {
      HealthValue distance = health_service_sum_today(HealthMetricWalkedDistanceMeters);
      redux_format_distance_meters(distance, buffer, size);
      break;
    }
    default:
      buffer[0] = '\0';
      break;
  }
}

static inline bool redux_metric_uses_health(uint8_t metric) {
  return metric == ReduxMetricCalories || metric == ReduxMetricActivity ||
         metric == ReduxMetricSleep || metric == ReduxMetricHeart ||
         metric == ReduxMetricSteps || metric == ReduxMetricDistance;
}
