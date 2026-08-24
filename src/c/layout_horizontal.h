#pragma once

#include <pebble.h>

// Figma source: REDUX - PEBBLE, Redux 2.1.0 horizontal three-slot layout.
// All measurements are native Pebble Time 2 pixels on the 200 x 228 Emery canvas.
//
// GEOMETRY RESET: revision 2, pending emulator/Figma overlay approval.

#define HORIZONTAL_LAYOUT_LOCK_REVISION 2

#define HORIZONTAL_CANVAS_W 200
#define HORIZONTAL_CANVAS_H 228

#define HORIZONTAL_TRAY_X 0
#define HORIZONTAL_TRAY_Y 0
#define HORIZONTAL_TRAY_W 200
#define HORIZONTAL_TRAY_H 110

#define HORIZONTAL_TOP_DIVIDER_X 0
#define HORIZONTAL_TOP_DIVIDER_Y 110
#define HORIZONTAL_TOP_DIVIDER_W 200
#define HORIZONTAL_TOP_DIVIDER_H 8

#define HORIZONTAL_TIME_PANEL_X 0
#define HORIZONTAL_TIME_PANEL_Y 118
#define HORIZONTAL_TIME_PANEL_W 200
#define HORIZONTAL_TIME_PANEL_H 82

#define HORIZONTAL_BOTTOM_DIVIDER_X 0
#define HORIZONTAL_BOTTOM_DIVIDER_Y 200
#define HORIZONTAL_BOTTOM_DIVIDER_W 200
#define HORIZONTAL_BOTTOM_DIVIDER_H 8

#define HORIZONTAL_BOTTOM_STRIP_X 0
#define HORIZONTAL_BOTTOM_STRIP_Y 208
#define HORIZONTAL_BOTTOM_STRIP_W 200
#define HORIZONTAL_BOTTOM_STRIP_H 20

// Revision-2 tray: 11 px outer margins, 8 px gaps, and three 54 px groups.
#define HORIZONTAL_SLOT_1_X 11
#define HORIZONTAL_SLOT_2_X 73
#define HORIZONTAL_SLOT_3_X 135
#define HORIZONTAL_SLOT_W 54

// Revision-2 artwork is exported directly on the 54 x 46 Figma canvas.
#define HORIZONTAL_ICON_W 54
#define HORIZONTAL_ICON_H 46
#define HORIZONTAL_CALENDAR_ICON_Y 20
#define HORIZONTAL_STEPS_ICON_Y 20
#define HORIZONTAL_BATTERY_ICON_Y 20

// Keep the proven 18 px resource for this build-isolation step.
#define HORIZONTAL_LABEL_Y 68
#define HORIZONTAL_LABEL_W 54
#define HORIZONTAL_LABEL_H 18

// Calendar day typography is still on the proven 18 px resource for this step.
#define HORIZONTAL_CALENDAR_DAY_X 11
#define HORIZONTAL_CALENDAR_DAY_Y 12
#define HORIZONTAL_CALENDAR_DAY_W 32
#define HORIZONTAL_CALENDAR_DAY_H 18

#define HORIZONTAL_FIGMA_TIME_X 19
#define HORIZONTAL_FIGMA_TIME_Y 128
#define HORIZONTAL_FIGMA_TIME_W 162
#define HORIZONTAL_FIGMA_TIME_H 62

#define HORIZONTAL_TIME_X 0
#define HORIZONTAL_TIME_Y 117
#define HORIZONTAL_TIME_W 200
#define HORIZONTAL_TIME_H 64

#define HORIZONTAL_LABEL_FONT_SIZE 18
#define HORIZONTAL_TIME_FONT_SIZE 62

void horizontal_layout_load(Window *window);
void horizontal_layout_unload(Window *window);
