# Redux Icon Asset Guide

Redux uses monochrome icon artwork. Separate color icon variants are intentionally out of scope so the asset count remains manageable across metrics, states, sizes, and layouts.

## Production PNG folders

### 52 × 44 px

Shared by the Essential Redux three-slot layouts:

- Vertical three-slot
- Horizontal three-slot

```text
52x44/
├── activity/
├── battery/
├── calendar/
└── weather/
```

### 64 × 64 px

Used by the Essential Redux two-slot layout.

```text
64x64/
├── activity/
├── battery/
├── calendar/
└── weather/
```

## Variants

Each icon has two monochrome treatments:

- `white_`: white-filled artwork with black details or outlines.
- `reverse_`: white outline artwork for dark or black panels.

Panel colors come from the watchface theme rather than from separate color icon files.

## Naming

Use lowercase snake case with the variant first and canvas dimensions last:

```text
{variant}_{metric_or_condition}_{width}x{height}.png
```

Examples:

```text
white_steps_52x44.png
reverse_steps_52x44.png
white_weather_partly_cloudy_64x64.png
reverse_weather_partly_cloudy_64x64.png
```

Corresponding 52 × 44 and 64 × 64 assets must use the same base name.

## Battery states and mapping

Each size and variant includes:

```text
0, 10, 20, 30, 40, 50, 60, 70, 80, 100, charging
```

Runtime mapping follows the battery indicator's floor-based behavior:

| Battery percentage | Asset |
| --- | --- |
| 0–9% | `battery_0` |
| 10–19% | `battery_10` |
| 20–29% | `battery_20` |
| 30–39% | `battery_30` |
| 40–49% | `battery_40` |
| 50–59% | `battery_50` |
| 60–69% | `battery_60` |
| 70–79% | `battery_70` |
| 80–99% | `battery_80` |
| 100% | `battery_100` |
| Charging | `battery_charging` |

Charging state overrides the percentage asset. A separate 90% asset is intentionally not required.

## Export rules

- Export every PNG at its exact runtime canvas size.
- Preserve the approved transparent padding.
- Preserve RGBA transparency.
- Do not upscale a 52 × 44 PNG for the 64 × 64 layout.
- Do not stretch square artwork into the 52 × 44 canvas.
- Keep the white and reverse asset sets paired.
- Keep vector masters in `assets/icons/source/`.
