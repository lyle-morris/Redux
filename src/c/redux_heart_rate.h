#pragma once

#include <pebble.h>

#define REDUX_HEART_SAMPLE_PERIOD_SECONDS 10
#define REDUX_HEART_SAMPLE_TIMEOUT_MS 120000

static bool s_redux_heart_sampling_attempted;
static bool s_redux_heart_sampling_requested;
static AppTimer *s_redux_heart_sampling_timer;

static void redux_heart_rate_sampling_stop(void);

static void redux_heart_rate_sampling_timeout(void *context) {
  s_redux_heart_sampling_timer = NULL;
  redux_heart_rate_sampling_stop();
}

static void redux_heart_rate_sampling_start(void) {
  if(s_redux_heart_sampling_attempted || s_redux_heart_sampling_requested) return;

  s_redux_heart_sampling_attempted = true;
  if(!health_service_set_heart_rate_sample_period(REDUX_HEART_SAMPLE_PERIOD_SECONDS)) {
    APP_LOG(APP_LOG_LEVEL_WARNING, "Redux could not request temporary heart rate sampling");
    return;
  }

  s_redux_heart_sampling_requested = true;
  s_redux_heart_sampling_timer = app_timer_register(
    REDUX_HEART_SAMPLE_TIMEOUT_MS,
    redux_heart_rate_sampling_timeout,
    NULL
  );
  APP_LOG(APP_LOG_LEVEL_INFO, "Redux temporary heart rate sampling requested");
}

static void redux_heart_rate_sampling_stop(void) {
  if(s_redux_heart_sampling_timer) {
    app_timer_cancel(s_redux_heart_sampling_timer);
    s_redux_heart_sampling_timer = NULL;
  }

  if(!s_redux_heart_sampling_requested) return;

  if(health_service_set_heart_rate_sample_period(0)) {
    s_redux_heart_sampling_requested = false;
    APP_LOG(APP_LOG_LEVEL_INFO, "Redux heart rate sampling returned to automatic");
  } else {
    APP_LOG(APP_LOG_LEVEL_WARNING, "Redux could not reset heart rate sampling");
  }
}

static void redux_heart_rate_sampling_reset(void) {
  redux_heart_rate_sampling_stop();
  s_redux_heart_sampling_attempted = false;
}

static HealthValue redux_current_heart_rate(void) {
  time_t now = time(NULL);
  HealthServiceAccessibilityMask accessibility = health_service_metric_accessible(HealthMetricHeartRateBPM, now, now);
  HealthValue bpm = 0;
  if(accessibility & HealthServiceAccessibilityMaskAvailable) {
    bpm = health_service_peek_current_value(HealthMetricHeartRateBPM);
  }

  if(bpm > 0) {
    redux_heart_rate_sampling_stop();
    return bpm > 999 ? 999 : bpm;
  }

  redux_heart_rate_sampling_start();
  return 0;
}
