#pragma once

#include <pebble.h>

// Redux 2.1.0 vertical layout geometry. The renderer supports both two-slot
// and three-slot tray states without a separate compilation unit.

#define VERTICAL_LAYOUT_LOCK_REVISION 4

#define VERTICAL_CANVAS_W 200
#define VERTICAL_CANVAS_H 228

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

#define VERTICAL_ICON_X 10
#define VERTICAL_ICON_W 54
#define VERTICAL_ICON_H 46

#define VERTICAL_CALENDAR_DAY_X 15
#define VERTICAL_CALENDAR_DAY_Y 11
#define VERTICAL_CALENDAR_DAY_W 24
#define VERTICAL_CALENDAR_DAY_H 21

// Three-slot tray: 8 px top/bottom, three 66 px groups and 7 px gaps.
#define VERTICAL_THREE_SLOT_1_ICON_Y 8
#define VERTICAL_THREE_SLOT_2_ICON_Y 81
#define VERTICAL_THREE_SLOT_3_ICON_Y 154
#define VERTICAL_THREE_SLOT_1_LABEL_Y 52
#define VERTICAL_THREE_SLOT_2_LABEL_Y 125
#define VERTICAL_THREE_SLOT_3_LABEL_Y 198

// Two-slot tray: two 66 px groups + 7 px gap centered vertically.
#define VERTICAL_TWO_SLOT_1_ICON_Y 45
#define VERTICAL_TWO_SLOT_2_ICON_Y 118
#define VERTICAL_TWO_SLOT_1_LABEL_Y 89
#define VERTICAL_TWO_SLOT_2_LABEL_Y 162

#define VERTICAL_LABEL_X 10
#define VERTICAL_LABEL_W 54
#define VERTICAL_LABEL_H 24

// Shift the entire time/date content 2px left from the approved r3 geometry.
#define VERTICAL_TIME_CONTENT_X 88
#define VERTICAL_TIME_CONTENT_W 104
#define VERTICAL_TIME_LAYER_H 90
#define VERTICAL_TIME_HOUR_Y 8
#define VERTICAL_TIME_MINUTE_Y 83

#define VERTICAL_DATE_X 88
// Minute layer ends at y=173; date starts 12px later.
#define VERTICAL_DATE_Y 185
#define VERTICAL_DATE_W 104
#define VERTICAL_DATE_H 26

#define VERTICAL_LABEL_FONT_SIZE 20
#define VERTICAL_CALENDAR_FONT_SIZE 21
#define VERTICAL_DATE_FONT_SIZE 20
#define VERTICAL_TIME_FONT_SIZE 88

void vertical_layout_load(Window *window);
void vertical_layout_unload(Window *window);
