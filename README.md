# Redux

Redux is a Pebble Time 2 watchface for the native 200 × 228 Emery canvas.

## Current release

**Redux 2.1.2** is the current production release.

- Store UUID: `7d9c2cc1-24e3-4e4e-9693-5c0f8e4b1e10`
- Target platform: `emery`
- Capabilities: configurable, health
- Source of truth: `main`
- Production app-config: `lyle-morris/Hosting/apps/redux/`

Release documentation:

- [`docs/release-2.1.2-handoff.md`](docs/release-2.1.2-handoff.md)
- [`docs/release-2.1.2-notes.md`](docs/release-2.1.2-notes.md)

## Layouts

Redux supports four layouts:

- `horizontal_2`
- `horizontal_3`
- `vertical_2`
- `vertical_3`

The established geometry contracts are documented in:

- [`docs/horizontal-2-layout.md`](docs/horizontal-2-layout.md)
- [`docs/horizontal-3-layout.md`](docs/horizontal-3-layout.md)
- [`docs/vertical-3-layout.md`](docs/vertical-3-layout.md)

The battery indicator design and behavior are documented in [`docs/battery-indicator.md`](docs/battery-indicator.md).

## Configuration

The companion opens the versioned production configuration endpoint hosted in the `lyle-morris/Hosting` repository. Production config files, assets, localization, and Analytics are owned under `apps/redux/`.

The persisted companion settings key remains `redux_qa_settings_v2` intentionally for settings continuity/migration. Do not rename it casually.

## Assets

Production icons live under `resources/images/icons/`, grouped by canvas size and metric. Redux uses paired `white_` and `reverse_` monochrome artwork; panel color comes from the theme.

Vector masters belong in `assets/icons/source/`.

## Development

Create a new feature or release branch from `main` for future work. Do not continue development on retired 2.1.x QA/backup branches.
