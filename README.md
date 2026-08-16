# Redux

Pebble Time 2 watchface project for the Redux layouts.

## Current build

The `vertical_3` and `horizontal_3` layouts are approved and geometry locked at
revision 1. The active QA build now targets the standard `horizontal_2` layout
with static maximum-width time, weather, and calendar values.

See [`docs/vertical-3-layout.md`](docs/vertical-3-layout.md) and
[`docs/horizontal-3-layout.md`](docs/horizontal-3-layout.md) for the locked
three-slot contracts. The active two-slot QA contract is documented in
[`docs/horizontal-2-layout.md`](docs/horizontal-2-layout.md).

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
