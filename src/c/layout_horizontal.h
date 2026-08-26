#pragma once

#include <pebble.h>

// Figma source: REDUX - PEBBLE, Redux 2.1.0 horizontal three-slot layout.
// All measurements are native Pebble Time 2 pixels on the 200 x 228 Emery canvas.
//
// GEOMETRY LOCKED: revision 2, approved 2026-08-24.
// Revision 3 changes only the calendar-day typography box for QA.

#define HORIZONTAL_LAYOUT_LOCK_REVISION 3

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

#define HORIZONTAL_LABEL_Y 68
#define HORIZONTAL_LABEL_W 54
// Give descenders such as the g in "Aug" room without moving the approved baseline.
#define HORIZONTAL_LABEL_H 24

// Figma 54x46 calendar day box: 15 px left/right, 14 px top,
// 11 px bottom => 24 x 21 px. Pebble's generated 21 px font needs
// a 3 px upward frame compensation to match the visible Figma baseline.
#define HORIZONTAL_FIGMA_CALENDAR_DAY_X 15
#define HORIZONTAL_FIGMA_CALENDAR_DAY_Y 14
#define HORIZONTAL_FIGMA_CALENDAR_DAY_W 24
#define HORIZONTAL_FIGMA_CALENDAR_DAY_H 21
#define HORIZONTAL_CALENDAR_DAY_X 15
#define HORIZONTAL_CALENDAR_DAY_Y 11
#define HORIZONTAL_CALENDAR_DAY_W 24
#define HORIZONTAL_CALENDAR_DAY_H 21

#define HORIZONTAL_FIGMA_TIME_X 19
#define HORIZONTAL_FIGMA_TIME_Y 128
#define HORIZONTAL_FIGMA_TIME_W 162
#define HORIZONTAL_FIGMA_TIME_H 62

#define HORIZONTAL_TIME_X 0
#define HORIZONTAL_TIME_Y 117
#define HORIZONTAL_TIME_W 200
#define HORIZONTAL_TIME_H 64

#define HORIZONTAL_LABEL_FONT_SIZE 20
#define HORIZONTAL_CALENDAR_FONT_SIZE 21
#define HORIZONTAL_TIME_FONT_SIZE 62

void horizontal_layout_load(Window *window);
void horizontal_layout_unload(Window *window);
