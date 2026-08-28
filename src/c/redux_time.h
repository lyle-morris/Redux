#pragma once

#include <pebble.h>
#include <stddef.h>
#include <string.h>

static inline void redux_strip_leading_zero(char *buffer) {
  if(buffer && buffer[0] == '0') {
    memmove(buffer, buffer + 1, strlen(buffer));
  }
}

static inline void redux_format_clock_time(struct tm *tick_time, char *buffer, size_t size) {
  if(!tick_time || !buffer || size == 0) return;
  strftime(buffer, size, g_redux_settings.hour24 ? "%H:%M" : "%I:%M", tick_time);
  if(!g_redux_settings.show_leading_zero) redux_strip_leading_zero(buffer);
}

static inline void redux_format_clock_hour(struct tm *tick_time, char *buffer, size_t size) {
  if(!tick_time || !buffer || size == 0) return;
  strftime(buffer, size, g_redux_settings.hour24 ? "%H" : "%I", tick_time);
  if(!g_redux_settings.show_leading_zero) redux_strip_leading_zero(buffer);
}
