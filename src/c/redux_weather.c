#include "redux_weather.h"

#define PERSIST_WEATHER_TEMP 220
#define PERSIST_WEATHER_AVAILABLE 221
#define PERSIST_LOCATION_OFFSET 222
#define PERSIST_USE_LOCATION_TIME 223

#define LOCATION_OFFSET_MIN_SECONDS -50400
#define LOCATION_OFFSET_MAX_SECONDS 50400
#define WEATHER_REFRESH_MS (15 * 60 * 1000)

static bool s_temperature_available;
static int s_temperature;
static bool s_use_location_time;
static int32_t s_location_offset_seconds;
static AppTimer *s_refresh_timer;

static int32_t clamp_offset(int32_t value) {
  if(value < LOCATION_OFFSET_MIN_SECONDS) return LOCATION_OFFSET_MIN_SECONDS;
  if(value > LOCATION_OFFSET_MAX_SECONDS) return LOCATION_OFFSET_MAX_SECONDS;
  return value;
}

static void request_weather(void) {
  DictionaryIterator *outbox = NULL;
  AppMessageResult result = app_message_outbox_begin(&outbox);
  if(result != APP_MSG_OK || !outbox) return;

  dict_write_uint8(outbox, REDUX_REQUEST_WEATHER_KEY, 1);
  app_message_outbox_send();
}

static void refresh_timer_handler(void *context) {
  s_refresh_timer = NULL;
  request_weather();
  s_refresh_timer = app_timer_register(WEATHER_REFRESH_MS, refresh_timer_handler, NULL);
}

void redux_weather_init(void) {
  s_temperature_available = persist_exists(PERSIST_WEATHER_AVAILABLE) && persist_read_bool(PERSIST_WEATHER_AVAILABLE);
  if(s_temperature_available && persist_exists(PERSIST_WEATHER_TEMP)) {
    s_temperature = persist_read_int(PERSIST_WEATHER_TEMP);
  }

  if(persist_exists(PERSIST_LOCATION_OFFSET)) {
    s_location_offset_seconds = clamp_offset(persist_read_int(PERSIST_LOCATION_OFFSET));
  }
  if(persist_exists(PERSIST_USE_LOCATION_TIME)) {
    s_use_location_time = persist_read_bool(PERSIST_USE_LOCATION_TIME);
  }

  if(s_refresh_timer) app_timer_cancel(s_refresh_timer);
  s_refresh_timer = app_timer_register(WEATHER_REFRESH_MS, refresh_timer_handler, NULL);
}

void redux_weather_deinit(void) {
  if(s_refresh_timer) {
    app_timer_cancel(s_refresh_timer);
    s_refresh_timer = NULL;
  }
}

void redux_weather_handle_message(DictionaryIterator *iterator) {
  Tuple *temperature = dict_find(iterator, REDUX_WEATHER_TEMP_KEY);
  if(temperature) {
    s_temperature = temperature->value->int32;
    s_temperature_available = true;
    persist_write_int(PERSIST_WEATHER_TEMP, s_temperature);
    persist_write_bool(PERSIST_WEATHER_AVAILABLE, true);
  }

  Tuple *offset = dict_find(iterator, REDUX_LOCATION_TIME_OFFSET_KEY);
  if(offset) {
    s_location_offset_seconds = clamp_offset(offset->value->int32);
    persist_write_int(PERSIST_LOCATION_OFFSET, s_location_offset_seconds);
  }

  Tuple *use_location_time = dict_find(iterator, REDUX_USE_LOCATION_TIME_KEY);
  if(use_location_time) {
    s_use_location_time = use_location_time->value->int32 != 0;
    persist_write_bool(PERSIST_USE_LOCATION_TIME, s_use_location_time);
  }
}

bool redux_weather_temperature_available(void) {
  return s_temperature_available;
}

int redux_weather_temperature(void) {
  return s_temperature;
}

struct tm *redux_weather_display_time(time_t now) {
  if(s_use_location_time) {
    time_t location_now = now + (time_t)s_location_offset_seconds;
    return gmtime(&location_now);
  }
  return localtime(&now);
}
