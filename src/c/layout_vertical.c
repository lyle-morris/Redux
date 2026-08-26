#include <pebble.h>
#include <stdio.h>

#include "battery_assets.h"
#include "layout_vertical.h"
#include "redux_settings.h"

#define COLOR_TRAY (g_redux_settings.theme_mode ? redux_color(g_redux_settings.tray_background) : redux_preset_color())
#define COLOR_DIVIDER (g_redux_settings.theme_mode ? redux_color(g_redux_settings.divider) : redux_preset_divider_color())
#define COLOR_TIME_PANEL (g_redux_settings.theme_mode ? redux_color(g_redux_settings.watchface_background) : redux_preset_time_panel_color())
#define COLOR_TIME_TEXT (g_redux_settings.theme_mode ? redux_color(g_redux_settings.time_text) : redux_preset_time_text_color())
#define COLOR_DATE_TEXT (g_redux_settings.theme_mode ? redux_color(g_redux_settings.date_text) : redux_preset_time_text_color())

static Layer *s_background_layer;
static BitmapLayer *s_icon_layers[3];
static TextLayer *s_value_layers[3];
static TextLayer *s_calendar_day_layers[3];
static GBitmap *s_bitmaps[3];
static uint32_t s_icon_resource_ids[3];
static TextLayer *s_hour_layer;
static TextLayer *s_minute_layer;
static TextLayer *s_date_layer;
static bool s_battery_service_subscribed;

static GFont s_font_18;
static GFont s_font_21;
static GFont s_font_label;
static GFont s_font_88;
static uint8_t s_slot_count;
static char s_hour_buffer[4];
static char s_minute_buffer[4];
static char s_date_buffer[24];
static char s_value_buffers[3][12];
static char s_day_buffers[3][4];

static GColor slot_color(uint8_t index) {
  return g_redux_settings.theme_mode
    ? redux_color(g_redux_settings.slot_text[index])
    : redux_preset_slot_text_color();
}

static uint32_t metric_resource(uint8_t metric) {
  switch(metric) {
    case ReduxMetricCalendar: return RESOURCE_ID_IMAGE_CALENDAR_54X46;
    case ReduxMetricWeather: return RESOURCE_ID_IMAGE_WEATHER_PARTLY_CLOUDY_54X46;
    case ReduxMetricBattery: return redux_battery_resource_54(battery_state_service_peek());
    case ReduxMetricCalories: return RESOURCE_ID_IMAGE_CALORIES_54X46;
    case ReduxMetricActivity: return RESOURCE_ID_IMAGE_ACTIVITY_TIME_54X46;
    case ReduxMetricSleep: return RESOURCE_ID_IMAGE_SLEEP_SCORE_54X46;
    case ReduxMetricHeart: return RESOURCE_ID_IMAGE_HEART_RATE_54X46;
    case ReduxMetricSteps: return RESOURCE_ID_IMAGE_STEPS_54X46;
    case ReduxMetricDistance: return RESOURCE_ID_IMAGE_DISTANCE_54X46;
    default: return 0;
  }
}

static void metric_value(uint8_t metric, struct tm *tick_time, char *buffer, size_t size) {
  switch(metric) {
    case ReduxMetricCalendar: strftime(buffer, size, "%b", tick_time); break;
    case ReduxMetricWeather: snprintf(buffer, size, "%s", g_redux_settings.celsius ? "22°" : "72°"); break;
    case ReduxMetricBattery: {
      redux_battery_format_label(battery_state_service_peek(), buffer, size);
      break;
    }
    case ReduxMetricCalories: snprintf(buffer, size, "9999"); break;
    case ReduxMetricActivity: snprintf(buffer, size, "99m"); break;
    case ReduxMetricSleep: snprintf(buffer, size, "99"); break;
    case ReduxMetricHeart: snprintf(buffer, size, "999"); break;
    case ReduxMetricSteps: snprintf(buffer, size, "99.9K"); break;
    case ReduxMetricDistance: snprintf(buffer, size, "99.9K"); break;
    default: buffer[0] = '\0'; break;
  }
}

static TextLayer *create_text_layer(Layer *parent, GRect frame, GFont font, GTextAlignment alignment, const char *text, GColor color) {
  TextLayer *layer = text_layer_create(frame);
  text_layer_set_background_color(layer, GColorClear);
  text_layer_set_text_color(layer, color);
  text_layer_set_font(layer, font);
  text_layer_set_text_alignment(layer, alignment);
  text_layer_set_overflow_mode(layer, GTextOverflowModeFill);
  text_layer_set_text(layer, text);
  layer_add_child(parent, text_layer_get_layer(layer));
  return layer;
}

static void replace_slot_bitmap(uint8_t index, uint32_t resource_id) {
  if (!s_icon_layers[index] || !resource_id || s_icon_resource_ids[index] == resource_id) return;

  GBitmap *bitmap = gbitmap_create_with_resource(resource_id);
  if (!bitmap) return;

  bitmap_layer_set_bitmap(s_icon_layers[index], bitmap);
  if (s_bitmaps[index]) gbitmap_destroy(s_bitmaps[index]);
  s_bitmaps[index] = bitmap;
  s_icon_resource_ids[index] = resource_id;
}

