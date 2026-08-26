#include <pebble.h>
#include <stdio.h>

#include "battery_indicator.h"
#include "layout_two_slot.h"
#include "redux_settings.h"

#define COLOR_TRAY (g_redux_settings.theme_mode ? redux_color(g_redux_settings.watchface_background) : redux_preset_color())
#define COLOR_TOP_DIVIDER (g_redux_settings.theme_mode ? redux_color(g_redux_settings.box_top_border) : redux_preset_divider_color())
#define COLOR_BOTTOM_DIVIDER (g_redux_settings.theme_mode ? redux_color(g_redux_settings.box_bottom_border) : redux_preset_divider_color())
#define COLOR_TIME_PANEL (g_redux_settings.theme_mode ? redux_color(g_redux_settings.box_background) : redux_preset_time_panel_color())
#define COLOR_TIME_TEXT (g_redux_settings.theme_mode ? redux_color(g_redux_settings.time_text) : redux_preset_time_text_color())
#define COLOR_BATTERY (g_redux_settings.theme_mode ? redux_color(g_redux_settings.battery_indicator) : redux_preset_battery_color())

static Layer *s_background_layer;
static BitmapLayer *s_icon_layers[2];
static TextLayer *s_value_layers[2];
static TextLayer *s_calendar_day_layers[2];
static GBitmap *s_bitmaps[2];
static BatteryIndicator *s_battery_indicator;
static TextLayer *s_time_layer;

static GFont s_font_24;
static GFont s_font_28;
static GFont s_font_62;
static char s_time_buffer[8];
static char s_value_buffers[2][12];
static char s_day_buffers[2][4];

static GColor slot_color(uint8_t index) {
  return g_redux_settings.theme_mode
    ? redux_color(g_redux_settings.slot_text[index])
    : redux_preset_slot_text_color();
}

static uint32_t metric_resource(uint8_t metric) {
  switch(metric) {
    case ReduxMetricCalendar: return RESOURCE_ID_IMAGE_CALENDAR_64X64;
    case ReduxMetricWeather: return RESOURCE_ID_IMAGE_WEATHER_PARTLY_CLOUDY_64X64;
    case ReduxMetricBattery: return RESOURCE_ID_IMAGE_BATTERY_100_64X64;
    case ReduxMetricCalories: return RESOURCE_ID_IMAGE_CALORIES_64X64;
    case ReduxMetricActivity: return RESOURCE_ID_IMAGE_ACTIVITY_TIME_64X64;
    case ReduxMetricSleep: return RESOURCE_ID_IMAGE_SLEEP_SCORE_64X64;
    case ReduxMetricHeart: return RESOURCE_ID_IMAGE_HEART_RATE_64X64;
    case ReduxMetricSteps: return RESOURCE_ID_IMAGE_STEPS_64X64;
    case ReduxMetricDistance: return RESOURCE_ID_IMAGE_DISTANCE_64X64;
    default: return 0;
  }
}

