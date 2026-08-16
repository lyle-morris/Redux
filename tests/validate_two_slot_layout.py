#!/usr/bin/env python3

import json
import re
import struct
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
HEADER = ROOT / "src" / "c" / "layout_two_slot.h"


def load_defines():
    values = {}
    pattern = re.compile(r"^#define\s+(TWO_SLOT_[A-Z0-9_]+)\s+(-?\d+)\s*$")
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

    assert d["TWO_SLOT_CANVAS_W"] == 200
    assert d["TWO_SLOT_CANVAS_H"] == 228
    assert d["TWO_SLOT_TRAY_H"] == 116
    assert d["TWO_SLOT_TOP_DIVIDER_Y"] == 116
    assert d["TWO_SLOT_TOP_DIVIDER_H"] == 8
    assert d["TWO_SLOT_TIME_PANEL_Y"] == 124
    assert d["TWO_SLOT_TIME_PANEL_H"] == 76
    assert d["TWO_SLOT_BOTTOM_DIVIDER_Y"] == 200
    assert d["TWO_SLOT_BOTTOM_DIVIDER_H"] == 8
    assert d["TWO_SLOT_BOTTOM_STRIP_Y"] == 208
    assert d["TWO_SLOT_BOTTOM_STRIP_H"] == 20
    assert (
        d["TWO_SLOT_TRAY_H"]
        + d["TWO_SLOT_TOP_DIVIDER_H"]
        + d["TWO_SLOT_TIME_PANEL_H"]
        + d["TWO_SLOT_BOTTOM_DIVIDER_H"]
        + d["TWO_SLOT_BOTTOM_STRIP_H"]
        == d["TWO_SLOT_CANVAS_H"]
    )

    assert [d["TWO_SLOT_1_X"], d["TWO_SLOT_2_X"]] == [24, 112]
    assert d["TWO_SLOT_ICON_Y"] == 16
    assert d["TWO_SLOT_ICON_W"] == 64
    assert d["TWO_SLOT_ICON_H"] == 64
    assert d["TWO_SLOT_FIGMA_LABEL_Y"] == 80
    assert d["TWO_SLOT_LABEL_Y"] == 78
    assert d["TWO_SLOT_LABEL_H"] == 20

    assert [
        d["TWO_SLOT_FIGMA_CALENDAR_DAY_X"],
        d["TWO_SLOT_FIGMA_CALENDAR_DAY_Y"],
        d["TWO_SLOT_FIGMA_CALENDAR_DAY_W"],
        d["TWO_SLOT_FIGMA_CALENDAR_DAY_H"],
    ] == [16, 21, 32, 28]
    assert [
        d["TWO_SLOT_CALENDAR_DAY_X"],
        d["TWO_SLOT_CALENDAR_DAY_Y"],
        d["TWO_SLOT_CALENDAR_DAY_W"],
        d["TWO_SLOT_CALENDAR_DAY_H"],
    ] == [16, 17, 32, 28]

    assert [
        d["TWO_SLOT_FIGMA_TIME_X"],
        d["TWO_SLOT_FIGMA_TIME_Y"],
        d["TWO_SLOT_FIGMA_TIME_W"],
        d["TWO_SLOT_FIGMA_TIME_H"],
    ] == [19, 131, 162, 62]
    assert [
        d["TWO_SLOT_TIME_X"],
        d["TWO_SLOT_TIME_Y"],
        d["TWO_SLOT_TIME_W"],
        d["TWO_SLOT_TIME_H"],
    ] == [0, 120, 200, 64]

    icon_paths = [
        ROOT / "resources/images/icons/64x64/calendar/white_calendar_64x64.png",
        ROOT
        / "resources/images/icons/64x64/weather/white_weather_partly_cloudy_64x64.png",
    ]
    for icon_path in icon_paths:
        assert png_dimensions(icon_path) == (64, 64), icon_path

    build_spec = ROOT / "qa/goldens/horizontal_2/build-specification.png"
    assert png_dimensions(build_spec) == (970, 1299), build_spec

    for font_name in ["RobotoFlex-Bold-20.ttf", "RobotoFlex-ExtraBold-28.ttf"]:
        font_path = ROOT / "resources/fonts" / font_name
        tables = sfnt_tables(font_path)
        assert "glyf" in tables, f"Expected TrueType outlines: {font_path}"
        assert "fvar" not in tables, f"Expected pinned static font: {font_path}"

    appinfo = json.loads((ROOT / "appinfo.json").read_text(encoding="utf-8"))
    resource_names = {item["name"] for item in appinfo["resources"]["media"]}
    assert "FONT_ROBOTO_FLEX_BOLD_20" in resource_names
    assert "FONT_ROBOTO_FLEX_EXTRABOLD_28" in resource_names
    assert "IMAGE_CALENDAR_64X64" in resource_names
    assert "IMAGE_WEATHER_PARTLY_CLOUDY_64X64" in resource_names
    assert appinfo["targetPlatforms"] == ["emery"]

    print("Horizontal two-slot layout validation passed.")


if __name__ == "__main__":
    main()
