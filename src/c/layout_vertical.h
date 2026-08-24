#pragma once

#include <pebble.h>

// Figma source: REDUX - PEBBLE, node 334:9297.
// All measurements are native Pebble Time 2 pixels on the 200 x 228 Emery canvas.
//
// GEOMETRY RESET: revision 2, 2026-08-24.
// Revision 1 was approved against the previous wireframe and is now obsolete.
// Revision 2 remains pending emulator/Figma overlay approval.

#define VERTICAL_LAYOUT_LOCK_REVISION 2

#define VERTICAL_CANVAS_W 200
#define VERTICAL_CANVAS_H 228

// Figma explicitly specifies 74 + 8 + 120 = 202 px. The final 2 px of the
// time panel are clipped by the native 200 px watch canvas. Preserve the
// Figma geometry during QA rather than silently shrinking it.
#define VERTICAL_TRAY_X 0
#define VERTICAL_TRAY_Y 0
#define VERTICAL_TRAY_W 74
#define VERTICAL_TRAY_H 228

#define VERTICAL_DIVIDER_X 74
#define VERTICAL_DIVIDER_Y 0
#define VERTICAL_DIVIDER_W 8
#define VERTICAL_DIVIDER_H 228

#define VERTICAL_TIME_PANEL_X 82
#define VERTICAL_TIME_PANEL_Y 0
#define VERTICAL_TIME_PANEL_W 120
#define VERTICAL_TIME_PANEL_H 228

// Figma slot container is 54 x 46. Existing production artwork remains
// 52 x 44 and is centered at +1/+1 so it is never scaled.
#define VERTICAL_ICON_X 10
#define VERTICAL_ICON_W 54
#define VERTICAL_ICON_H 46
#define VERTICAL_ICON_BITMAP_X 1
#define VERTICAL_ICON_BITMAP_Y 1
#define VERTICAL_ICON_BITMAP_W 52
#define VERTICAL_ICON_BITMAP_H 44

#define VERTICAL_FIGMA_CALENDAR_DAY_X 11
#define VERTICAL_FIGMA_CALENDAR_DAY_Y 14
#define VERTICAL_FIGMA_CALENDAR_DAY_W 32
#define VERTICAL_FIGMA_CALENDAR_DAY_H 21
#define VERTICAL_CALENDAR_DAY_X 11
#define VERTICAL_CALENDAR_DAY_Y 11
#define VERTICAL_CALENDAR_DAY_W 32
#define VERTICAL_CALENDAR_DAY_H 21

// 3-slot tray: 8 px top/bottom, three 66 px groups, and 7 px gaps.
#define VERTICAL_SLOT_GROUP_H 66
#define VERTICAL_SLOT_GAP 7
#define VERTICAL_SLOT_1_ICON_Y 8
#define VERTICAL_SLOT_2_ICON_Y 81
#define VERTICAL_SLOT_3_ICON_Y 154

#define VERTICAL_LABEL_X 10
#define VERTICAL_LABEL_W 54
#define VERTICAL_LABEL_H 20
#define VERTICAL_SLOT_1_FIGMA_LABEL_Y 54
#define VERTICAL_SLOT_2_FIGMA_LABEL_Y 127
#define VERTICAL_SLOT_3_FIGMA_LABEL_Y 200
#define VERTICAL_SLOT_1_LABEL_Y 52
#define VERTICAL_SLOT_2_LABEL_Y 125
#define VERTICAL_SLOT_3_LABEL_Y 198

// The 120 px time panel uses 8 px horizontal padding in Figma.
#define VERTICAL_TIME_CONTENT_X 90
#define VERTICAL_TIME_CONTENT_W 104

// Final typography target is 88 px / 80 px line-height. This build still uses
// the known-good 93 px resource so resource generation remains unchanged.
#define VERTICAL_FIGMA_HOUR_Y 32
#define VERTICAL_FIGMA_MINUTE_Y 107
#define VERTICAL_FIGMA_TIME_VISIBLE_H 63
#define VERTICAL_FIGMA_TIME_LINE_H 80
#define VERTICAL_TIME_LINE_GAP 12

// Initial renderer frames for revision-2 overlay QA with the existing 93 px
// resource. These will be retuned when the exact 88 px resource is restored.
#define VERTICAL_TIME_LAYER_H 90
#define VERTICAL_TIME_HOUR_Y 8
#define VERTICAL_TIME_MINUTE_Y 83

#define VERTICAL_DATE_X 90
#define VERTICAL_FIGMA_DATE_Y 178
#define VERTICAL_DATE_Y 174
#define VERTICAL_DATE_W 104
#define VERTICAL_DATE_H 22

#define VERTICAL_LABEL_FONT_SIZE 20
#define VERTICAL_CALENDAR_FONT_SIZE 21
#define VERTICAL_DATE_FONT_SIZE 17
#define VERTICAL_TIME_FONT_SIZE 88

void vertical_layout_load(Window *window);
void vertical_layout_unload(Window *window);
