#include <pebble.h>

#include "battery_indicator.h"
#include "layout_horizontal.h"
#include "redux_settings.h"

// Static Figma-reference stress build for horizontal three-slot pixel QA.

#define COLOR_TRAY (g_redux_settings.theme_mode ? redux_color(g_redux_settings.watchface_background) : redux_preset_color())
#define COLOR_DIVIDER (g_redux_settings.theme_mode ? redux_color(g_redux_settings.box_top_border) : GColorBlack)
#define COLOR_TIME_PANEL (g_redux_settings.theme_mode ? redux_color(g_redux_settings.box_background) : GColorWhite)
#define COLOR_TEXT (g_redux_settings.theme_mode ? redux_color(g_redux_settings.slot_text[0]) : redux_contrast_color(COLOR_TRAY))

static Layer *s_background_layer;
static Layer *s_calendar_icon_layer;
static BitmapLayer *s_steps_icon_layer;
static BitmapLayer *s_battery_icon_layer;
static BatteryIndicator *s_battery_indicator;

static TextLayer *s_calendar_value_layer;
static TextLayer *s_steps_value_layer;
static TextLayer *s_battery_value_layer;
static TextLayer *s_time_layer;

static GBitmap *s_calendar_bitmap;
static GBitmap *s_steps_bitmap;
static GBitmap *s_battery_bitmap;

static GFont s_font_18;
static GFont s_font_label;
static GFont s_font_62;
static char s_time_buffer[8];

static void update_time(struct tm *tick_time) {
  if(g_redux_settings.hour24) strftime(s_time_buffer, sizeof(s_time_buffer), "%H:%M", tick_time);
  else strftime(s_time_buffer, sizeof(s_time_buffer), g_redux_settings.show_leading_zero ? "%I:%M" : "%l:%M", tick_time);
  text_layer_set_text(s_time_layer, s_time_buffer);
}
static void tick_handler(struct tm *tick_time, TimeUnits units) { update_time(tick_time); }

static void background_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, COLOR_TRAY);
  graphics_fill_rect(
    ctx,
    GRect(0, 0, HORIZONTAL_CANVAS_W, HORIZONTAL_CANVAS_H),
    0,
    GCornerNone
  );

  graphics_context_set_fill_color(ctx, COLOR_DIVIDER);
  graphics_fill_rect(
    ctx,
    GRect(
      HORIZONTAL_TOP_DIVIDER_X,
      HORIZONTAL_TOP_DIVIDER_Y,
      HORIZONTAL_TOP_DIVIDER_W,
      HORIZONTAL_TOP_DIVIDER_H
    ),
    0,
    GCornerNone
  );
  graphics_fill_rect(
    ctx,
    GRect(
      HORIZONTAL_BOTTOM_DIVIDER_X,
      HORIZONTAL_BOTTOM_DIVIDER_Y,
      HORIZONTAL_BOTTOM_DIVIDER_W,
      HORIZONTAL_BOTTOM_DIVIDER_H
    ),
    0,
    GCornerNone
  );

  graphics_context_set_fill_color(ctx, COLOR_TIME_PANEL);
  graphics_fill_rect(
    ctx,
    GRect(
      HORIZONTAL_TIME_PANEL_X,
      HORIZONTAL_TIME_PANEL_Y,
      HORIZONTAL_TIME_PANEL_W,
      HORIZONTAL_TIME_PANEL_H
    ),
    0,
    GCornerNone
  );
}

static void calendar_icon_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  graphics_draw_bitmap_in_rect(ctx, s_calendar_bitmap, layer_get_bounds(layer));

  graphics_context_set_text_color(ctx, COLOR_TEXT);
  graphics_draw_text(
    ctx,
    "30",
    s_font_18,
    GRect(
      HORIZONTAL_CALENDAR_DAY_X,
      HORIZONTAL_CALENDAR_DAY_Y,
      HORIZONTAL_CALENDAR_DAY_W,
      HORIZONTAL_CALENDAR_DAY_H
    ),
    GTextOverflowModeFill,
    GTextAlignmentCenter,
    NULL
  );
}

static BitmapLayer *create_icon_layer(
  Layer *parent,
  int16_t x,
  int16_t y,
  GBitmap *bitmap
) {
  BitmapLayer *layer = bitmap_layer_create(
    GRect(x, y, HORIZONTAL_ICON_W, HORIZONTAL_ICON_H)
  );
  bitmap_layer_set_bitmap(layer, bitmap);
  bitmap_layer_set_background_color(layer, GColorClear);
  bitmap_layer_set_compositing_mode(layer, GCompOpSet);
  bitmap_layer_set_alignment(layer, GAlignCenter);
  layer_add_child(parent, bitmap_layer_get_layer(layer));
  return layer;
}

static TextLayer *create_text_layer(
  Layer *parent,
  GRect frame,
  GFont font,
  GTextAlignment alignment,
  const char *text
) {
  TextLayer *layer = text_layer_create(frame);
  text_layer_set_background_color(layer, GColorClear);
  text_layer_set_text_color(layer, COLOR_TEXT);
  text_layer_set_font(layer, font);
  text_layer_set_text_alignment(layer, alignment);
  text_layer_set_overflow_mode(layer, GTextOverflowModeFill);
  text_layer_set_text(layer, text);
  layer_add_child(parent, text_layer_get_layer(layer));
  return layer;
}

