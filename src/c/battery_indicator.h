#pragma once

#include <pebble.h>

// Figma source: Battery Indicator component set, node 330:1453.
// The component always occupies the existing 200 x 20 reserved strip.
//
// DESIGN LOCKED: revision 1, approved 2026-08-17.
// Any geometry, typography, color-state, percentage-band, or animation-timing
// change requires a lock revision increment, validator update, and new QA
// approval.

#define BATTERY_INDICATOR_LOCK_REVISION 1

typedef struct BatteryIndicator BatteryIndicator;

BatteryIndicator *battery_indicator_create(GRect frame);
Layer *battery_indicator_get_layer(BatteryIndicator *indicator);

void battery_indicator_set_percentage(
  BatteryIndicator *indicator,
  uint8_t percentage
);
void battery_indicator_set_charging(
  BatteryIndicator *indicator,
  bool charging
);
void battery_indicator_set_normal_color(
  BatteryIndicator *indicator,
  GColor color
);

void battery_indicator_destroy(BatteryIndicator *indicator);
