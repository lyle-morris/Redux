#include <pebble.h>
#include <stdio.h>

#include "battery_assets.h"
#include "battery_indicator.h"
#include "layout_horizontal.h"
#include "redux_settings.h"

#define COLOR_TRAY (g_redux_settings.theme_mode ? redux_color(g_redux_settings.watchface_background) : redux_preset_color())
#define COLOR_TOP_DIVIDER (g_redux_settings.theme_mode ? redux_color(g_redux_settings.box_top_border) : redux_preset_divider_color())
#define COLOR_BOTTOM_DIVIDER (g_redux_settings.theme_mode ? redux_color(g_redux_settings.box_bottom_border) : redux_preset_divider_color())
#define COLOR_TIME_PANEL (g_redux_settings.theme_mode ? redux_color(g_redux_settings.box_background) : redux_preset_time_panel_color())
#define COLOR_TIME_TEXT (g_redux_settings.theme_mode ? redux_color(g_redux_settings.time_text) : redux_preset_time_text_color())
#define COLOR_BATTERY (g_redux_settings.theme_mode ? redux_color(g_redux_settings.battery_indicator) : redux_preset_battery_color())

static Layer *s_background_layer;
static BitmapLayer *s_icon_layers[3];
static TextLayer *s_value_layers[3];
static TextLayer *s_calendar_day_layers[3];
static GBitmap *s_bitmaps[3];
static uint32_t s_icon_resource_ids[3];
static BatteryIndicator *s_battery_indicator;
static TextLayer *s_time_layer;
static bool s_battery_service_subscribed;

static GFont s_font_21;
static GFont s_font_label;
static GFont s_font_62;
static char s_time_buffer[8];
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
  return g_redux_settings.slot_metric[0] == ReduxMetricBattery ||
         g_redux_settings.slot_metric[1] == ReduxMetricBattery ||
         g_redux_settings.slot_metric[2] == ReduxMetricBattery;
}

static void battery_state_handler(BatteryChargeState state) {
  if (s_battery_indicator) {
    battery_indicator_set_percentage(s_battery_indicator, state.charge_percent);
    battery_indicator_set_charging(s_battery_indicator, state.is_charging);
  }

  for (uint8_t i = 0; i < 3; ++i) {
    if (g_redux_settings.slot_metric[i] != ReduxMetricBattery) continue;

    redux_battery_format_label(state, s_value_buffers[i], sizeof(s_value_buffers[i]));
    if (s_value_layers[i]) text_layer_set_text(s_value_layers[i], s_value_buffers[i]);
    replace_slot_bitmap(i, redux_battery_resource_54(state));
  }
}

