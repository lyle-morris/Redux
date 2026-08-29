# Redux 2.1.2 Release Handoff

Updated: August 29, 2026

## Release status

Redux 2.1.2 is the production release for Pebble Time 2 / Emery. Watchface and companion source are on `main`. The final watch/companion release source was validated at commit `a3da435e1cfb0a61a9e5a852dd61a9605a212077`; documentation-only commits follow that commit on `main`.

Production app-config is hosted separately in `lyle-morris/Hosting` under `apps/redux/` and can receive hosted fixes without rebuilding the PBW, provided the versioned entrypoint remains compatible with the installed companion.

## Repository boundaries

### Watchface / companion

- Repository: `lyle-morris/Redux`
- Production branch: `main`
- Store UUID: `7d9c2cc1-24e3-4e4e-9693-5c0f8e4b1e10`
- App name: `Redux`
- Version: `2.1.2`
- Target: `emery`
- Capabilities: `configurable`, `health`
- Companion: `src/pkjs/index.js`

### Hosted configuration

- Repository: `lyle-morris/Hosting`
- Production branch: `main`
- Production directory: `apps/redux/`
- Versioned entrypoint: `apps/redux/app-config-2.1.2.html`
- Production loader: `apps/redux/app-config.html`
- Production base: `apps/redux/app-config-base.html`
- Production patch: `apps/redux/app-config-patch.js`
- Production Analytics: `apps/redux/app-config-analytics.js`
- Production localization snapshot: `apps/redux/app-config-i18n-source.html`
- Production images: `apps/redux/images/`
- Current hosted build token: `redux-2.1.2-prod-20260829h`

The companion opens `app-config-2.1.2.html`; that entrypoint redirects into the production loader with a cache-busting build parameter.

## Validated build

Final validated PBW build used for 2.1.2:

- Artifact: `build/tmpjycf9ndq.pbw` (temporary CloudPebble build filename)
- Target: Emery / Pebble Time 2
- Resources: 84,217 bytes / 256 KB
- RAM footprint: 18,000 bytes / 128 KB
- Free heap: 113,072 bytes

Known build notices were accepted as non-blocking:

- `enableMultiJS` disabled notice
- ELF LOAD segment with RWX permissions
- Pebble Tool update notice (tooling update only)

## Runtime configuration

### Layouts

- `horizontal_2`
- `horizontal_3`
- `vertical_2`
- `vertical_3`

Native canvas: 200 × 228.

### Metrics

- Calendar
- Weather
- Battery
- Calories
- Activity
- Sleep
- Heart rate
- Steps
- Distance

Activity and sleep display as `HH:MM`.

### Time / date

- 12/24-hour support
- Leading-zero setting independent of 12/24-hour mode
- Month label is used; weekday-label option was removed for this release
- Manual location can provide location-aware time via weather timezone offset

### Weather / location

- Open-Meteo forecast + geocoding
- 15-minute watch-request schedule
- Manual postal code or city supported
- Postal fallback uses Zippopotam for US/CA when needed
- Device geolocation used when manual location is not configured

Known technical debt: the companion still has a hard-coded fallback coordinate (`34.0232, -84.3616`) when geolocation is unavailable/fails. A future release should prefer an unavailable-weather state instead of silently showing weather for an unrelated location.

### Current release defaults

The current 2.1.2 runtime/config source resolves to:

- Layout: horizontal 2-slot
- Slot 1: Calendar
- Slot 2: Weather
- Theme: Orange preset
- Leading zero: on
- 12-hour format: on
- Celsius: off
- Bluetooth indicator: off
- Battery indicator: on
- Language: English
- Analytics: enabled

If product defaults are changed in a future release, update both companion defaults and hosted config defaults together.

## AppMessage contract

