# Battery Indicator

Status: **approved and design locked**

- Lock revision: `1`
- Approved: August 17, 2026
- Approved 50% reference:
  [`qa/goldens/battery_indicator/approved-50-percent.png`](../qa/goldens/battery_indicator/approved-50-percent.png)
- Charging animation: approved in Pebble Time 2 emulator testing

Figma source: Battery Indicator component set, node `330:1453`; approved blue
instance, node `334:8182`.

The indicator is a styling addition inside the 200 × 20 px bottom strip that
already exists in the locked horizontal two-slot and three-slot layouts. No
locked layout coordinate, panel dimension, font size, or icon canvas changed.

## Geometry

| Element | X | Y | Width | Height |
| --- | ---: | ---: | ---: | ---: |
| Component | 0 | 0 | 200 | 20 |
| Inner content | 2 | 2 | 196 | 16 |
| Segment | varies | 2 | 16 | 16 |
| Percentage frame | 154 | 2 | 42 | 16 |

- Background: black
- Maximum segments: 9
- Segment gap: 1 px
- Outer scale corner radius: 2 px
- Percentage: Roboto Flex ExtraBold, 12 px, right aligned

## Colors

| State | Segments | Percentage |
| --- | --- | --- |
| Normal | Theme blue `#60A0FF` | White |
| Charging | Green `#00FF00` | Green `#00FF00` |

The normal segment color is configurable so production themes can override the
blue without changing indicator geometry.

## Charging animation

While charging below 100%, the blocks progressively fill from the actual
battery level to all nine blocks. Each additional block appears every 350 ms.
The full scale holds for 700 ms, then resets to the actual level and repeats.
The percentage label always shows the real battery value.

At 100%, all nine blocks remain steadily illuminated. The timer is cancelled
when charging stops or the indicator is destroyed, so the animation cannot
continue after the layout unloads.

## Percentage mapping

| Charge | Segments |
| --- | ---: |
| 90–100% | 9 |
| 80–89% | 8 |
| 70–79% | 7 |
| 60–69% | 6 |
| 50–59% | 5 |
| 40–49% | 4 |
| 30–39% | 3 |
| 20–29% | 2 |
| 1–19% | 1 |
| 0% | 0 |

This resolves the overlapping shorthand labels used by the Figma variants
(`99-89%`, `89-79%`, and so on) into deterministic runtime bands. The label
always displays the actual percentage.

## Layout availability

- `horizontal_2`: enabled in the reserved bottom strip
- `horizontal_3`: enabled in the reserved bottom strip
- `vertical_3`: not included; that layout has no 200 × 20 indicator strip

## Lock contract

The geometry, 12 px typography, normal and charging colors, percentage bands,
350 ms animation step, and 700 ms full-bar hold are locked at revision 1.
Connecting live battery and charging services does not change this contract.

Any change to those approved properties must:

1. Increment `BATTERY_INDICATOR_LOCK_REVISION`.
2. Update `tests/validate_battery_indicator.py`.
3. Pass a Pebble Time 2 emulator build.
4. Receive new visual and animation approval.