static void update_slot_values(struct tm *tick_time) {
  for(uint8_t i = 0; i < 3; ++i) {
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
  graphics_fill_rect(ctx, GRect(0, 0, HORIZONTAL_CANVAS_W, HORIZONTAL_CANVAS_H), 0, GCornerNone);
  graphics_context_set_fill_color(ctx, COLOR_TOP_DIVIDER);
  graphics_fill_rect(ctx, GRect(HORIZONTAL_TOP_DIVIDER_X, HORIZONTAL_TOP_DIVIDER_Y, HORIZONTAL_TOP_DIVIDER_W, HORIZONTAL_TOP_DIVIDER_H), 0, GCornerNone);
  graphics_context_set_fill_color(ctx, COLOR_BOTTOM_DIVIDER);
  graphics_fill_rect(ctx, GRect(HORIZONTAL_BOTTOM_DIVIDER_X, HORIZONTAL_BOTTOM_DIVIDER_Y, HORIZONTAL_BOTTOM_DIVIDER_W, HORIZONTAL_BOTTOM_DIVIDER_H), 0, GCornerNone);
  graphics_context_set_fill_color(ctx, COLOR_TIME_PANEL);
  graphics_fill_rect(ctx, GRect(HORIZONTAL_TIME_PANEL_X, HORIZONTAL_TIME_PANEL_Y, HORIZONTAL_TIME_PANEL_W, HORIZONTAL_TIME_PANEL_H), 0, GCornerNone);
}

static void create_metric_slot(Layer *root_layer, uint8_t index, int16_t x) {
  uint8_t metric = g_redux_settings.slot_metric[index];
  uint32_t resource_id = metric_resource(metric);
  GColor color = slot_color(index);

  if(resource_id) {
    s_bitmaps[index] = gbitmap_create_with_resource(resource_id);
    s_icon_resource_ids[index] = resource_id;
    s_icon_layers[index] = bitmap_layer_create(GRect(x, HORIZONTAL_CALENDAR_ICON_Y, HORIZONTAL_ICON_W, HORIZONTAL_ICON_H));
    bitmap_layer_set_bitmap(s_icon_layers[index], s_bitmaps[index]);
    bitmap_layer_set_background_color(s_icon_layers[index], GColorClear);
    bitmap_layer_set_compositing_mode(s_icon_layers[index], GCompOpSet);
    bitmap_layer_set_alignment(s_icon_layers[index], GAlignCenter);
    layer_add_child(root_layer, bitmap_layer_get_layer(s_icon_layers[index]));
  }

  s_value_layers[index] = create_text_layer(
    root_layer,
    GRect(x, HORIZONTAL_LABEL_Y, HORIZONTAL_LABEL_W, HORIZONTAL_LABEL_H),
    s_font_label,
    GTextAlignmentCenter,
    "",
    color
  );

  if(metric == ReduxMetricCalendar) {
    s_calendar_day_layers[index] = create_text_layer(
      root_layer,
      GRect(x + HORIZONTAL_CALENDAR_DAY_X, HORIZONTAL_CALENDAR_ICON_Y + HORIZONTAL_CALENDAR_DAY_Y, HORIZONTAL_CALENDAR_DAY_W, HORIZONTAL_CALENDAR_DAY_H),
      s_font_21,
      GTextAlignmentCenter,
      "",
      color
    );
  }
}

void horizontal_layout_load(Window *window) {
  Layer *root_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(root_layer);
  APP_LOG(APP_LOG_LEVEL_INFO, "Horizontal 3 QA canvas: %d x %d (geometry r%d) slots=%d/%d/%d", bounds.size.w, bounds.size.h, HORIZONTAL_LAYOUT_LOCK_REVISION, (int)g_redux_settings.slot_metric[0], (int)g_redux_settings.slot_metric[1], (int)g_redux_settings.slot_metric[2]);

  s_battery_service_subscribed = false;
  for(uint8_t i = 0; i < 3; ++i) {
    s_icon_layers[i] = NULL; s_value_layers[i] = NULL; s_calendar_day_layers[i] = NULL; s_bitmaps[i] = NULL; s_icon_resource_ids[i] = 0;
  }

  s_font_21 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_FLEX_EXTRABOLD_21));
  s_font_label = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_FLEX_BOLD_20));
  s_font_62 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_FLEX_EXTRABOLD_62));

  s_background_layer = layer_create(GRect(0, 0, HORIZONTAL_CANVAS_W, HORIZONTAL_CANVAS_H));
  layer_set_update_proc(s_background_layer, background_update_proc);
  layer_add_child(root_layer, s_background_layer);

  create_metric_slot(root_layer, 0, HORIZONTAL_SLOT_1_X);
  create_metric_slot(root_layer, 1, HORIZONTAL_SLOT_2_X);
  create_metric_slot(root_layer, 2, HORIZONTAL_SLOT_3_X);

  s_time_layer = create_text_layer(root_layer, GRect(HORIZONTAL_TIME_X, HORIZONTAL_TIME_Y, HORIZONTAL_TIME_W, HORIZONTAL_TIME_H), s_font_62, GTextAlignmentCenter, "23:59", COLOR_TIME_TEXT);
  time_t now = time(NULL); update_time(localtime(&now));
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  s_battery_indicator = g_redux_settings.show_battery_indicator ? battery_indicator_create(GRect(HORIZONTAL_BOTTOM_STRIP_X, HORIZONTAL_BOTTOM_STRIP_Y, HORIZONTAL_BOTTOM_STRIP_W, HORIZONTAL_BOTTOM_STRIP_H)) : NULL;
  if(s_battery_indicator) {
    battery_indicator_set_normal_color(s_battery_indicator, COLOR_BATTERY);
    layer_add_child(root_layer, battery_indicator_get_layer(s_battery_indicator));
  }

  if (s_battery_indicator || has_battery_slot()) {
    battery_state_service_subscribe(battery_state_handler);
    s_battery_service_subscribed = true;
    battery_state_handler(battery_state_service_peek());
  }
}

void horizontal_layout_unload(Window *window) {
  tick_timer_service_unsubscribe();
  if (s_battery_service_subscribed) {
    battery_state_service_unsubscribe();
    s_battery_service_subscribed = false;
  }
  battery_indicator_destroy(s_battery_indicator);
  s_battery_indicator = NULL;
  text_layer_destroy(s_time_layer);

  for(uint8_t i = 0; i < 3; ++i) {
    if(s_calendar_day_layers[i]) text_layer_destroy(s_calendar_day_layers[i]);
    if(s_value_layers[i]) text_layer_destroy(s_value_layers[i]);
    if(s_icon_layers[i]) bitmap_layer_destroy(s_icon_layers[i]);
    if(s_bitmaps[i]) gbitmap_destroy(s_bitmaps[i]);
  }
  layer_destroy(s_background_layer);
  fonts_unload_custom_font(s_font_62);
  fonts_unload_custom_font(s_font_label);
  fonts_unload_custom_font(s_font_21);
}
