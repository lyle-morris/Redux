#include <pebble.h>

#include "layout_two_slot.h"

// Static maximum-width build for horizontal two-slot pixel QA.

#define COLOR_TRAY GColorFromHEX(0x00AAFF)
#define COLOR_DIVIDER GColorBlack
#define COLOR_TIME_PANEL GColorWhite
#define COLOR_TEXT GColorBlack

static Layer *s_background_layer;
static Layer *s_calendar_icon_layer;
static BitmapLayer *s_weather_icon_layer;

static TextLayer *s_calendar_value_layer;
static TextLayer *s_weather_value_layer;
static TextLayer *s_time_layer;

static GBitmap *s_calendar_bitmap;
static GBitmap *s_weather_bitmap;

static GFont s_font_20;
static GFont s_font_28;
static GFont s_font_62;

static void background_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, COLOR_TRAY);
  graphics_fill_rect(
    ctx,
    GRect(0, 0, TWO_SLOT_CANVAS_W, TWO_SLOT_CANVAS_H),
    0,
    GCornerNone
  );

  graphics_context_set_fill_color(ctx, COLOR_DIVIDER);
  graphics_fill_rect(
    ctx,
    GRect(
      TWO_SLOT_TOP_DIVIDER_X,
      TWO_SLOT_TOP_DIVIDER_Y,
      TWO_SLOT_TOP_DIVIDER_W,
      TWO_SLOT_TOP_DIVIDER_H
    ),
    0,
    GCornerNone
  );
  graphics_fill_rect(
    ctx,
    GRect(
      TWO_SLOT_BOTTOM_DIVIDER_X,
      TWO_SLOT_BOTTOM_DIVIDER_Y,
      TWO_SLOT_BOTTOM_DIVIDER_W,
      TWO_SLOT_BOTTOM_DIVIDER_H
    ),
    0,
    GCornerNone
  );

  graphics_context_set_fill_color(ctx, COLOR_TIME_PANEL);
  graphics_fill_rect(
    ctx,
    GRect(
      TWO_SLOT_TIME_PANEL_X,
      TWO_SLOT_TIME_PANEL_Y,
      TWO_SLOT_TIME_PANEL_W,
      TWO_SLOT_TIME_PANEL_H
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
    s_font_28,
    GRect(
      TWO_SLOT_CALENDAR_DAY_X,
      TWO_SLOT_CALENDAR_DAY_Y,
      TWO_SLOT_CALENDAR_DAY_W,
      TWO_SLOT_CALENDAR_DAY_H
    ),
    GTextOverflowModeFill,
    GTextAlignmentCenter,
    NULL
  );
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
    GRect(x, TWO_SLOT_LABEL_Y, TWO_SLOT_LABEL_W, TWO_SLOT_LABEL_H),
    s_font_20,
    GTextAlignmentCenter,
    text
  );
}

void two_slot_layout_load(Window *window) {
  Layer *root_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(root_layer);

  APP_LOG(
    APP_LOG_LEVEL_INFO,
    "Two-slot QA canvas: %d x %d",
    bounds.size.w,
    bounds.size.h
  );

  s_font_20 = fonts_load_custom_font(
    resource_get_handle(RESOURCE_ID_FONT_ROBOTO_FLEX_BOLD_20)
  );
  s_font_28 = fonts_load_custom_font(
    resource_get_handle(RESOURCE_ID_FONT_ROBOTO_FLEX_EXTRABOLD_28)
  );
  s_font_62 = fonts_load_custom_font(
    resource_get_handle(RESOURCE_ID_FONT_ROBOTO_FLEX_EXTRABOLD_62)
  );

  s_calendar_bitmap = gbitmap_create_with_resource(
    RESOURCE_ID_IMAGE_CALENDAR_64X64
  );
  s_weather_bitmap = gbitmap_create_with_resource(
    RESOURCE_ID_IMAGE_WEATHER_PARTLY_CLOUDY_64X64
  );

  s_background_layer = layer_create(
    GRect(0, 0, TWO_SLOT_CANVAS_W, TWO_SLOT_CANVAS_H)
  );
  layer_set_update_proc(s_background_layer, background_update_proc);
  layer_add_child(root_layer, s_background_layer);

  s_calendar_icon_layer = layer_create(
    GRect(
      TWO_SLOT_1_X,
      TWO_SLOT_ICON_Y,
      TWO_SLOT_ICON_W,
      TWO_SLOT_ICON_H
    )
  );
  layer_set_update_proc(s_calendar_icon_layer, calendar_icon_update_proc);
  layer_add_child(root_layer, s_calendar_icon_layer);
  s_calendar_value_layer = create_slot_value_layer(
    root_layer,
    TWO_SLOT_1_X,
    "Mer"
  );

  s_weather_icon_layer = bitmap_layer_create(
    GRect(
      TWO_SLOT_2_X,
      TWO_SLOT_ICON_Y,
      TWO_SLOT_ICON_W,
      TWO_SLOT_ICON_H
    )
  );
  bitmap_layer_set_bitmap(s_weather_icon_layer, s_weather_bitmap);
  bitmap_layer_set_background_color(s_weather_icon_layer, GColorClear);
  bitmap_layer_set_compositing_mode(s_weather_icon_layer, GCompOpSet);
  bitmap_layer_set_alignment(s_weather_icon_layer, GAlignCenter);
  layer_add_child(root_layer, bitmap_layer_get_layer(s_weather_icon_layer));
  s_weather_value_layer = create_slot_value_layer(
    root_layer,
    TWO_SLOT_2_X,
    "888°"
  );

  s_time_layer = create_text_layer(
    root_layer,
    GRect(
      TWO_SLOT_TIME_X,
      TWO_SLOT_TIME_Y,
      TWO_SLOT_TIME_W,
      TWO_SLOT_TIME_H
    ),
    s_font_62,
    GTextAlignmentCenter,
    "23:59"
  );
}

void two_slot_layout_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_weather_value_layer);
  text_layer_destroy(s_calendar_value_layer);

  bitmap_layer_destroy(s_weather_icon_layer);
  layer_destroy(s_calendar_icon_layer);
  layer_destroy(s_background_layer);

  gbitmap_destroy(s_weather_bitmap);
  gbitmap_destroy(s_calendar_bitmap);

  fonts_unload_custom_font(s_font_62);
  fonts_unload_custom_font(s_font_28);
  fonts_unload_custom_font(s_font_20);
}
