#pragma once

#include <pebble.h>
#include <stddef.h>
#include <stdio.h>

// Battery slot icon levels mirror the bottom indicator behavior:
// 0% uses the empty icon, 1-9% uses the 10% icon, 10-89% floors to
// the matching 10% bucket, and 90-100% uses the 100% icon.
static inline uint8_t redux_battery_icon_bucket(uint8_t percentage) {
  if (percentage == 0) return 0;
  if (percentage < 10) return 10;
  if (percentage >= 90) return 100;
  return (percentage / 10) * 10;
}

static inline uint32_t redux_battery_resource_54(BatteryChargeState state) {
  if (state.is_charging) return RESOURCE_ID_IMAGE_BATTERY_CHARGING_54X46;

  switch (redux_battery_icon_bucket(state.charge_percent)) {
    case 0: return RESOURCE_ID_IMAGE_BATTERY_0_54X46;
    case 10: return RESOURCE_ID_IMAGE_BATTERY_10_54X46;
    case 20: return RESOURCE_ID_IMAGE_BATTERY_20_54X46;
    case 30: return RESOURCE_ID_IMAGE_BATTERY_30_54X46;
    case 40: return RESOURCE_ID_IMAGE_BATTERY_40_54X46;
    case 50: return RESOURCE_ID_IMAGE_BATTERY_50_54X46;
    case 60: return RESOURCE_ID_IMAGE_BATTERY_60_54X46;
    case 70: return RESOURCE_ID_IMAGE_BATTERY_70_54X46;
    case 80: return RESOURCE_ID_IMAGE_BATTERY_80_54X46;
    default: return RESOURCE_ID_IMAGE_BATTERY_100_54X46;
  }
}

static inline uint32_t redux_battery_resource_64(BatteryChargeState state) {
  if (state.is_charging) return RESOURCE_ID_IMAGE_BATTERY_CHARGING_64X64;

  switch (redux_battery_icon_bucket(state.charge_percent)) {
    case 0: return RESOURCE_ID_IMAGE_BATTERY_0_64X64;
    case 10: return RESOURCE_ID_IMAGE_BATTERY_10_64X64;
    case 20: return RESOURCE_ID_IMAGE_BATTERY_20_64X64;
    case 30: return RESOURCE_ID_IMAGE_BATTERY_30_64X64;
    case 40: return RESOURCE_ID_IMAGE_BATTERY_40_64X64;
    case 50: return RESOURCE_ID_IMAGE_BATTERY_50_64X64;
    case 60: return RESOURCE_ID_IMAGE_BATTERY_60_64X64;
    case 70: return RESOURCE_ID_IMAGE_BATTERY_70_64X64;
    case 80: return RESOURCE_ID_IMAGE_BATTERY_80_64X64;
    default: return RESOURCE_ID_IMAGE_BATTERY_100_64X64;
  }
}

static inline void redux_battery_format_label(
  BatteryChargeState state,
  char *buffer,
  size_t size
) {
  snprintf(buffer, size, "%u%%", (unsigned int)state.charge_percent);
}
