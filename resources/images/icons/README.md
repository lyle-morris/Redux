# Redux Icon Upload Guide

## Production PNG folders

### 52 x 44 px

Shared by the vertical 3-slot and horizontal 3-slot layouts:

- `52x44/color/`
- `52x44/black/`
- `52x44/white/`

### 64 x 64 px

Used by the horizontal 2-slot layout:

- `64x64/color/`
- `64x64/black/`
- `64x64/white/`

## Naming

Use lowercase metric names and include the dimensions and variant:

```text
weather_52x44_color.png
weather_52x44_black.png
weather_52x44_white.png
weather_64x64_color.png
weather_64x64_black.png
weather_64x64_white.png
```

Use the same naming pattern for calendar, steps, calories, heart rate, battery, activity time, sleep, and future metrics.

## Rules

- Export each PNG at its exact runtime size.
- Do not upscale a 52 x 44 PNG for the 64 x 64 layout.
- Do not stretch square artwork into the 52 x 44 surface.
- Keep vector masters in `assets/icons/source/`.
- Preserve transparent padding from the approved Figma artwork.
