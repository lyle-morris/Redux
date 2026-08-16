#!/usr/bin/env python3

import json
import re
import struct
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
HEADER = ROOT / "src" / "c" / "layout_vertical.h"


def load_defines():
    values = {}
    pattern = re.compile(r"^#define\s+(VERTICAL_[A-Z0-9_]+)\s+(-?\d+)\s*$")
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

    assert d["VERTICAL_LAYOUT_LOCK_REVISION"] == 1
    assert d["VERTICAL_CANVAS_W"] == 200
    assert d["VERTICAL_CANVAS_H"] == 228
    assert (
        d["VERTICAL_TRAY_W"]
        + d["VERTICAL_DIVIDER_W"]
        + d["VERTICAL_TIME_PANEL_W"]
        == d["VERTICAL_CANVAS_W"]
    )
    assert d["VERTICAL_DIVIDER_X"] == d["VERTICAL_TRAY_W"]
    assert d["VERTICAL_TIME_PANEL_X"] == (
        d["VERTICAL_DIVIDER_X"] + d["VERTICAL_DIVIDER_W"]
    )
    assert d["VERTICAL_ICON_W"] == 52
    assert d["VERTICAL_ICON_H"] == 44
    assert [
        d["VERTICAL_CALENDAR_DAY_X"],
        d["VERTICAL_CALENDAR_DAY_Y"],
        d["VERTICAL_CALENDAR_DAY_W"],
        d["VERTICAL_CALENDAR_DAY_H"],
    ] == [10, 13, 32, 18]
    assert d["VERTICAL_LABEL_H"] == 18
    assert [
        d["VERTICAL_SLOT_1_ICON_Y"],
        d["VERTICAL_SLOT_2_ICON_Y"],
        d["VERTICAL_SLOT_3_ICON_Y"],
    ] == [16, 83, 151]
    assert [
        d["VERTICAL_SLOT_1_LABEL_Y"],
        d["VERTICAL_SLOT_2_LABEL_Y"],
        d["VERTICAL_SLOT_3_LABEL_Y"],
    ] == [57, 125, 192]
    assert d["VERTICAL_TIME_CONTENT_W"] == 108
    assert d["VERTICAL_FIGMA_TIME_LINE_H"] == 76
    assert d["VERTICAL_TIME_LAYER_H"] == 94
    assert d["VERTICAL_TIME_HOUR_Y"] == -2
    assert d["VERTICAL_TIME_MINUTE_Y"] == 80
    assert d["VERTICAL_DATE_X"] + d["VERTICAL_DATE_W"] == 194
    assert d["VERTICAL_DATE_Y"] == 192
    assert d["VERTICAL_DATE_H"] == 22

    icon_paths = [
        ROOT / "resources/images/icons/52x44/calendar/white_calendar_52x44.png",
        ROOT / "resources/images/icons/52x44/activity/white_steps_52x44.png",
        ROOT / "resources/images/icons/52x44/battery/white_battery_100_52x44.png",
    ]
    for icon_path in icon_paths:
        assert png_dimensions(icon_path) == (52, 44), icon_path

    approved_capture = (
        ROOT / "qa/goldens/vertical_3/approved-stress-emulator.png"
    )
    assert png_dimensions(approved_capture) == (200, 228), approved_capture

    for font_name in [
        "RobotoFlex-ExtraBold-18.ttf",
        "RobotoFlex-ExtraBold-93.ttf",
    ]:
        font_path = ROOT / "resources/fonts" / font_name
        tables = sfnt_tables(font_path)
        assert "glyf" in tables, f"Expected TrueType outlines: {font_path}"
        assert "fvar" not in tables, f"Expected pinned static font: {font_path}"

    appinfo = json.loads((ROOT / "appinfo.json").read_text(encoding="utf-8"))
    assert appinfo["targetPlatforms"] == ["emery"]
    assert appinfo["watchapp"]["watchface"] is True
    resource_names = {item["name"] for item in appinfo["resources"]["media"]}
    assert "IMAGE_BATTERY_100_52X44" in resource_names
    assert "IMAGE_WEATHER_CLOUDY_NIGHT_52X44" not in resource_names

    print("Vertical three-slot layout validation passed.")


if __name__ == "__main__":
    main()
