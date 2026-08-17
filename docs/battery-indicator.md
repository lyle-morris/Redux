# Battery Indicator

Status: **implemented for horizontal layout QA**

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