static void metric_value(uint8_t metric, struct tm *tick_time, char *buffer, size_t size) {
  switch(metric) {
    case ReduxMetricCalendar: strftime(buffer, size, "%b", tick_time); break;
    case ReduxMetricWeather: snprintf(buffer, size, "%s", g_redux_settings.celsius ? "22°" : "72°"); break;
    case ReduxMetricBattery: snprintf(buffer, size, "100%%"); break;
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

static void update_slot_values(struct tm *tick_time) {
  for(uint8_t i = 0; i < 2; ++i) {
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
  if(g_redux_settings.hour24) strftime(s_time_buffer, sizeof(s_time_buffer), "%H:%M", tick_time);
  else strftime(s_time_buffer, sizeof(s_time_buffer), g_redux_settings.show_leading_zero ? "%I:%M" : "%l:%M", tick_time);
  text_layer_set_text(s_time_layer, s_time_buffer);
  update_slot_values(tick_time);
}

static void tick_handler(struct tm *tick_time, TimeUnits units) { update_time(tick_time); }

static void background_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, COLOR_TRAY);
  graphics_fill_rect(ctx, GRect(0, 0, TWO_SLOT_CANVAS_W, TWO_SLOT_CANVAS_H), 0, GCornerNone);
  graphics_context_set_fill_color(ctx, COLOR_TOP_DIVIDER);
  graphics_fill_rect(ctx, GRect(TWO_SLOT_TOP_DIVIDER_X, TWO_SLOT_TOP_DIVIDER_Y, TWO_SLOT_TOP_DIVIDER_W, TWO_SLOT_TOP_DIVIDER_H), 0, GCornerNone);
  graphics_context_set_fill_color(ctx, COLOR_BOTTOM_DIVIDER);
  graphics_fill_rect(ctx, GRect(TWO_SLOT_BOTTOM_DIVIDER_X, TWO_SLOT_BOTTOM_DIVIDER_Y, TWO_SLOT_BOTTOM_DIVIDER_W, TWO_SLOT_BOTTOM_DIVIDER_H), 0, GCornerNone);
  graphics_context_set_fill_color(ctx, COLOR_TIME_PANEL);
  graphics_fill_rect(ctx, GRect(TWO_SLOT_TIME_PANEL_X, TWO_SLOT_TIME_PANEL_Y, TWO_SLOT_TIME_PANEL_W, TWO_SLOT_TIME_PANEL_H), 0, GCornerNone);
}

static void create_metric_slot(Layer *root_layer, uint8_t index, int16_t x) {
  uint8_t metric = g_redux_settings.slot_metric[index];
  uint32_t resource_id = metric_resource(metric);
  GColor color = slot_color(index);

  if(resource_id) {
    s_bitmaps[index] = gbitmap_create_with_resource(resource_id);
    s_icon_layers[index] = bitmap_layer_create(GRect(x, TWO_SLOT_ICON_Y, TWO_SLOT_ICON_W, TWO_SLOT_ICON_H));
    bitmap_layer_set_bitmap(s_icon_layers[index], s_bitmaps[index]);
    bitmap_layer_set_background_color(s_icon_layers[index], GColorClear);
    bitmap_layer_set_compositing_mode(s_icon_layers[index], GCompOpSet);
    bitmap_layer_set_alignment(s_icon_layers[index], GAlignCenter);
    layer_add_child(root_layer, bitmap_layer_get_layer(s_icon_layers[index]));
  }

  s_value_layers[index] = create_text_layer(
    root_layer,
    GRect(x, TWO_SLOT_LABEL_Y, TWO_SLOT_LABEL_W, TWO_SLOT_LABEL_H),
    s_font_24,
    GTextAlignmentCenter,
    "",
    color
  );

  if(metric == ReduxMetricCalendar) {
    s_calendar_day_layers[index] = create_text_layer(
      root_layer,
      GRect(x + TWO_SLOT_CALENDAR_DAY_X, TWO_SLOT_ICON_Y + TWO_SLOT_CALENDAR_DAY_Y, TWO_SLOT_CALENDAR_DAY_W, TWO_SLOT_CALENDAR_DAY_H),
      s_font_28,
      GTextAlignmentCenter,
      "",
      color
    );
  }
}

void two_slot_layout_load(Window *window) {
  Layer *root_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(root_layer);
  APP_LOG(APP_LOG_LEVEL_INFO, "Horizontal 2 QA canvas: %d x %d (geometry r%d) slots=%d/%d", bounds.size.w, bounds.size.h, TWO_SLOT_LAYOUT_LOCK_REVISION, (int)g_redux_settings.slot_metric[0], (int)g_redux_settings.slot_metric[1]);

  for(uint8_t i = 0; i < 2; ++i) {
    s_icon_layers[i] = NULL; s_value_layers[i] = NULL; s_calendar_day_layers[i] = NULL; s_bitmaps[i] = NULL;
  }

  s_font_24 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_FLEX_EXTRABOLD_SLOT_24));
  s_font_28 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_FLEX_EXTRABOLD_28));
  s_font_62 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_FLEX_EXTRABOLD_62));

  s_background_layer = layer_create(GRect(0, 0, TWO_SLOT_CANVAS_W, TWO_SLOT_CANVAS_H));
  layer_set_update_proc(s_background_layer, background_update_proc);
  layer_add_child(root_layer, s_background_layer);

  create_metric_slot(root_layer, 0, TWO_SLOT_1_X);
  create_metric_slot(root_layer, 1, TWO_SLOT_2_X);

  s_time_layer = create_text_layer(root_layer, GRect(TWO_SLOT_TIME_X, TWO_SLOT_TIME_Y, TWO_SLOT_TIME_W, TWO_SLOT_TIME_H), s_font_62, GTextAlignmentCenter, "23:59", COLOR_TIME_TEXT);
  time_t now = time(NULL); update_time(localtime(&now));
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  s_battery_indicator = g_redux_settings.show_battery_indicator ? battery_indicator_create(GRect(TWO_SLOT_BOTTOM_STRIP_X, TWO_SLOT_BOTTOM_STRIP_Y, TWO_SLOT_BOTTOM_STRIP_W, TWO_SLOT_BOTTOM_STRIP_H)) : NULL;
  if(s_battery_indicator) {
    battery_indicator_set_normal_color(s_battery_indicator, COLOR_BATTERY);
    layer_add_child(root_layer, battery_indicator_get_layer(s_battery_indicator));
  }
}

void two_slot_layout_unload(Window *window) {
  tick_timer_service_unsubscribe();
  battery_indicator_destroy(s_battery_indicator);
  s_battery_indicator = NULL;
  text_layer_destroy(s_time_layer);

  for(uint8_t i = 0; i < 2; ++i) {
    if(s_calendar_day_layers[i]) text_layer_destroy(s_calendar_day_layers[i]);
    if(s_value_layers[i]) text_layer_destroy(s_value_layers[i]);
    if(s_icon_layers[i]) bitmap_layer_destroy(s_icon_layers[i]);
    if(s_bitmaps[i]) gbitmap_destroy(s_bitmaps[i]);
  }
  layer_destroy(s_background_layer);
  fonts_unload_custom_font(s_font_62);
  fonts_unload_custom_font(s_font_28);
  fonts_unload_custom_font(s_font_24);
}
