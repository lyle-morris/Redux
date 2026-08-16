# Redux Font Upload Guide

Upload the original Roboto Flex font file to this directory:

```text
resources/fonts/
```

## Preferred upload

- Original `.ttf` file
- Roboto Flex license file, if supplied with the download
- Keep the original font filename until validation is complete

A clear preferred filename is:

```text
RobotoFlex-VariableFont.ttf
```

## Validation after upload

Before the font is wired into the watchface, validate:

- File integrity and font metadata
- Supported Roboto Flex axes
- Glyph coverage for every supported language
- Rendering at the specified watchface sizes
- Long localized dates and numeric stress cases
- Pebble build compatibility
- Runtime storage and memory impact

The original variable font is the source asset. If Pebble requires static instances or smaller glyph subsets, derived production files should be generated after the typography settings and supported languages are finalized.