| Key | Setting |
| ---: | --- |
| 0 | layout |
| 1 | slot_1_metric |
| 2 | slot_2_metric |
| 3 | slot_3_metric |
| 4 | theme_mode |
| 5 | theme |
| 6 | show_battery_indicator |
| 7 | watchface_background |
| 8 | box_background_color |
| 9 | box_top_border_color |
| 10 | box_bottom_border_color |
| 11 | tray_background_color |
| 12 | divider_color |
| 13 | time_text_color |
| 14 | date_text_color |
| 15 | slot_1_text_color |
| 16 | slot_2_text_color |
| 17 | slot_3_text_color |
| 18 | battery_indicator_color |
| 19 | js_ready |
| 20 | show_leading_zero |
| 21 | hour24 |
| 22 | celsius |
| 23 | show_bluetooth |
| 24 | language |
| 25 | analytics_enabled |
| 26 | weather_temp |
| 27 | location_time_offset |
| 28 | use_location_time |
| 29 | request_weather |

Key 26 was reused from the old weekday-toggle contract and now carries weather temperature.

## Settings persistence

`SETTINGS_KEY` remains `redux_qa_settings_v2` intentionally. The name is historical, but changing it would break settings continuity/migration for installed users. Do not rename it without an explicit migration plan.

## Analytics

GA4 Measurement ID: `G-37VYMTXT5S`

Production Analytics is loaded from `apps/redux/app-config-analytics.js` only when anonymous analytics is enabled.

Tracked events include:

- `page_view`
- `config_open`
- `analytics_enabled`
- `settings_saved`
- `theme_used`
- `theme_selected`
- `layout_changed`
- `layout_reset`
- `donation_clicked`

`theme_used` records the theme actually in use on config open and settings save. Parameters include theme name/mode, orientation, slot count, and source.

For Custom themes, `theme_used` also records actual Pebble color values (hex without `#`), including the applicable background, panel, text, divider/border, and battery-indicator colors.

GA4 custom dimensions must be created manually for parameters that need long-term reporting (for example `theme`, `theme_mode`, `background_color`, `panel_color`, `time_text_color`). Realtime can be used immediately to verify events.

## Issues and blockers encountered in 2.1.x

### 1. App Store identity mismatch — resolved

Early release builds used a development UUID instead of the existing Essential Redux / Redux Store UUID. The production UUID is now fixed at:

`7d9c2cc1-24e3-4e4e-9693-5c0f8e4b1e10`

Do not change this UUID for future updates to the existing listing.

### 2. Version progression confusion — resolved

2.1.0 was initially published, then configuration corrections required another upload. 2.1.1 was used during the correction cycle, so the clean republish was moved to 2.1.2 rather than attempting to reuse an already-used release number.

### 3. Stale app-config page / Pebble WebView cache — resolved

The companion originally opened a stable config URL/version token. Pebble's embedded WebView could continue serving an old page even after Hosting changed.

Resolution:

- Dedicated versioned entrypoint `app-config-2.1.2.html`
- Explicit no-cache meta directives
- Cache-busting query/build tokens
- Production loader and asset script tokens incremented when hosted runtime changes are made

When diagnosing a future stale-config report, first verify the exact `Redux opening config:` URL from the companion logs.

### 4. Production config depended on `/qa/` — resolved

The first production endpoint loaded the approved UI from `apps/redux/qa/`. That meant production could change accidentally with QA work.

Resolution: production now owns a frozen base, patch, images, localization snapshot, and Analytics module under `apps/redux/`.

### 5. Localization depended on Essential Redux QA — resolved

The Redux v11 patch historically fetched its translation table from `apps/essential-redux/qa/app-config.html`. That path was later converted into a redirect, which could silently break localization.

Resolution: Redux production now owns `app-config-i18n-source.html`; the production loader redirects the historical localization request to that local snapshot.

Technical debt: the frozen base/patch still contains historical QA/Essential Redux references that are intercepted/re-written by the production loader. A future config refactor should remove those references directly instead of maintaining compatibility transforms.

### 6. Google Analytics stopped reporting Realtime — resolved

When production was isolated from Essential Redux/QA, the old Analytics loader was no longer executed. The Analytics toggle still saved correctly, but no `gtag.js` runtime was being loaded, so Realtime was empty.

Resolution: production-owned `app-config-analytics.js` now loads GA4 directly and emits config/settings events.

### 7. Theme usage visibility was incomplete — resolved

Initial Analytics could report Custom only as `theme=custom`.

