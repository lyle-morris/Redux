#pragma once

// Figma source: Redux Racer, node 334:8183
// All measurements are native Pebble Time 2 pixels on the 200 x 228 Emery canvas.

#define VERTICAL_CANVAS_W 200
#define VERTICAL_CANVAS_H 228

#define VERTICAL_TRAY_X 0
#define VERTICAL_TRAY_Y 0
#define VERTICAL_TRAY_W 72
#define VERTICAL_TRAY_H 228

#define VERTICAL_DIVIDER_X 72
#define VERTICAL_DIVIDER_Y 0
#define VERTICAL_DIVIDER_W 8
#define VERTICAL_DIVIDER_H 228

#define VERTICAL_TIME_PANEL_X 80
#define VERTICAL_TIME_PANEL_Y 0
#define VERTICAL_TIME_PANEL_W 120
#define VERTICAL_TIME_PANEL_H 228

#define VERTICAL_ICON_X 10
#define VERTICAL_ICON_W 52
#define VERTICAL_ICON_H 44

// Dynamic day number is drawn after the reusable blank calendar bitmap in the
// same custom Layer update pass. The calendar's white body occupies local
// x=12..39 and y=11..36 inside its 52 x 44 canvas. Pebble's 18 px custom-font
// inset centers the visible two-digit glyph inside that body.
#define VERTICAL_CALENDAR_DAY_X 10
#define VERTICAL_CALENDAR_DAY_Y 13
#define VERTICAL_CALENDAR_DAY_W 32
#define VERTICAL_CALENDAR_DAY_H 18

// Figma tray: 16 px vertical padding, centered content, 6 px between
// three 62 px icon/value groups. Emulator capture QA showed that Pebble
// renders the first two icon bitmaps one pixel above the Figma output and the
// custom 18 px text glyphs below it. These input frames compensate for those
// renderer offsets. The third slot is independently tuned from overlay QA.
#define VERTICAL_SLOT_GROUP_H 62
#define VERTICAL_SLOT_GAP 6
#define VERTICAL_SLOT_1_ICON_Y 16
#define VERTICAL_SLOT_2_ICON_Y 84
#define VERTICAL_SLOT_3_ICON_Y 151

#define VERTICAL_LABEL_X 0
#define VERTICAL_LABEL_W 72
#define VERTICAL_LABEL_H 18
#define VERTICAL_SLOT_1_LABEL_Y 57
#define VERTICAL_SLOT_2_LABEL_Y 125
#define VERTICAL_SLOT_3_LABEL_Y 192

#define VERTICAL_TIME_CONTENT_X 86
#define VERTICAL_TIME_CONTENT_W 108
#define VERTICAL_FIGMA_TIME_LINE_H 76
#define VERTICAL_TIME_LINE_GAP 6

// Roboto Flex 93 px has a 67 px visible numeral height, but Pebble places the
// glyph 25 px below the TextLayer origin. A 76 px layer therefore clips its
// lower 17 px. The expanded and shifted transparent layers preserve Figma's
// visible numeral bounds: hour y=23..90 and minute y=105..173.
#define VERTICAL_TIME_LAYER_H 94
#define VERTICAL_TIME_HOUR_Y -2
#define VERTICAL_TIME_MINUTE_Y 80

#define VERTICAL_DATE_X 86
#define VERTICAL_DATE_Y 192
#define VERTICAL_DATE_W 108
// The Figma frame is 18 px high. Pebble needs four additional transparent
// pixels to expose the custom font's descender instead of clipping it.
#define VERTICAL_DATE_H 22

#define VERTICAL_LABEL_FONT_SIZE 18
#define VERTICAL_TIME_FONT_SIZE 93