static bool has_battery_slot(void) {
  for (uint8_t i = 0; i < s_slot_count; ++i) {
    if (g_redux_settings.slot_metric[i] == ReduxMetricBattery) return true;
  }
  return false;
}

static void battery_state_handler(BatteryChargeState state) {
  for (uint8_t i = 0; i < s_slot_count; ++i) {
    if (g_redux_settings.slot_metric[i] != ReduxMetricBattery) continue;

    redux_battery_format_label(state, s_value_buffers[i], sizeof(s_value_buffers[i]));
    if (s_value_layers[i]) text_layer_set_text(s_value_layers[i], s_value_buffers[i]);
    replace_slot_bitmap(i, redux_battery_resource_54(state));
  }
}

static void update_slot_values(struct tm *tick_time) {
  for(uint8_t i = 0; i < s_slot_count; ++i) {
    uint8_t metric = g_redux_settings.slot_metric[i];
    metric_value(metric, tick_time, s_value_buffers[i], sizeof(s_value_buffers[i]));
    if(s_value_layers[i]) text_layer_set_text(s_value_layers[i], s_value_buffers[i]);
    if(s_calendar_day_layers[i]) {
      snprintf(s_day_buffers[i], sizeof(s_day_buffers[i]), "%d", tick_time->tm_mday);
      text_layer_set_text(s_calendar_day_layers[i], s_day_buffers[i]);
    }
  }
}

static void update_time(struct tm *tick_time) {
  if(g_redux_settings.hour24) {
    strftime(s_hour_buffer, sizeof(s_hour_buffer), "%H", tick_time);
  } else {
    strftime(s_hour_buffer, sizeof(s_hour_buffer), g_redux_settings.show_leading_zero ? "%I" : "%l", tick_time);
  }
  strftime(s_minute_buffer, sizeof(s_minute_buffer), "%M", tick_time);
  strftime(s_date_buffer, sizeof(s_date_buffer), "%b %e %a", tick_time);
  text_layer_set_text(s_hour_layer, s_hour_buffer);
  text_layer_set_text(s_minute_layer, s_minute_buffer);
  text_layer_set_text(s_date_layer, s_date_buffer);
  update_slot_values(tick_time);
}

static void tick_handler(struct tm *tick_time, TimeUnits units) { update_time(tick_time); }

static void background_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, COLOR_TRAY);
  graphics_fill_rect(ctx, GRect(VERTICAL_TRAY_X, VERTICAL_TRAY_Y, VERTICAL_TRAY_W, VERTICAL_TRAY_H), 0, GCornerNone);
  graphics_context_set_fill_color(ctx, COLOR_DIVIDER);
  graphics_fill_rect(ctx, GRect(VERTICAL_DIVIDER_X, VERTICAL_DIVIDER_Y, VERTICAL_DIVIDER_W, VERTICAL_DIVIDER_H), 0, GCornerNone);
  graphics_context_set_fill_color(ctx, COLOR_TIME_PANEL);
  graphics_fill_rect(ctx, GRect(VERTICAL_TIME_PANEL_X, VERTICAL_TIME_PANEL_Y, VERTICAL_TIME_PANEL_W, VERTICAL_TIME_PANEL_H), 0, GCornerNone);
}

static int16_t slot_icon_y(uint8_t index) {
  if(s_slot_count == 2) return index == 0 ? VERTICAL_TWO_SLOT_1_ICON_Y : VERTICAL_TWO_SLOT_2_ICON_Y;
  if(index == 0) return VERTICAL_THREE_SLOT_1_ICON_Y;
  if(index == 1) return VERTICAL_THREE_SLOT_2_ICON_Y;
  return VERTICAL_THREE_SLOT_3_ICON_Y;
}

static int16_t slot_label_y(uint8_t index) {
  if(s_slot_count == 2) return index == 0 ? VERTICAL_TWO_SLOT_1_LABEL_Y : VERTICAL_TWO_SLOT_2_LABEL_Y;
  if(index == 0) return VERTICAL_THREE_SLOT_1_LABEL_Y;
  if(index == 1) return VERTICAL_THREE_SLOT_2_LABEL_Y;
  return VERTICAL_THREE_SLOT_3_LABEL_Y;
}

