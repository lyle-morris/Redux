#include "redux_heart_rate.h"

#define REDUX_HEART_SAMPLE_PERIOD_SECONDS 10
#define REDUX_HEART_SAMPLE_TIMEOUT_MS 120000

static bool s_sampling_attempted;
static bool s_sampling_requested;
static AppTimer *s_sampling_timer;

static void sampling_stop(void);

static void sampling_timeout(void *context) {
  s_sampling_timer = NULL;
  sampling_stop();
}

static void sampling_start(void) {
  if(s_sampling_attempted || s_sampling_requested) return;

  s_sampling_attempted = true;
  if(!health_service_set_heart_rate_sample_period(REDUX_HEART_SAMPLE_PERIOD_SECONDS)) {
    APP_LOG(APP_LOG_LEVEL_WARNING, "Redux could not request temporary heart rate sampling");
    return;
  }

  s_sampling_requested = true;
  s_sampling_timer = app_timer_register(
    REDUX_HEART_SAMPLE_TIMEOUT_MS,
    sampling_timeout,
    NULL
  );
  APP_LOG(APP_LOG_LEVEL_INFO, "Redux temporary heart rate sampling requested");
}

static void sampling_stop(void) {
  if(s_sampling_timer) {
    app_timer_cancel(s_sampling_timer);
    s_sampling_timer = NULL;
  }

  if(!s_sampling_requested) return;

  if(health_service_set_heart_rate_sample_period(0)) {
    s_sampling_requested = false;
    APP_LOG(APP_LOG_LEVEL_INFO, "Redux heart rate sampling returned to automatic");
  } else {
    APP_LOG(APP_LOG_LEVEL_WARNING, "Redux could not reset heart rate sampling");
  }
}

void redux_heart_rate_sampling_reset(void) {
  sampling_stop();
  s_sampling_attempted = false;
}

HealthValue redux_current_heart_rate(void) {
  time_t now = time(NULL);
  HealthServiceAccessibilityMask accessibility = health_service_metric_accessible(HealthMetricHeartRateBPM, now, now);
  HealthValue bpm = 0;

  if(accessibility & HealthServiceAccessibilityMaskAvailable) {
    bpm = health_service_peek_current_value(HealthMetricHeartRateBPM);
  }

  if(bpm > 0) {
    sampling_stop();
    return bpm > 999 ? 999 : bpm;
  }

  sampling_start();
  return 0;
}
