# Redux

Pebble Time 2 watchface project for the Redux layouts.

## Current build

The `vertical_3`, `horizontal_3`, and standard `horizontal_2` layouts are all
approved and geometry locked at revision 1. The active static build remains on
`horizontal_2` for the next integration phase.

The locked contracts are documented in
[`docs/vertical-3-layout.md`](docs/vertical-3-layout.md),
[`docs/horizontal-3-layout.md`](docs/horizontal-3-layout.md), and
[`docs/horizontal-2-layout.md`](docs/horizontal-2-layout.md).

The Figma battery indicator is approved and design locked at revision 1 in the
existing 200 × 20 px reserved strip for both horizontal layouts. Its behavior,
charging animation, and theme rules are documented in
[`docs/battery-indicator.md`](docs/battery-indicator.md).

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