static TextLayer *create_slot_value_layer(
  Layer *parent,
  int16_t x,
  const char *text
) {
  return create_text_layer(
    parent,
    GRect(x, HORIZONTAL_LABEL_Y, HORIZONTAL_LABEL_W, HORIZONTAL_LABEL_H),
    s_font_label,
    GTextAlignmentCenter,
    text
  );
}

void horizontal_layout_load(Window *window) {
  Layer *root_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(root_layer);

  APP_LOG(
    APP_LOG_LEVEL_INFO,
    "Horizontal QA canvas: %d x %d",
    bounds.size.w,
    bounds.size.h
  );

  s_font_18 = fonts_load_custom_font(
    resource_get_handle(RESOURCE_ID_FONT_ROBOTO_FLEX_EXTRABOLD_18)
  );
  s_font_label = fonts_load_custom_font(
    resource_get_handle(RESOURCE_ID_FONT_ROBOTO_FLEX_BOLD_20)
  );
  s_font_62 = fonts_load_custom_font(
    resource_get_handle(RESOURCE_ID_FONT_ROBOTO_FLEX_EXTRABOLD_62)
  );

  s_calendar_bitmap = gbitmap_create_with_resource(
    RESOURCE_ID_IMAGE_CALENDAR_54X46
  );
  s_steps_bitmap = gbitmap_create_with_resource(
    RESOURCE_ID_IMAGE_STEPS_54X46
  );
  s_battery_bitmap = gbitmap_create_with_resource(
    RESOURCE_ID_IMAGE_BATTERY_100_54X46
  );

  s_background_layer = layer_create(
    GRect(0, 0, HORIZONTAL_CANVAS_W, HORIZONTAL_CANVAS_H)
  );
  layer_set_update_proc(s_background_layer, background_update_proc);
  layer_add_child(root_layer, s_background_layer);

  s_calendar_icon_layer = layer_create(
    GRect(
      HORIZONTAL_SLOT_1_X,
      HORIZONTAL_CALENDAR_ICON_Y,
      HORIZONTAL_ICON_W,
      HORIZONTAL_ICON_H
    )
  );
  layer_set_update_proc(s_calendar_icon_layer, calendar_icon_update_proc);
  layer_add_child(root_layer, s_calendar_icon_layer);
  s_calendar_value_layer = create_slot_value_layer(
    root_layer,
    HORIZONTAL_SLOT_1_X,
    "Mer"
  );

  s_steps_icon_layer = create_icon_layer(
    root_layer,
    HORIZONTAL_SLOT_2_X,
    HORIZONTAL_STEPS_ICON_Y,
    s_steps_bitmap
  );
  s_steps_value_layer = create_slot_value_layer(
    root_layer,
    HORIZONTAL_SLOT_2_X,
    "99.9K"
  );

  s_battery_icon_layer = create_icon_layer(
    root_layer,
    HORIZONTAL_SLOT_3_X,
    HORIZONTAL_BATTERY_ICON_Y,
    s_battery_bitmap
  );
  s_battery_value_layer = create_slot_value_layer(
    root_layer,
    HORIZONTAL_SLOT_3_X,
    "100%"
  );

  s_time_layer = create_text_layer(
    root_layer,
    GRect(
      HORIZONTAL_TIME_X,
      HORIZONTAL_TIME_Y,
      HORIZONTAL_TIME_W,
      HORIZONTAL_TIME_H
    ),
    s_font_62,
    GTextAlignmentCenter,
    "23:59"
  );
  text_layer_set_text_color(s_time_layer, g_redux_settings.theme_mode ? redux_color(g_redux_settings.time_text) : GColorBlack);
  time_t now = time(NULL); update_time(localtime(&now)); tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  s_battery_indicator = g_redux_settings.show_battery_indicator ? battery_indicator_create(
    GRect(
      HORIZONTAL_BOTTOM_STRIP_X,
      HORIZONTAL_BOTTOM_STRIP_Y,
      HORIZONTAL_BOTTOM_STRIP_W,
      HORIZONTAL_BOTTOM_STRIP_H
    )
  ) : NULL;
  if (s_battery_indicator) {
    battery_indicator_set_normal_color(s_battery_indicator, redux_color(g_redux_settings.battery_indicator));
    layer_add_child(
      root_layer,
      battery_indicator_get_layer(s_battery_indicator)
    );
  }
}

void horizontal_layout_unload(Window *window) {
  tick_timer_service_unsubscribe();
  battery_indicator_destroy(s_battery_indicator);

  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_battery_value_layer);
  text_layer_destroy(s_steps_value_layer);
  text_layer_destroy(s_calendar_value_layer);

  bitmap_layer_destroy(s_battery_icon_layer);
  bitmap_layer_destroy(s_steps_icon_layer);
  layer_destroy(s_calendar_icon_layer);
  layer_destroy(s_background_layer);

  gbitmap_destroy(s_battery_bitmap);
  gbitmap_destroy(s_steps_bitmap);
  gbitmap_destroy(s_calendar_bitmap);

  fonts_unload_custom_font(s_font_62);
  fonts_unload_custom_font(s_font_label);
  fonts_unload_custom_font(s_font_18);
}
