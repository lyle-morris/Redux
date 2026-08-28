#include "redux_settings.h"
#include <stdio.h>

ReduxSettings g_redux_settings;

enum {
  ReduxThemeOrange = 0,
  ReduxThemeBlue = 1,
  ReduxThemePurple = 2,
  ReduxThemeYellow = 3,
  ReduxThemeGreen = 4,
  ReduxThemeRed = 5,
  ReduxThemePink = 6,
  ReduxThemeBrown = 7,
  ReduxThemeWhite = 8,
  ReduxThemeBlack = 9,
};

static const char * const s_month_labels[ReduxLanguageCount][12] = {
  { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" },
  { "Ene", "Feb", "Mar", "Abr", "May", "Jun", "Jul", "Ago", "Sep", "Oct", "Nov", "Dic" },
  { "Jan", "Fév", "Mar", "Avr", "Mai", "Jun", "Jul", "Aoû", "Sep", "Oct", "Nov", "Déc" },
  { "Jan", "Feb", "Mär", "Apr", "Mai", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Dez" },
  { "Jan", "Fev", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez" },
  { "Gen", "Feb", "Mar", "Apr", "Mag", "Giu", "Lug", "Ago", "Set", "Ott", "Nov", "Dic" },
  { "Jan", "Feb", "Mrt", "Apr", "Mei", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Dec" },
  { "Jan", "Feb", "Mar", "Apr", "Maj", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Dec" },
  { "Jan", "Feb", "Mar", "Apr", "Mai", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Des" },
  { "Jan", "Feb", "Mar", "Apr", "Maj", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Dec" },
  { "Tam", "Hel", "Maa", "Huh", "Tou", "Kes", "Hei", "Elo", "Syy", "Lok", "Mar", "Jou" },
  { "Jan", "Feb", "Mar", "Apr", "Maí", "Jún", "Júl", "Ágú", "Sep", "Okt", "Nóv", "Des" },
  { "Gen", "Feb", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Oct", "Nov", "Des" },
  { "Urt", "Ots", "Mar", "Api", "Mai", "Eka", "Uzt", "Abu", "Ira", "Urr", "Aza", "Abe" },
  { "Xan", "Feb", "Mar", "Abr", "Mai", "Xuñ", "Xul", "Ago", "Set", "Out", "Nov", "Dec" },
  { "Sty", "Lut", "Mar", "Kwi", "Maj", "Cze", "Lip", "Sie", "Wrz", "Paź", "Lis", "Gru" },
  { "Led", "Úno", "Bře", "Dub", "Kvě", "Čvn", "Čvc", "Srp", "Zář", "Říj", "Lis", "Pro" },
  { "Jan", "Feb", "Mar", "Apr", "Máj", "Jún", "Júl", "Aug", "Sep", "Okt", "Nov", "Dec" },
  { "Jan", "Feb", "Mar", "Apr", "Maj", "Jun", "Jul", "Avg", "Sep", "Okt", "Nov", "Dec" },
  { "Sij", "Vel", "Ožu", "Tra", "Svi", "Lip", "Srp", "Kol", "Ruj", "Lis", "Stu", "Pro" },
  { "Jan", "Feb", "Mar", "Apr", "Maj", "Jun", "Jul", "Avg", "Sep", "Okt", "Nov", "Dec" },
  { "Jan", "Feb", "Mar", "Apr", "Maj", "Jun", "Jul", "Avg", "Sep", "Okt", "Nov", "Dec" },
  { "Ian", "Feb", "Mar", "Apr", "Mai", "Iun", "Iul", "Aug", "Sep", "Oct", "Noi", "Dec" },
  { "Jan", "Feb", "Már", "Ápr", "Máj", "Jún", "Júl", "Aug", "Sze", "Okt", "Nov", "Dec" },
  { "Jan", "Veb", "Mär", "Apr", "Mai", "Jun", "Jul", "Aug", "Sep", "Okt", "Nov", "Det" },
  { "Jan", "Feb", "Mar", "Apr", "Mai", "Jūn", "Jūl", "Aug", "Sep", "Okt", "Nov", "Dec" },
  { "Sau", "Vas", "Kov", "Bal", "Geg", "Bir", "Lie", "Rgp", "Rgs", "Spa", "Lap", "Grd" },
  { "Oca", "Şub", "Mar", "Nis", "May", "Haz", "Tem", "Ağu", "Eyl", "Eki", "Kas", "Ara" },
  { "Jan", "Shk", "Mar", "Pri", "Maj", "Qer", "Kor", "Gus", "Sht", "Tet", "Nën", "Dhj" },
  { "Jan", "Fra", "Mar", "Apr", "Mej", "Gun", "Lul", "Aww", "Set", "Ott", "Nov", "Dic" }
};

static uint8_t valid_theme(void) {
  if(g_redux_settings.theme == ReduxThemePink) return ReduxThemeRed;
  return g_redux_settings.theme < 10 ? g_redux_settings.theme : ReduxThemeOrange;
}

uint8_t redux_valid_metric(uint8_t metric, uint8_t fallback) {
  return metric <= ReduxMetricNone ? metric : fallback;
}

uint8_t redux_valid_language(uint8_t language) {
  return language < ReduxLanguageCount ? language : ReduxLanguageEnglish;
}

void redux_settings_set_defaults(void) {
  g_redux_settings = (ReduxSettings) {
    .theme_mode = 0, .theme = ReduxThemeOrange,
    .slot_metric = {ReduxMetricCalendar, ReduxMetricWeather, ReduxMetricNone},
    .show_battery_indicator = true,
    .show_leading_zero = true, .hour24 = false, .celsius = false,
    .show_bluetooth = false,
    .language = ReduxLanguageEnglish,
    .watchface_background = 0x000000, .box_background = 0xffffff,
    .box_top_border = 0x000000, .box_bottom_border = 0x000000,
    .tray_background = 0xff5500, .divider = 0x000000,
    .time_text = 0x000000, .date_text = 0x000000,
    .slot_text = {0x000000, 0x000000, 0x000000},
    .battery_indicator = 0xff5500
  };
}

void redux_format_calendar_label(struct tm *tick_time, char *buffer, size_t size) {
  if (!tick_time || !buffer || size == 0) return;
  uint8_t language = redux_valid_language(g_redux_settings.language);
  int month = tick_time->tm_mon >= 0 && tick_time->tm_mon < 12 ? tick_time->tm_mon : 0;
  snprintf(buffer, size, "%s", s_month_labels[language][month]);
}

void redux_format_localized_date(struct tm *tick_time, char *buffer, size_t size) {
  if (!tick_time || !buffer || size == 0) return;
  uint8_t language = redux_valid_language(g_redux_settings.language);
  int month = tick_time->tm_mon >= 0 && tick_time->tm_mon < 12 ? tick_time->tm_mon : 0;
  snprintf(buffer, size, "%s %d", s_month_labels[language][month], tick_time->tm_mday);
}

GColor redux_color(uint32_t value) { return GColorFromHEX(value & 0xffffff); }

GColor redux_preset_color(void) {
  static const uint32_t colors[] = {
    0xff5500, 0x00aaff, 0x5500ff, 0xffaa00, 0x55aa00,
    0xff0000, 0xff0055, 0xaa5500, 0xffffff, 0x000000
  };
  return redux_color(colors[valid_theme()]);
}

GColor redux_preset_divider_color(void) {
  return valid_theme() == ReduxThemeBlack ? GColorWhite : GColorBlack;
}

GColor redux_preset_time_panel_color(void) {
  return valid_theme() == ReduxThemeBlack ? GColorBlack : GColorWhite;
}

GColor redux_preset_time_text_color(void) {
  return valid_theme() == ReduxThemeBlack ? GColorWhite : GColorBlack;
}

GColor redux_preset_slot_text_color(void) {
  uint8_t theme = valid_theme();
  return (theme == ReduxThemePurple || theme == ReduxThemeRed || theme == ReduxThemeBlack)
    ? GColorWhite
    : GColorBlack;
}

GColor redux_preset_battery_color(void) {
  return valid_theme() == ReduxThemeBlack ? GColorWhite : redux_preset_color();
}

bool redux_preset_uses_reverse_icons(void) {
  uint8_t theme = valid_theme();
  return theme == ReduxThemePurple || theme == ReduxThemeBlack;
}

GColor redux_contrast_color(GColor color) {
  return (color.argb == GColorWhite.argb || color.argb == GColorYellow.argb || color.argb == GColorChromeYellow.argb || color.argb == GColorCyan.argb) ? GColorBlack : GColorWhite;
}
