#include <pebble.h>

#include "battery_indicator.h"

#define BATTERY_INDICATOR_PADDING 2
#define BATTERY_INDICATOR_SEGMENT_W 16
#define BATTERY_INDICATOR_SEGMENT_H 16
#define BATTERY_INDICATOR_SEGMENT_GAP 1
#define BATTERY_INDICATOR_MAX_SEGMENTS 9
#define BATTERY_INDICATOR_LABEL_X 154
#define BATTERY_INDICATOR_LABEL_Y 2
#define BATTERY_INDICATOR_LABEL_W 42
#define BATTERY_INDICATOR_LABEL_H 16

#define BATTERY_INDICATOR_COLOR_BACKGROUND GColorBlack
#define BATTERY_INDICATOR_COLOR_NORMAL GColorFromHEX(0x60A0FF)
#define BATTERY_INDICATOR_COLOR_CHARGING GColorFromHEX(0x00FF00)
#define BATTERY_INDICATOR_COLOR_LABEL GColorWhite

struct BatteryIndicator {
  Layer *layer;
  GFont font;
  uint8_t percentage;
  bool charging;
  GColor normal_color;
  char label[5];
};

static uint8_t segment_count_for_percentage(uint8_t percentage) {
  if (percentage == 0) {
    return 0;
  }

  uint8_t count = percentage / 10;
  if (count == 0) {
    count = 1;
  }
  if (count > BATTERY_INDICATOR_MAX_SEGMENTS) {
    count = BATTERY_INDICATOR_MAX_SEGMENTS;
  }
  return count;
}

static void update_label(BatteryIndicator *indicator) {
  snprintf(
    indicator->label,
    sizeof(indicator->label),
    "%u%%",
    (unsigned int)indicator->percentage
  );
}

static void battery_indicator_update_proc(Layer *layer, GContext *ctx) {
  BatteryIndicator **layer_indicator = layer_get_data(layer);
  BatteryIndicator *indicator = *layer_indicator;
  GRect bounds = layer_get_bounds(layer);

  graphics_context_set_fill_color(ctx, BATTERY_INDICATOR_COLOR_BACKGROUND);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);

  uint8_t segment_count = segment_count_for_percentage(
    indicator->percentage
  );
  GColor active_color = indicator->charging
    ? BATTERY_INDICATOR_COLOR_CHARGING
    : indicator->normal_color;

  graphics_context_set_fill_color(ctx, active_color);
  for (uint8_t index = 0; index < segment_count; ++index) {
    GCornerMask corners = GCornerNone;
    if (index == 0) {
      corners |= GCornerTopLeft | GCornerBottomLeft;
    }
    if (index == segment_count - 1) {
      corners |= GCornerTopRight | GCornerBottomRight;
    }

    graphics_fill_rect(
      ctx,
      GRect(
        BATTERY_INDICATOR_PADDING
          + index * (
            BATTERY_INDICATOR_SEGMENT_W + BATTERY_INDICATOR_SEGMENT_GAP
          ),
        BATTERY_INDICATOR_PADDING,
        BATTERY_INDICATOR_SEGMENT_W,
        BATTERY_INDICATOR_SEGMENT_H
      ),
      2,
      corners
    );
  }

  graphics_context_set_text_color(
    ctx,
    indicator->charging
      ? BATTERY_INDICATOR_COLOR_CHARGING
      : BATTERY_INDICATOR_COLOR_LABEL
  );
  graphics_draw_text(
    ctx,
    indicator->label,
    indicator->font,
    GRect(
      BATTERY_INDICATOR_LABEL_X,
      BATTERY_INDICATOR_LABEL_Y,
      BATTERY_INDICATOR_LABEL_W,
      BATTERY_INDICATOR_LABEL_H
    ),
    GTextOverflowModeFill,
    GTextAlignmentRight,
    NULL
  );
}

BatteryIndicator *battery_indicator_create(GRect frame) {
  BatteryIndicator *indicator = malloc(sizeof(BatteryIndicator));
  if (!indicator) {
    return NULL;
  }

  indicator->layer = layer_create_with_data(
    frame,
    sizeof(BatteryIndicator *)
  );
  if (!indicator->layer) {
    free(indicator);
    return NULL;
  }

  indicator->font = fonts_load_custom_font(
    resource_get_handle(RESOURCE_ID_FONT_ROBOTO_FLEX_EXTRABOLD_12)
  );
  indicator->percentage = 100;
  indicator->charging = false;
  indicator->normal_color = BATTERY_INDICATOR_COLOR_NORMAL;
  update_label(indicator);

  BatteryIndicator **layer_indicator = layer_get_data(indicator->layer);
  *layer_indicator = indicator;
  layer_set_update_proc(indicator->layer, battery_indicator_update_proc);

  return indicator;
}

Layer *battery_indicator_get_layer(BatteryIndicator *indicator) {
  return indicator ? indicator->layer : NULL;
}

void battery_indicator_set_percentage(
  BatteryIndicator *indicator,
  uint8_t percentage
) {
  if (!indicator) {
    return;
  }

  indicator->percentage = percentage > 100 ? 100 : percentage;
  update_label(indicator);
  layer_mark_dirty(indicator->layer);
}

void battery_indicator_set_charging(
  BatteryIndicator *indicator,
  bool charging
) {
  if (!indicator) {
    return;
  }

  indicator->charging = charging;
  layer_mark_dirty(indicator->layer);
}

void battery_indicator_set_normal_color(
  BatteryIndicator *indicator,
  GColor color
) {
  if (!indicator) {
    return;
  }

  indicator->normal_color = color;
  layer_mark_dirty(indicator->layer);
}

void battery_indicator_destroy(BatteryIndicator *indicator) {
  if (!indicator) {
    return;
  }

  fonts_unload_custom_font(indicator->font);
  layer_destroy(indicator->layer);
  free(indicator);
}