Resolution: `theme_used` now reports preset theme names and, for Custom themes, the actual selected Pebble colors.

### 8. Custom Time box / slot text color reset — resolved with hosted hotfix

User report: changing another setting while using a Custom theme could reset the Time box text color.

Root cause: the companion persisted active custom colors as flattened settings (`timeTextColor`, `slot1TextColor`, etc.), but the hosted config's `normalize()` function rebuilt `state.customTheme` without rehydrating those flattened fields. Reopening Settings and saving an unrelated change could therefore write default text colors back over the user's custom values.

Resolution: production loader now rehydrates active orientation Time/slot text colors into `customTheme` before the base config normalizes/render/saves them. Current hosted build token: `redux-2.1.2-prod-20260829h`.

QA confirmation: custom Time box text color persisted after reopening Settings, changing an unrelated option, saving, and reopening again.

This was a hosted config fix; no new PBW was required.

### 9. Repository branch drift — resolved for source of truth

During release audit, Redux `main` was far behind `dev/pixel-perfect-layouts`. `main` was fast-forwarded to the exact 2.1.2 shipping source. Future development should branch from `main`.

### 10. Bluetooth placement / vertical edge — resolved

Vertical Bluetooth placement was pinned to the visible right edge of the 200 px canvas while retaining the correct Time panel bottom offset.

### 11. Theme reverse-icon behavior — resolved

Reverse icons are tied to the supported theme rule rather than arbitrary border changes. Black uses reverse artwork; Purple follows its intended icon/text treatment.

### 12. Settings UX regressions found during QA — resolved

QA found and corrected multiple hosted config issues including:

- ZIP/city clearing and mutual override
- Language reset/clear behavior
- Reset Layout behavior
- Theme reset placement
- Missing sections/settings wiring
- Bluetooth option wiring
- Metrics wiring
- Month-vs-weekday behavior
- Language support/default ordering
- Battery indicator/color-row inconsistencies
- Theme tab/panel corner radii and separator polish

## Production app-config architecture warning

Although production no longer executes files directly from `/qa/`, `app-config.html` currently performs compatibility rewrites on the frozen base HTML at runtime. This is stable for 2.1.2, but it is not the ideal long-term architecture.

For the next substantial config release, prefer a clean production source file that directly contains the final normalization, localization, asset, and Analytics paths. That will eliminate string-replacement hotfixes and make regression testing easier.

## Release QA checklist for future updates

Before publishing a new PBW:

1. Confirm Store UUID is unchanged.
2. Confirm `versionLabel` is the intended new version.
3. Confirm target is Emery.
4. Confirm companion config URL/version token.
5. Confirm AppMessage key map is unchanged or migration is documented.
6. Build a fresh PBW and review memory/resource usage.
7. Open Settings from the actual Pebble mobile app.
8. Test all four layouts and 2/3-slot transitions.
9. Test preset and Custom themes.
10. For Custom themes, save → reopen → change an unrelated setting → save → reopen and verify colors persist.
11. Test language switching and reset to English.
12. Test ZIP, city, clearing, and weather/location-time behavior.
13. Confirm GA4 Realtime receives `config_open`, `theme_used`, and `settings_saved` when Analytics is enabled.
14. Confirm production Hosting deployment completed successfully.
15. Only then publish the Store PBW.

## Branch cleanup

At closeout, `main` is the only branch that should be treated as production source. Historical backup/QA branches from the 2.1.x development cycle are not release sources and should not be merged back into `main`.

See the repository branch list at closeout for obsolete `backup/`, `qa/`, and `dev/` branches. Their unique historical commits are superseded by the final release and should be deleted once no longer needed for archaeology.

## Follow-up candidates

Not blockers for 2.1.2, but good targets for the next development cycle:

- Remove arbitrary weather fallback coordinates.
- Refactor production app-config so it no longer relies on runtime string rewriting/interception.
- Rename/migrate the historical `redux_qa_settings_v2` key only if a safe migration is implemented.
- Add automated browser/config regression tests for custom-theme persistence and AppMessage serialization.
- Add a current vertical 2-slot geometry document if layout-specific documentation is expanded.
