# Redux 2.1.2 Release Notes

Redux 2.1.2 is a Pebble Time 2 release focused on the completed Redux layout system, expanded metrics, production configuration, localization, Analytics, and release hardening.

## What’s included

- Horizontal and vertical layouts
- 2 or 3 informational slots
- Calendar, weather, battery, calories, activity, sleep, heart rate, steps, and distance metrics
- Manual location support for weather and location-aware time
- 12/24-hour time and leading-zero controls
- Bluetooth and battery indicators
- Preset themes and full Custom theme color controls
- Expanded language support
- Pebble Health integration
- Updated icons, spacing, typography, and pixel-level layout polish
- Redesigned production app-config experience
- GA4 usage tracking for settings, layouts, themes, and Custom-theme color usage

## Fixes and release hardening

- Corrected App Store UUID and release versioning
- Improved settings persistence and startup behavior
- Replaced placeholder health values with live data
- Activity and sleep display as `HH:MM`
- Improved Bluetooth placement
- Corrected reverse-icon theme behavior
- Improved location, input clear/reset, and language behavior
- Fixed stale Pebble WebView configuration caching with a versioned no-cache entrypoint
- Isolated production app-config from QA files
- Isolated Redux localization from Essential Redux QA
- Restored GA4 Realtime tracking after production-config isolation
- Added `theme_used` reporting for preset and Custom themes
- Added actual Custom Pebble color values to Analytics
- Fixed a Custom-theme round-trip bug that could reset Time box or slot text colors after changing an unrelated setting

## Hosted 2.1.2 hotfix state

The production app-config may receive hosted fixes without a PBW rebuild. Current production build token at closeout:

`redux-2.1.2-prod-20260829h`

The final Custom-theme persistence hotfix was deployed through Hosting and confirmed by QA.

## Known non-blocking items

- Build still reports the known `enableMultiJS` disabled notice.
- Linker still reports the known RWX LOAD-segment warning.
- Companion still has a hard-coded fallback weather coordinate when device geolocation fails; this should be removed in a future release.
- Production app-config is isolated from QA, but the loader still contains compatibility rewriting for historical QA/Essential Redux references. A future refactor should remove those transforms directly.

For full release architecture, issue history, blockers, and future QA requirements, see [`release-2.1.2-handoff.md`](release-2.1.2-handoff.md).
