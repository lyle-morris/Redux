# Redux Font Upload Guide

The original Roboto Flex variable font is stored in this directory:

```text
resources/fonts/
```

## Preferred upload

- Original `.ttf` file
- Roboto Flex license file, if supplied with the download
- Keep the original font filename until validation is complete

Source filename:

```text
RobotoFlex-Variable.ttf
```

## Generated production instances

The vertical three-slot QA build uses pinned ExtraBold instances generated from the source font:

| File | Optical size | Weight | Width | Usage |
| --- | ---: | ---: | ---: | --- |
| `RobotoFlex-ExtraBold-18.ttf` | 18 | 800 | 100 | Tray values and date |
| `RobotoFlex-Bold-20.ttf` | 20 | 700 | 100 | Two-slot tray values |
| `RobotoFlex-ExtraBold-28.ttf` | 28 | 800 | 100 | Two-slot calendar day |
| `RobotoFlex-ExtraBold-62.ttf` | 62 | 800 | 100 | Horizontal time |
| `RobotoFlex-ExtraBold-93.ttf` | 93 | 800 | 100 | Stacked time |

All remaining Roboto Flex axes are pinned to their defaults. These production files are static TrueType fonts; the Pebble manifest applies a character regex to each font resource to limit bundled glyphs.

## Validation after upload

Before the font is wired into the watchface, validate:

- File integrity and font metadata
- Supported Roboto Flex axes
- Glyph coverage for every supported language
- Rendering at the specified watchface sizes
- Localized date stress case `Sept 30 Mer` (no punctuation) and numeric stress cases
- Pebble build compatibility
- Runtime storage and memory impact

The original variable font is the source asset. If Pebble requires static instances or smaller glyph subsets, derived production files should be generated after the typography settings and supported languages are finalized.
