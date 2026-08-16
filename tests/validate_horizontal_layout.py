#!/usr/bin/env python3

import json
import re
import struct
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
HEADER = ROOT / "src" / "c" / "layout_horizontal.h"


def load_defines():
    values = {}
    pattern = re.compile(r"^#define\s+(HORIZONTAL_[A-Z0-9_]+)\s+(-?\d+)\s*$")
    for line in HEADER.read_text(encoding="utf-8").splitlines():
        match = pattern.match(line)
        if match:
            values[match.group(1)] = int(match.group(2))
    return values


def png_dimensions(path):
    with path.open("rb") as image:
        header = image.read(24)
    assert header[:8] == b"\x89PNG\r\n\x1a\n", f"Invalid PNG: {path}"
    return struct.unpack(">II", header[16:24])


def sfnt_tables(path):
    with path.open("rb") as font:
        header = font.read(12)
        assert len(header) == 12, f"Invalid font: {path}"
        _, table_count, _, _, _ = struct.unpack(">IHHHH", header)
        tables = set()
        for _ in range(table_count):
            record = font.read(16)
            assert len(record) == 16, f"Invalid font table directory: {path}"
            tables.add(record[:4].decode("ascii"))
    return tables


def main():
    d = load_defines()

    assert d["HORIZONTAL_CANVAS_W"] == 200
    assert d["HORIZONTAL_CANVAS_H"] == 228
    assert d["HORIZONTAL_TRAY_H"] == 116
    assert d["HORIZONTAL_TOP_DIVIDER_Y"] == 116
    assert d["HORIZONTAL_TOP_DIVIDER_H"] == 8
    assert d["HORIZONTAL_TIME_PANEL_Y"] == 124
    assert d["HORIZONTAL_TIME_PANEL_H"] == 76
    assert d["HORIZONTAL_BOTTOM_DIVIDER_Y"] == 200
    assert d["HORIZONTAL_BOTTOM_DIVIDER_H"] == 8
    assert d["HORIZONTAL_BOTTOM_STRIP_Y"] == 208
    assert d["HORIZONTAL_BOTTOM_STRIP_H"] == 20
    assert (
        d["HORIZONTAL_TRAY_H"]
        + d["HORIZONTAL_TOP_DIVIDER_H"]
        + d["HORIZONTAL_TIME_PANEL_H"]
        + d["HORIZONTAL_BOTTOM_DIVIDER_H"]
        + d["HORIZONTAL_BOTTOM_STRIP_H"]
        == d["HORIZONTAL_CANVAS_H"]
    )

    assert [
        d["HORIZONTAL_SLOT_1_X"],
        d["HORIZONTAL_SLOT_2_X"],
        d["HORIZONTAL_SLOT_3_X"],
    ] == [12, 74, 136]
    assert d["HORIZONTAL_SLOT_W"] == 52
    assert d["HORIZONTAL_ICON_W"] == 52
    assert d["HORIZONTAL_ICON_H"] == 44
    assert [
        d["HORIZONTAL_CALENDAR_ICON_Y"],
        d["HORIZONTAL_STEPS_ICON_Y"],
        d["HORIZONTAL_BATTERY_ICON_Y"],
    ] == [28, 27, 27]
    assert d["HORIZONTAL_LABEL_Y"] == 69
    assert d["HORIZONTAL_LABEL_H"] == 18
    assert [
        d["HORIZONTAL_CALENDAR_DAY_X"],
        d["HORIZONTAL_CALENDAR_DAY_Y"],
        d["HORIZONTAL_CALENDAR_DAY_W"],
        d["HORIZONTAL_CALENDAR_DAY_H"],
    ] == [10, 13, 32, 18]

    assert [
        d["HORIZONTAL_FIGMA_TIME_X"],
        d["HORIZONTAL_FIGMA_TIME_Y"],
        d["HORIZONTAL_FIGMA_TIME_W"],
        d["HORIZONTAL_FIGMA_TIME_H"],
    ] == [19, 131, 162, 62]
    assert [
        d["HORIZONTAL_TIME_X"],
        d["HORIZONTAL_TIME_Y"],
        d["HORIZONTAL_TIME_W"],
        d["HORIZONTAL_TIME_H"],
    ] == [0, 123, 200, 64]

    icon_paths = [
        ROOT / "resources/images/icons/52x44/calendar/white_calendar_52x44.png",
        ROOT / "resources/images/icons/52x44/activity/white_steps_52x44.png",
        ROOT / "resources/images/icons/52x44/battery/white_battery_100_52x44.png",
    ]
    for icon_path in icon_paths:
        assert png_dimensions(icon_path) == (52, 44), icon_path

    golden = ROOT / "qa/goldens/horizontal_3/figma-334-8203.png"
    assert png_dimensions(golden) == (200, 228), golden

    font_path = ROOT / "resources/fonts/RobotoFlex-ExtraBold-62.ttf"
    tables = sfnt_tables(font_path)
    assert "glyf" in tables, f"Expected TrueType outlines: {font_path}"
    assert "fvar" not in tables, f"Expected pinned static font: {font_path}"

    appinfo = json.loads((ROOT / "appinfo.json").read_text(encoding="utf-8"))
    resource_names = {item["name"] for item in appinfo["resources"]["media"]}
    assert "FONT_ROBOTO_FLEX_EXTRABOLD_62" in resource_names
    assert appinfo["targetPlatforms"] == ["emery"]

    print("Horizontal three-slot layout validation passed.")


if __name__ == "__main__":
    main()
