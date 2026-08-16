# Redux

Pebble Time 2 watchface project for the Redux layouts.

## Current build

The first runnable QA target is the Figma-specified `vertical_3` layout. It intentionally renders static reference values so geometry can be approved with an emulator overlay before live data and configuration are connected.

See [`docs/vertical-3-layout.md`](docs/vertical-3-layout.md) for the exact pixel contract.

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
