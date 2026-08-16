# Redux

Pebble Time 2 watchface project for the Redux layouts.

## Current build

The Figma-specified `vertical_3` layout is approved and geometry locked at
revision 1. The active QA build now targets the `horizontal_3` layout with
static maximum-width time, steps, battery, and calendar values.

See [`docs/vertical-3-layout.md`](docs/vertical-3-layout.md) for the locked
vertical contract and [`docs/horizontal-3-layout.md`](docs/horizontal-3-layout.md)
for the active horizontal QA contract.

## Target layouts

- `horizontal_2`
- `horizontal_3`
- `vertical_3`

All variants render on the native 200 x 228 px Pebble Time 2 canvas.

## Working branch

Pixel-perfect layout development is performed on `dev/pixel-perfect-layouts`.

## Icon uploads

Production icons live under `resources/images/icons/`, grouped by canvas size and metric. Redux uses paired `white_` and `reverse_` monochrome artwork; panel color comes from the theme.

- `52x44/` is shared by the vertical and horizontal three-slot layouts.
- `64x64/` is used by the horizontal two-slot layout.

Vector masters belong in `assets/icons/source/`.