static void create_metric_slot(Layer *root_layer, uint8_t index) {
  uint8_t metric = g_redux_settings.slot_metric[index];
  uint32_t resource_id = metric_resource(metric);
  int16_t icon_y = slot_icon_y(index);
  int16_t label_y = slot_label_y(index);
  GColor color = slot_color(index);

  if(resource_id) {
    s_bitmaps[index] = gbitmap_create_with_resource(resource_id);
    s_icon_resource_ids[index] = resource_id;
    s_icon_layers[index] = bitmap_layer_create(GRect(VERTICAL_ICON_X, icon_y, VERTICAL_ICON_W, VERTICAL_ICON_H));
    bitmap_layer_set_bitmap(s_icon_layers[index], s_bitmaps[index]);
    bitmap_layer_set_background_color(s_icon_layers[index], GColorClear);
    bitmap_layer_set_compositing_mode(s_icon_layers[index], GCompOpSet);
    bitmap_layer_set_alignment(s_icon_layers[index], GAlignCenter);
    layer_add_child(root_layer, bitmap_layer_get_layer(s_icon_layers[index]));
  }

  s_value_layers[index] = create_text_layer(
    root_layer,
    GRect(VERTICAL_LABEL_X, label_y, VERTICAL_LABEL_W, VERTICAL_LABEL_H),
    s_font_label,
    GTextAlignmentCenter,
    "",
    color
  );

  if(metric == ReduxMetricCalendar) {
    s_calendar_day_layers[index] = create_text_layer(
      root_layer,
      GRect(VERTICAL_ICON_X + VERTICAL_CALENDAR_DAY_X, icon_y + VERTICAL_CALENDAR_DAY_Y, VERTICAL_CALENDAR_DAY_W, VERTICAL_CALENDAR_DAY_H),
      s_font_21,
      GTextAlignmentCenter,
      "",
      color
    );
  }
}

void vertical_layout_load(Window *window) {
  Layer *root_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(root_layer);
  s_slot_count = g_redux_settings.slot_metric[2] == ReduxMetricNone ? 2 : 3;
  APP_LOG(APP_LOG_LEVEL_INFO, "Vertical %d QA canvas: %d x %d (geometry r%d) slots=%d/%d/%d", (int)s_slot_count, bounds.size.w, bounds.size.h, VERTICAL_LAYOUT_LOCK_REVISION, (int)g_redux_settings.slot_metric[0], (int)g_redux_settings.slot_metric[1], (int)g_redux_settings.slot_metric[2]);

  s_battery_service_subscribed = false;
  for(uint8_t i = 0; i < 3; ++i) {
    s_icon_layers[i] = NULL; s_value_layers[i] = NULL; s_calendar_day_layers[i] = NULL; s_bitmaps[i] = NULL; s_icon_resource_ids[i] = 0;
  }

  s_font_18 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_FLEX_EXTRABOLD_18));
  s_font_21 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_FLEX_EXTRABOLD_21));
  s_font_label = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_FLEX_BOLD_20));
  s_font_88 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_FLEX_EXTRABOLD_88));

  s_background_layer = layer_create(GRect(0, 0, VERTICAL_CANVAS_W, VERTICAL_CANVAS_H));
  layer_set_update_proc(s_background_layer, background_update_proc);
  layer_add_child(root_layer, s_background_layer);

  for(uint8_t i = 0; i < s_slot_count; ++i) create_metric_slot(root_layer, i);

  s_hour_layer = create_text_layer(root_layer, GRect(VERTICAL_TIME_CONTENT_X, VERTICAL_TIME_HOUR_Y, VERTICAL_TIME_CONTENT_W, VERTICAL_TIME_LAYER_H), s_font_88, GTextAlignmentRight, "23", COLOR_TIME_TEXT);
  s_minute_layer = create_text_layer(root_layer, GRect(VERTICAL_TIME_CONTENT_X, VERTICAL_TIME_MINUTE_Y, VERTICAL_TIME_CONTENT_W, VERTICAL_TIME_LAYER_H), s_font_88, GTextAlignmentRight, "59", COLOR_TIME_TEXT);
  s_date_layer = create_text_layer(root_layer, GRect(VERTICAL_DATE_X, VERTICAL_DATE_Y, VERTICAL_DATE_W, VERTICAL_DATE_H), s_font_18, GTextAlignmentCenter, "Aug 24 Mon", COLOR_DATE_TEXT);

  time_t now = time(NULL); update_time(localtime(&now));
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  if (has_battery_slot()) {
    battery_state_service_subscribe(battery_state_handler);
    s_battery_service_subscribed = true;
    battery_state_handler(battery_state_service_peek());
  }
}

void vertical_layout_unload(Window *window) {
  tick_timer_service_unsubscribe();
  if (s_battery_service_subscribed) {
    battery_state_service_unsubscribe();
    s_battery_service_subscribed = false;
  }
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_minute_layer);
  text_layer_destroy(s_hour_layer);

  for(uint8_t i = 0; i < 3; ++i) {
    if(s_calendar_day_layers[i]) text_layer_destroy(s_calendar_day_layers[i]);
    if(s_value_layers[i]) text_layer_destroy(s_value_layers[i]);
    if(s_icon_layers[i]) bitmap_layer_destroy(s_icon_layers[i]);
    if(s_bitmaps[i]) gbitmap_destroy(s_bitmaps[i]);
  }
  layer_destroy(s_background_layer);
  fonts_unload_custom_font(s_font_88);
  fonts_unload_custom_font(s_font_label);
  fonts_unload_custom_font(s_font_21);
  fonts_unload_custom_font(s_font_18);
}
