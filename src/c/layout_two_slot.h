#pragma once

#include <pebble.h>

// Geometry source: Build Specifications, "Horizontal layout (2 info slots)".
// Figma reference: Redux Racer, node 295:4551. The build specification
// supersedes that legacy frame's Teko typography and 229 px canvas.
//
// GEOMETRY LOCKED: revision 1, approved 2026-08-17.
// Any coordinate, dimension, font size, or icon-canvas change requires a lock
// revision increment, validator update, and new emulator/Figma overlay approval.

#define TWO_SLOT_LAYOUT_LOCK_REVISION 1

#define TWO_SLOT_CANVAS_W 200
#define TWO_SLOT_CANVAS_H 228

#define TWO_SLOT_TRAY_X 0
#define TWO_SLOT_TRAY_Y 0
#define TWO_SLOT_TRAY_W 200
#define TWO_SLOT_TRAY_H 116

#define TWO_SLOT_TOP_DIVIDER_X 0
#define TWO_SLOT_TOP_DIVIDER_Y 116
#define TWO_SLOT_TOP_DIVIDER_W 200
#define TWO_SLOT_TOP_DIVIDER_H 8

#define TWO_SLOT_TIME_PANEL_X 0
#define TWO_SLOT_TIME_PANEL_Y 124
#define TWO_SLOT_TIME_PANEL_W 200
#define TWO_SLOT_TIME_PANEL_H 76

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
#define TWO_SLOT_ICON_Y 16
#define TWO_SLOT_ICON_W 64
#define TWO_SLOT_ICON_H 64

#define TWO_SLOT_FIGMA_LABEL_Y 80
// Approved renderer compensation for the custom 20 px font.
#define TWO_SLOT_LABEL_Y 78
#define TWO_SLOT_LABEL_W 64
#define TWO_SLOT_LABEL_H 20

// Calendar day coordinates are local to the 64 x 64 custom icon layer.
#define TWO_SLOT_FIGMA_CALENDAR_DAY_X 16
#define TWO_SLOT_FIGMA_CALENDAR_DAY_Y 21
#define TWO_SLOT_FIGMA_CALENDAR_DAY_W 32
#define TWO_SLOT_FIGMA_CALENDAR_DAY_H 28
// Approved renderer-only vertical compensation for the custom 28 px font.
#define TWO_SLOT_CALENDAR_DAY_X 16
#define TWO_SLOT_CALENDAR_DAY_Y 17
#define TWO_SLOT_CALENDAR_DAY_W 32
#define TWO_SLOT_CALENDAR_DAY_H 28

#define TWO_SLOT_FIGMA_TIME_X 19
#define TWO_SLOT_FIGMA_TIME_Y 131
#define TWO_SLOT_FIGMA_TIME_W 162
#define TWO_SLOT_FIGMA_TIME_H 62

// Reuse the approved horizontal time renderer frame. The transparent 200 px
// width avoids Pebble ellipsis while retaining the visible Figma centering.
#define TWO_SLOT_TIME_X 0
#define TWO_SLOT_TIME_Y 120
#define TWO_SLOT_TIME_W 200
#define TWO_SLOT_TIME_H 64

#define TWO_SLOT_LABEL_FONT_SIZE 20
#define TWO_SLOT_CALENDAR_FONT_SIZE 28
#define TWO_SLOT_TIME_FONT_SIZE 62

void two_slot_layout_load(Window *window);
void two_slot_layout_unload(Window *window);
