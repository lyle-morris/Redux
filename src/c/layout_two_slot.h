#pragma once

#include <pebble.h>

// Figma source: REDUX - PEBBLE, Redux 2.1.0 horizontal two-slot layout.
// All measurements are native Pebble Time 2 pixels on the 200 x 228 Emery canvas.
//
// GEOMETRY RESET: revision 2, pending emulator/Figma overlay approval.

#define TWO_SLOT_LAYOUT_LOCK_REVISION 2

#define TWO_SLOT_CANVAS_W 200
#define TWO_SLOT_CANVAS_H 228

#define TWO_SLOT_TRAY_X 0
#define TWO_SLOT_TRAY_Y 0
#define TWO_SLOT_TRAY_W 200
#define TWO_SLOT_TRAY_H 110

#define TWO_SLOT_TOP_DIVIDER_X 0
#define TWO_SLOT_TOP_DIVIDER_Y 110
#define TWO_SLOT_TOP_DIVIDER_W 200
#define TWO_SLOT_TOP_DIVIDER_H 8

#define TWO_SLOT_TIME_PANEL_X 0
#define TWO_SLOT_TIME_PANEL_Y 118
#define TWO_SLOT_TIME_PANEL_W 200
#define TWO_SLOT_TIME_PANEL_H 82

#define TWO_SLOT_BOTTOM_DIVIDER_X 0
#define TWO_SLOT_BOTTOM_DIVIDER_Y 200
#define TWO_SLOT_BOTTOM_DIVIDER_W 200
#define TWO_SLOT_BOTTOM_DIVIDER_H 8

#define TWO_SLOT_BOTTOM_STRIP_X 0
#define TWO_SLOT_BOTTOM_STRIP_Y 208
#define TWO_SLOT_BOTTOM_STRIP_W 200
#define TWO_SLOT_BOTTOM_STRIP_H 20

// 24 px outer padding and gap: 24 + 64 + 24 + 64 + 24 = 200.
#define TWO_SLOT_1_X 24
#define TWO_SLOT_2_X 112
#define TWO_SLOT_ICON_Y 10
#define TWO_SLOT_ICON_W 64
#define TWO_SLOT_ICON_H 64

// Figma tray stack: 10 px top + 64 px icon + 2 px gap + 24 px label + 10 px bottom.
#define TWO_SLOT_FIGMA_LABEL_Y 76
// Initial Pebble renderer compensation for the custom 24 px font; tune by overlay.
#define TWO_SLOT_LABEL_Y 73
#define TWO_SLOT_LABEL_W 64
// Give descenders such as the g in "Aug" room without moving the approved baseline.
#define TWO_SLOT_LABEL_H 28

// Calendar day coordinates are local to the 64 x 64 custom icon layer.
#define TWO_SLOT_FIGMA_CALENDAR_DAY_X 16
#define TWO_SLOT_FIGMA_CALENDAR_DAY_Y 20
#define TWO_SLOT_FIGMA_CALENDAR_DAY_W 32
#define TWO_SLOT_FIGMA_CALENDAR_DAY_H 28
// Preserve the previously proven renderer compensation for the 28 px custom font.
#define TWO_SLOT_CALENDAR_DAY_X 16
#define TWO_SLOT_CALENDAR_DAY_Y 16
#define TWO_SLOT_CALENDAR_DAY_W 32
#define TWO_SLOT_CALENDAR_DAY_H 28

#define TWO_SLOT_FIGMA_TIME_X 19
#define TWO_SLOT_FIGMA_TIME_Y 128
#define TWO_SLOT_FIGMA_TIME_W 162
#define TWO_SLOT_FIGMA_TIME_H 62

// Share the locked Horizontal 3 time renderer frame.
#define TWO_SLOT_TIME_X 0
#define TWO_SLOT_TIME_Y 117
#define TWO_SLOT_TIME_W 200
#define TWO_SLOT_TIME_H 64

#define TWO_SLOT_LABEL_FONT_SIZE 24
#define TWO_SLOT_CALENDAR_FONT_SIZE 28
#define TWO_SLOT_TIME_FONT_SIZE 62

void two_slot_layout_load(Window *window);
void two_slot_layout_unload(Window *window);
