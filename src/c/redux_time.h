#pragma once

#include <pebble.h>
#include <stddef.h>
#include <string.h>
#include "redux_weather.h"

static inline void redux_strip_leading_zero(char *buffer) {
  if(buffer && buffer[0] == '0') {
    memmove(buffer, buffer + 1, strlen(buffer));
  }
}

static inline struct tm *redux_clock_display_time(struct tm *tick_time) {
  time_t now = time(NULL);
  struct tm *display_time = redux_weather_display_time(now);
  return display_time ? display_time : tick_time;
}

static inline void redux_format_clock_time(struct tm *tick_time, char *buffer, size_t size) {
  if(!tick_time || !buffer || size == 0) return;
  struct tm *display_time = redux_clock_display_time(tick_time);
  strftime(buffer, size, g_redux_settings.hour24 ? "%H:%M" : "%I:%M", display_time);
  if(!g_redux_settings.show_leading_zero) redux_strip_leading_zero(buffer);
}

static inline void redux_format_clock_hour(struct tm *tick_time, char *buffer, size_t size) {
  if(!tick_time || !buffer || size == 0) return;
  struct tm *display_time = redux_clock_display_time(tick_time);
  strftime(buffer, size, g_redux_settings.hour24 ? "%H" : "%I", display_time);
  if(!g_redux_settings.show_leading_zero) redux_strip_leading_zero(buffer);
}
