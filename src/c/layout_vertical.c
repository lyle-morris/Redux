#include <pebble.h>

#include "layout_vertical.h"

// Static Figma-reference build for vertical three-slot pixel QA.
// Live settings and data are intentionally connected after geometry approval.

#define COLOR_TRAY GColorFromHEX(0x00AAFF)
#define COLOR_DIVIDER GColorBlack
#define COLOR_TIME_PANEL GColorWhite
#define COLOR_TEXT GColorBlack

static Layer *s_background_layer;

static Layer *s_calendar_icon_layer;
static BitmapLayer *s_steps_icon_layer;
static BitmapLayer *s_battery_icon_layer;

static TextLayer *s_calendar_value_layer;
static TextLayer *s_steps_value_layer;
static TextLayer *s_battery_value_layer;
static TextLayer *s_hour_layer;
static TextLayer *s_minute_layer;
static TextLayer *s_date_layer;

static GBitmap *s_calendar_bitmap;
static GBitmap *s_steps_bitmap;
static GBitmap *s_battery_bitmap;

static GFont s_font_18;
static GFont s_font_93;

static void calendar_icon_update_proc(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);

  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  graphics_draw_bitmap_in_rect(ctx, s_calendar_bitmap, bounds);

  graphics_context_set_text_color(ctx, COLOR_TEXT);
  graphics_draw_text(
    ctx,
    "30",
    s_font_18,
    GRect(
      VERTICAL_CALENDAR_DAY_X,
      VERTICAL_CALENDAR_DAY_Y,
      VERTICAL_CALENDAR_DAY_W,
      VERTICAL_CALENDAR_DAY_H
    ),
    GTextOverflowModeFill,
    GTextAlignmentCenter,
    NULL
  );
}

static void background_update_proc(Layer *layer, GContext *ctx) {
  graphics_context_set_fill_color(ctx, COLOR_TRAY);
  graphics_fill_rect(
    ctx,
    GRect(VERTICAL_TRAY_X, VERTICAL_TRAY_Y, VERTICAL_TRAY_W, VERTICAL_TRAY_H),
    0,
    GCornerNone
  );

  graphics_context_set_fill_color(ctx, COLOR_DIVIDER);
  graphics_fill_rect(
    ctx,
    GRect(
      VERTICAL_DIVIDER_X,
      VERTICAL_DIVIDER_Y,
      VERTICAL_DIVIDER_W,
      VERTICAL_DIVIDER_H
    ),
    0,
    GCornerNone
  );

  graphics_context_set_fill_color(ctx, COLOR_TIME_PANEL);
  graphics_fill_rect(
    ctx,
    GRect(
      VERTICAL_TIME_PANEL_X,
      VERTICAL_TIME_PANEL_Y,
      VERTICAL_TIME_PANEL_W,
      VERTICAL_TIME_PANEL_H
    ),
    0,
    GCornerNone
  );
}

static BitmapLayer *create_icon_layer(
  Layer *parent,
  int16_t y,
  GBitmap *bitmap
) {
  BitmapLayer *layer = bitmap_layer_create(
    GRect(VERTICAL_ICON_X, y, VERTICAL_ICON_W, VERTICAL_ICON_H)
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

static TextLayer *create_tray_value_layer(
  Layer *parent,
  int16_t y,
  const char *text
) {
  return create_text_layer(
    parent,
    GRect(VERTICAL_LABEL_X, y, VERTICAL_LABEL_W, VERTICAL_LABEL_H),
    s_font_18,
    GTextAlignmentCenter,
    text
  );
}

void vertical_layout_load(Window *window) {
  Layer *root_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(root_layer);

  APP_LOG(
    APP_LOG_LEVEL_INFO,
    "Vertical QA canvas: %d x %d",
    bounds.size.w,
    bounds.size.h
  );

  s_font_18 = fonts_load_custom_font(
    resource_get_handle(RESOURCE_ID_FONT_ROBOTO_FLEX_EXTRABOLD_18)
  );
  s_font_93 = fonts_load_custom_font(
    resource_get_handle(RESOURCE_ID_FONT_ROBOTO_FLEX_EXTRABOLD_93)
  );

  s_calendar_bitmap = gbitmap_create_with_resource(
    RESOURCE_ID_IMAGE_CALENDAR_52X44
  );
  s_steps_bitmap = gbitmap_create_with_resource(
    RESOURCE_ID_IMAGE_STEPS_52X44
  );
  s_battery_bitmap = gbitmap_create_with_resource(
    RESOURCE_ID_IMAGE_BATTERY_100_52X44
  );

  s_background_layer = layer_create(
    GRect(0, 0, VERTICAL_CANVAS_W, VERTICAL_CANVAS_H)
  );
  layer_set_update_proc(s_background_layer, background_update_proc);
  layer_add_child(root_layer, s_background_layer);

  s_calendar_icon_layer = layer_create(
    GRect(
      VERTICAL_ICON_X,
      VERTICAL_SLOT_1_ICON_Y,
      VERTICAL_ICON_W,
      VERTICAL_ICON_H
    )
  );
  layer_set_update_proc(s_calendar_icon_layer, calendar_icon_update_proc);
  layer_add_child(root_layer, s_calendar_icon_layer);
  s_calendar_value_layer = create_tray_value_layer(
    root_layer,
    VERTICAL_SLOT_1_LABEL_Y,
    "Mer"
  );

  s_steps_icon_layer = create_icon_layer(
    root_layer,
    VERTICAL_SLOT_2_ICON_Y,
    s_steps_bitmap
  );
  s_steps_value_layer = create_tray_value_layer(
    root_layer,
    VERTICAL_SLOT_2_LABEL_Y,
    "99999"
  );

  s_battery_icon_layer = create_icon_layer(
    root_layer,
    VERTICAL_SLOT_3_ICON_Y,
    s_battery_bitmap
  );
  s_battery_value_layer = create_tray_value_layer(
    root_layer,
    VERTICAL_SLOT_3_LABEL_Y,
    "100%"
  );

  s_hour_layer = create_text_layer(
    root_layer,
    GRect(
      VERTICAL_TIME_CONTENT_X,
      VERTICAL_TIME_HOUR_Y,
      VERTICAL_TIME_CONTENT_W,
      VERTICAL_TIME_LAYER_H
    ),
    s_font_93,
    GTextAlignmentRight,
    "23"
  );

  s_minute_layer = create_text_layer(
    root_layer,
    GRect(
      VERTICAL_TIME_CONTENT_X,
      VERTICAL_TIME_MINUTE_Y,
      VERTICAL_TIME_CONTENT_W,
      VERTICAL_TIME_LAYER_H
    ),
    s_font_93,
    GTextAlignmentRight,
    "59"
  );

  s_date_layer = create_text_layer(
    root_layer,
    GRect(VERTICAL_DATE_X, VERTICAL_DATE_Y, VERTICAL_DATE_W, VERTICAL_DATE_H),
    s_font_18,
    GTextAlignmentCenter,
    "Sept 30 Mer"
  );
}

void vertical_layout_unload(Window *window) {
  text_layer_destroy(s_date_layer);
  text_layer_destroy(s_minute_layer);
  text_layer_destroy(s_hour_layer);

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

  fonts_unload_custom_font(s_font_93);
  fonts_unload_custom_font(s_font_18);
}
