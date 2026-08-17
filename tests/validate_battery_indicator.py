#!/usr/bin/env python3

import json
import re
import struct
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "src" / "c" / "battery_indicator.c"
HEADER = ROOT / "src" / "c" / "battery_indicator.h"
APPINFO = ROOT / "appinfo.json"


def define_int(name: str, text: str) -> int:
    match = re.search(rf"^#define\s+{name}\s+(\d+)\s*$", text, re.MULTILINE)
    assert match, f"missing integer define: {name}"
    return int(match.group(1))


def sfnt_tables(path: Path) -> set[str]:
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


def png_dimensions(path: Path) -> tuple[int, int]:
    with path.open("rb") as image:
        header = image.read(24)
    assert header[:8] == b"\x89PNG\r\n\x1a\n", f"Invalid PNG: {path}"
    return struct.unpack(">II", header[16:24])


def main() -> None:
    source = SOURCE.read_text()
    header = HEADER.read_text()
    appinfo = json.loads(APPINFO.read_text())

    assert define_int("BATTERY_INDICATOR_LOCK_REVISION", header) == 1

    assert define_int("BATTERY_INDICATOR_PADDING", source) == 2
    assert define_int("BATTERY_INDICATOR_SEGMENT_W", source) == 16
    assert define_int("BATTERY_INDICATOR_SEGMENT_H", source) == 16
    assert define_int("BATTERY_INDICATOR_SEGMENT_GAP", source) == 1
    assert define_int("BATTERY_INDICATOR_MAX_SEGMENTS", source) == 9
    assert define_int("BATTERY_INDICATOR_LABEL_X", source) == 154
    assert define_int("BATTERY_INDICATOR_LABEL_Y", source) == 2
    assert define_int("BATTERY_INDICATOR_LABEL_W", source) == 42
    assert define_int("BATTERY_INDICATOR_LABEL_H", source) == 16
    assert define_int("BATTERY_INDICATOR_ANIMATION_STEP_MS", source) == 350
    assert define_int("BATTERY_INDICATOR_ANIMATION_HOLD_MS", source) == 700

    assert "0x60A0FF" in source
    assert "0x00FF00" in source
    assert "RESOURCE_ID_FONT_ROBOTO_FLEX_EXTRABOLD_12" in source
    assert "app_timer_register" in source
    assert "app_timer_cancel" in source
    assert "animated_segment_count" in source

    approved_capture = (
        ROOT
        / "qa"
        / "goldens"
        / "battery_indicator"
        / "approved-50-percent.png"
    )
    assert png_dimensions(approved_capture) == (637, 719), approved_capture

    resources = appinfo["resources"]["media"]
    font = next(
        item
        for item in resources
        if item["name"] == "FONT_ROBOTO_FLEX_EXTRABOLD_12"
    )
    assert font["file"] == "fonts/RobotoFlex-ExtraBold-12.ttf"
    assert font["characterRegex"] == "[0-9%]"
    font_path = ROOT / "resources" / font["file"]
    tables = sfnt_tables(font_path)
    assert "glyf" in tables, f"Expected TrueType outlines: {font_path}"
    assert "fvar" not in tables, f"Expected pinned static font: {font_path}"

    for layout_source in ("layout_horizontal.c", "layout_two_slot.c"):
        text = (ROOT / "src" / "c" / layout_source).read_text()
        assert "battery_indicator_create" in text
        assert "battery_indicator_destroy" in text

    print("Battery indicator specification validated.")


if __name__ == "__main__":
    main()
