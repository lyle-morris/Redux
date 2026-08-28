#pragma once

#include <pebble.h>

#define REDUX_WEATHER_TEMP_KEY 26
#define REDUX_LOCATION_TIME_OFFSET_KEY 27
#define REDUX_USE_LOCATION_TIME_KEY 28
#define REDUX_REQUEST_WEATHER_KEY 29

void redux_weather_init(void);
void redux_weather_deinit(void);
void redux_weather_handle_message(DictionaryIterator *iterator);

bool redux_weather_temperature_available(void);
int redux_weather_temperature(void);
struct tm *redux_weather_display_time(time_t now);
