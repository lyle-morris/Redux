#pragma once

#include <pebble.h>

// Figma content source: Redux Racer, node 334:8203
// All measurements are native Pebble Time 2 pixels on the 200 x 228 Emery canvas.

#define HORIZONTAL_CANVAS_W 200
#define HORIZONTAL_CANVAS_H 228

#define HORIZONTAL_TRAY_X 0
#define HORIZONTAL_TRAY_Y 0
#define HORIZONTAL_TRAY_W 200
#define HORIZONTAL_TRAY_H 116

#define HORIZONTAL_TOP_DIVIDER_X 0
#define HORIZONTAL_TOP_DIVIDER_Y 116
#define HORIZONTAL_TOP_DIVIDER_W 200
#define HORIZONTAL_TOP_DIVIDER_H 8

#define HORIZONTAL_TIME_PANEL_X 0
#define HORIZONTAL_TIME_PANEL_Y 124
#define HORIZONTAL_TIME_PANEL_W 200
#define HORIZONTAL_TIME_PANEL_H 76

#define HORIZONTAL_BOTTOM_DIVIDER_X 0
#define HORIZONTAL_BOTTOM_DIVIDER_Y 200
#define HORIZONTAL_BOTTOM_DIVIDER_W 200
#define HORIZONTAL_BOTTOM_DIVIDER_H 8

#define HORIZONTAL_BOTTOM_STRIP_X 0
#define HORIZONTAL_BOTTOM_STRIP_Y 208
#define HORIZONTAL_BOTTOM_STRIP_W 200
#define HORIZONTAL_BOTTOM_STRIP_H 20

// Figma tray: 12 px horizontal padding, 27 px vertical padding, and 10 px
// gaps between three 52 x 62 icon/value groups.
#define HORIZONTAL_SLOT_1_X 12
#define HORIZONTAL_SLOT_2_X 74
#define HORIZONTAL_SLOT_3_X 136
#define HORIZONTAL_SLOT_W 52

#define HORIZONTAL_ICON_W 52
#define HORIZONTAL_ICON_H 44
#define HORIZONTAL_CALENDAR_ICON_Y 28
#define HORIZONTAL_STEPS_ICON_Y 27
#define HORIZONTAL_BATTERY_ICON_Y 27

// Custom-font glyphs render two pixels below their Figma line boxes on Emery.
#define HORIZONTAL_LABEL_Y 69
#define HORIZONTAL_LABEL_W 52
#define HORIZONTAL_LABEL_H 18

// Local coordinates inside the 52 x 44 calendar custom layer.
#define HORIZONTAL_CALENDAR_DAY_X 10
#define HORIZONTAL_CALENDAR_DAY_Y 13
#define HORIZONTAL_CALENDAR_DAY_W 32
#define HORIZONTAL_CALENDAR_DAY_H 18

#define HORIZONTAL_FIGMA_TIME_X 19
#define HORIZONTAL_FIGMA_TIME_Y 131
#define HORIZONTAL_FIGMA_TIME_W 162
#define HORIZONTAL_FIGMA_TIME_H 62

// Pebble input frame compensates for the custom font's baseline metrics. It
// spans the full panel because Pebble's text engine ellipsizes the 161.54 px
// stress value inside Figma's 162 px frame. Centering the transparent 200 px
// layer preserves the intended visible x position (approximately 19.23 px).
#define HORIZONTAL_TIME_X 0
#define HORIZONTAL_TIME_Y 123
#define HORIZONTAL_TIME_W 200
#define HORIZONTAL_TIME_H 64

#define HORIZONTAL_LABEL_FONT_SIZE 18
#define HORIZONTAL_TIME_FONT_SIZE 62

void horizontal_layout_load(Window *window);
void horizontal_layout_unload(Window *window);
