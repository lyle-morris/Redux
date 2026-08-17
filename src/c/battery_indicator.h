#pragma once

#include <pebble.h>

// Figma source: Battery Indicator component set, node 330:1453.
// The component always occupies the existing 200 x 20 reserved strip.

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
