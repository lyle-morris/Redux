#include <pebble.h>

#include "battery_indicator.h"

#define BATTERY_INDICATOR_PADDING 2
#define BATTERY_INDICATOR_SEGMENT_W 15
#define BATTERY_INDICATOR_SEGMENT_H 16
#define BATTERY_INDICATOR_SEGMENT_GAP 1
#define BATTERY_INDICATOR_MAX_SEGMENTS 9
// Figma Battery Indicator component: 2 px outer padding, 143 px scale,
// then a right-aligned 16 px percentage label. Pebble's generated font
// metrics are slightly wider than the browser metrics used by Figma, so
// the text frame overlaps the visual gap slightly. Right alignment keeps
// values such as "100%" fully visible while matching the Figma placement.
// Pebble's font baseline sits about 2 px lower than Figma's browser text,
// so the label frame is shifted up 2 px for visual alignment.
#define BATTERY_INDICATOR_LABEL_X 147
#define BATTERY_INDICATOR_LABEL_Y 0
#define BATTERY_INDICATOR_LABEL_W 53
#define BATTERY_INDICATOR_LABEL_H 16
#define BATTERY_INDICATOR_ANIMATION_STEP_MS 350
#define BATTERY_INDICATOR_ANIMATION_HOLD_MS 700

#define BATTERY_INDICATOR_COLOR_BACKGROUND GColorBlack
#define BATTERY_INDICATOR_COLOR_NORMAL GColorFromHEX(0x60A0FF)
#define BATTERY_INDICATOR_COLOR_CHARGING GColorFromHEX(0x00FF00)
#define BATTERY_INDICATOR_COLOR_LABEL GColorWhite

struct BatteryIndicator {
  Layer *layer;
  GFont font;
  uint8_t percentage;
  uint8_t animated_segment_count;
  bool charging;
  GColor normal_color;
  AppTimer *animation_timer;
  char label[5];
};
static BatteryIndicator *s_active_indicator;

static void battery_state_handler(BatteryChargeState state) {
  if(!s_active_indicator) return;
  battery_indicator_set_percentage(s_active_indicator, state.charge_percent);
  battery_indicator_set_charging(s_active_indicator, state.is_charging);
}

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

static void animation_timer_callback(void *context);

static void cancel_animation(BatteryIndicator *indicator) {
  if (!indicator->animation_timer) {
    return;
  }

  app_timer_cancel(indicator->animation_timer);
  indicator->animation_timer = NULL;
}

static void schedule_animation(
  BatteryIndicator *indicator,
  uint32_t delay_ms
) {
  indicator->animation_timer = app_timer_register(
    delay_ms,
    animation_timer_callback,
    indicator
  );
}

static void restart_animation(BatteryIndicator *indicator) {
  cancel_animation(indicator);
  indicator->animated_segment_count = segment_count_for_percentage(
    indicator->percentage
  );

  if (indicator->charging && indicator->percentage < 100) {
    schedule_animation(indicator, BATTERY_INDICATOR_ANIMATION_STEP_MS);
  }
}

static void animation_timer_callback(void *context) {
  BatteryIndicator *indicator = context;
  indicator->animation_timer = NULL;

  if (!indicator->charging || indicator->percentage >= 100) {
    return;
  }

  uint32_t next_delay = BATTERY_INDICATOR_ANIMATION_STEP_MS;
  if (indicator->animated_segment_count < BATTERY_INDICATOR_MAX_SEGMENTS) {
    indicator->animated_segment_count++;
    if (
      indicator->animated_segment_count == BATTERY_INDICATOR_MAX_SEGMENTS
    ) {
      next_delay = BATTERY_INDICATOR_ANIMATION_HOLD_MS;
    }
  } else {
    indicator->animated_segment_count = segment_count_for_percentage(
      indicator->percentage
    );
  }

  layer_mark_dirty(indicator->layer);
  schedule_animation(indicator, next_delay);
}

static void battery_indicator_update_proc(Layer *layer, GContext *ctx) {
  BatteryIndicator **layer_indicator = layer_get_data(layer);
  BatteryIndicator *indicator = *layer_indicator;
  GRect bounds = layer_get_bounds(layer);

  graphics_context_set_fill_color(ctx, BATTERY_INDICATOR_COLOR_BACKGROUND);
  graphics_fill_rect(ctx, bounds, 0, GCornerNone);

  uint8_t segment_count = indicator->charging
    ? indicator->animated_segment_count
    : segment_count_for_percentage(indicator->percentage);
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
    resource_get_handle(RESOURCE_ID_FONT_ROBOTO_FLEX_EXTRABOLD_16)
  );
  indicator->percentage = 100;
  indicator->animated_segment_count = BATTERY_INDICATOR_MAX_SEGMENTS;
  indicator->charging = false;
  indicator->normal_color = BATTERY_INDICATOR_COLOR_NORMAL;
  indicator->animation_timer = NULL;
  update_label(indicator);

  BatteryIndicator **layer_indicator = layer_get_data(indicator->layer);
  *layer_indicator = indicator;
  layer_set_update_proc(indicator->layer, battery_indicator_update_proc);
  s_active_indicator = indicator;
  BatteryChargeState state = battery_state_service_peek();
  battery_state_handler(state);
  battery_state_service_subscribe(battery_state_handler);

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
  restart_animation(indicator);
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
  restart_animation(indicator);
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

  cancel_animation(indicator);
  if(s_active_indicator == indicator) { battery_state_service_unsubscribe(); s_active_indicator = NULL; }
  fonts_unload_custom_font(indicator->font);
  layer_destroy(indicator->layer);
  free(indicator);
}
