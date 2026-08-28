#include <pebble.h>

#include "layout_horizontal.h"
#include "layout_two_slot.h"
#include "layout_vertical.h"
#include "redux_settings.h"
#include "redux_heart_rate.h"
#include "redux_weather.h"

typedef enum {
  ReduxLayoutHorizontalTwo = 0,
  ReduxLayoutHorizontalThree = 1,
  ReduxLayoutVerticalThree = 2,
  ReduxLayoutVerticalTwo = 3,
} ReduxLayout;

enum {
  PersistKeyLayout = 100,
  PersistKeyLanguage = 101,
  PersistKeySettings = 102,
};

#define BLUETOOTH_SIZE 14
#define BLUETOOTH_MARGIN 4

static Window *s_main_window;
static ReduxLayout s_layout = ReduxLayoutHorizontalTwo;
static bool s_layout_loaded;
static BitmapLayer *s_bluetooth_layer;
static GBitmap *s_bluetooth_bitmap;
static bool s_phone_connected;

static ReduxLayout valid_layout(int32_t value) {
  if(value < ReduxLayoutHorizontalTwo || value > ReduxLayoutVerticalTwo) {
    return ReduxLayoutHorizontalTwo;
  }
  return (ReduxLayout)value;
}

static GRect bluetooth_frame_for_layout(void) {
  switch(s_layout) {
    case ReduxLayoutVerticalTwo:
    case ReduxLayoutVerticalThree:
      return GRect(
        VERTICAL_CANVAS_W - BLUETOOTH_MARGIN - BLUETOOTH_SIZE,
        VERTICAL_TIME_PANEL_Y + VERTICAL_TIME_PANEL_H - BLUETOOTH_MARGIN - BLUETOOTH_SIZE,
        BLUETOOTH_SIZE,
        BLUETOOTH_SIZE
      );
    case ReduxLayoutHorizontalTwo:
      return GRect(
        TWO_SLOT_TIME_PANEL_X + TWO_SLOT_TIME_PANEL_W - BLUETOOTH_MARGIN - BLUETOOTH_SIZE,
        TWO_SLOT_TIME_PANEL_Y + TWO_SLOT_TIME_PANEL_H - BLUETOOTH_MARGIN - BLUETOOTH_SIZE,
        BLUETOOTH_SIZE,
        BLUETOOTH_SIZE
      );
    case ReduxLayoutHorizontalThree:
    default:
      return GRect(
        HORIZONTAL_TIME_PANEL_X + HORIZONTAL_TIME_PANEL_W - BLUETOOTH_MARGIN - BLUETOOTH_SIZE,
        HORIZONTAL_TIME_PANEL_Y + HORIZONTAL_TIME_PANEL_H - BLUETOOTH_MARGIN - BLUETOOTH_SIZE,
        BLUETOOTH_SIZE,
        BLUETOOTH_SIZE
      );
  }
}

static void sanitize_settings(void) {
  g_redux_settings.slot_metric[0] = redux_valid_metric(g_redux_settings.slot_metric[0], ReduxMetricCalendar);
  g_redux_settings.slot_metric[1] = redux_valid_metric(g_redux_settings.slot_metric[1], ReduxMetricWeather);
  g_redux_settings.slot_metric[2] = redux_valid_metric(g_redux_settings.slot_metric[2], ReduxMetricNone);
  g_redux_settings.language = redux_valid_language(g_redux_settings.language);
  g_redux_settings.theme_mode = g_redux_settings.theme_mode ? 1 : 0;
  if(g_redux_settings.theme > 9) g_redux_settings.theme = 0;
  if(g_redux_settings.theme == 6) g_redux_settings.theme = 5;
}

static void persist_settings(void) {
  persist_write_data(PersistKeySettings, &g_redux_settings, sizeof(g_redux_settings));
}

static void update_bluetooth_overlay(void) {
  if(!s_bluetooth_layer) return;
  layer_set_hidden(
    bitmap_layer_get_layer(s_bluetooth_layer),
    !g_redux_settings.show_bluetooth || !s_phone_connected
  );
}

static void raise_bluetooth_overlay(void) {
  if(!s_bluetooth_layer || !s_main_window) return;
  Layer *layer = bitmap_layer_get_layer(s_bluetooth_layer);
  layer_set_frame(layer, bluetooth_frame_for_layout());
  layer_remove_from_parent(layer);
  layer_add_child(window_get_root_layer(s_main_window), layer);
  update_bluetooth_overlay();
}

static void bluetooth_connection_handler(bool connected) {
  s_phone_connected = connected;
  update_bluetooth_overlay();
}

static void bluetooth_overlay_load(Window *window) {
  if(s_bluetooth_layer) return;
  s_bluetooth_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLUETOOTH_CONNECTED);
  s_bluetooth_layer = bitmap_layer_create(bluetooth_frame_for_layout());
  bitmap_layer_set_background_color(s_bluetooth_layer, GColorClear);
  bitmap_layer_set_compositing_mode(s_bluetooth_layer, GCompOpSet);
  bitmap_layer_set_alignment(s_bluetooth_layer, GAlignCenter);
  bitmap_layer_set_bitmap(s_bluetooth_layer, s_bluetooth_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_bluetooth_layer));
  update_bluetooth_overlay();
}

static void bluetooth_overlay_unload(void) {
  if(s_bluetooth_layer) {
    bitmap_layer_destroy(s_bluetooth_layer);
    s_bluetooth_layer = NULL;
  }
  if(s_bluetooth_bitmap) {
    gbitmap_destroy(s_bluetooth_bitmap);
    s_bluetooth_bitmap = NULL;
  }
}

static void load_active_layout(Window *window) {
  switch(s_layout) {
    case ReduxLayoutHorizontalTwo:
      two_slot_layout_load(window);
      APP_LOG(APP_LOG_LEVEL_INFO, "Redux layout loaded: horizontal_2");
      break;
    case ReduxLayoutHorizontalThree:
      horizontal_layout_load(window);
      APP_LOG(APP_LOG_LEVEL_INFO, "Redux layout loaded: horizontal_3");
      break;
    case ReduxLayoutVerticalTwo:
      vertical_layout_load(window);
      APP_LOG(APP_LOG_LEVEL_INFO, "Redux layout loaded: vertical_2");
      break;
    case ReduxLayoutVerticalThree:
    default:
      vertical_layout_load(window);
      APP_LOG(APP_LOG_LEVEL_INFO, "Redux layout loaded: vertical_3");
      break;
  }
  s_layout_loaded = true;
  raise_bluetooth_overlay();
}

static void unload_active_layout(Window *window) {
  if(!s_layout_loaded) return;
  redux_heart_rate_sampling_reset();
  switch(s_layout) {
    case ReduxLayoutHorizontalTwo: two_slot_layout_unload(window); break;
    case ReduxLayoutHorizontalThree: horizontal_layout_unload(window); break;
    case ReduxLayoutVerticalTwo:
    case ReduxLayoutVerticalThree:
    default: vertical_layout_unload(window); break;
  }
  s_layout_loaded = false;
}

static void apply_layout(ReduxLayout layout) {
  if(layout == s_layout || !s_main_window || !s_layout_loaded) return;
  unload_active_layout(s_main_window);
  s_layout = layout;
  persist_write_int(PersistKeyLayout, s_layout);
  load_active_layout(s_main_window);
}

static void inbox_received_handler(DictionaryIterator *iterator, void *context) {
  ReduxLayout requested_layout = s_layout;
  Tuple *layout_tuple = dict_find(iterator, MESSAGE_KEY_layout);
  if(layout_tuple) requested_layout = valid_layout(layout_tuple->value->int32);

#define READ_U8(key, field) do { Tuple *t = dict_find(iterator, MESSAGE_KEY_##key); if(t) g_redux_settings.field = (uint8_t)t->value->int32; } while(0)
#define READ_BOOL(key, field) do { Tuple *t = dict_find(iterator, MESSAGE_KEY_##key); if(t) g_redux_settings.field = t->value->int32 != 0; } while(0)
#define READ_COLOR(key, field) do { Tuple *t = dict_find(iterator, MESSAGE_KEY_##key); if(t) g_redux_settings.field = (uint32_t)t->value->int32; } while(0)
  READ_U8(slot_1_metric, slot_metric[0]);
  READ_U8(slot_2_metric, slot_metric[1]);
  READ_U8(slot_3_metric, slot_metric[2]);
  READ_U8(theme_mode, theme_mode); READ_U8(theme, theme);
  READ_BOOL(show_battery_indicator, show_battery_indicator);
  READ_BOOL(show_leading_zero, show_leading_zero); READ_BOOL(hour24, hour24);
  READ_BOOL(celsius, celsius); READ_BOOL(show_bluetooth, show_bluetooth);
  READ_U8(language, language);
  READ_COLOR(watchface_background, watchface_background); READ_COLOR(box_background_color, box_background);
  READ_COLOR(box_top_border_color, box_top_border); READ_COLOR(box_bottom_border_color, box_bottom_border);
  READ_COLOR(tray_background_color, tray_background); READ_COLOR(divider_color, divider);
  READ_COLOR(time_text_color, time_text); READ_COLOR(date_text_color, date_text);
  READ_COLOR(slot_1_text_color, slot_text[0]); READ_COLOR(slot_2_text_color, slot_text[1]);
  READ_COLOR(slot_3_text_color, slot_text[2]); READ_COLOR(battery_indicator_color, battery_indicator);
#undef READ_U8
#undef READ_BOOL
#undef READ_COLOR

  redux_weather_handle_message(iterator);
  sanitize_settings();
  persist_write_int(PersistKeyLanguage, g_redux_settings.language);
  persist_settings();

  if(requested_layout != s_layout) {
    apply_layout(requested_layout);
  } else if(s_layout_loaded) {
    unload_active_layout(s_main_window);
    load_active_layout(s_main_window);
  }
  update_bluetooth_overlay();

  Tuple *theme_mode = dict_find(iterator, MESSAGE_KEY_theme_mode);
  Tuple *theme = dict_find(iterator, MESSAGE_KEY_theme);
  Tuple *battery = dict_find(iterator, MESSAGE_KEY_show_battery_indicator);
  APP_LOG(APP_LOG_LEVEL_INFO,
          "Redux settings received: layout=%d slots=%d/%d/%d theme_mode=%d theme=%d battery=%d bluetooth=%d language=%d",
          (int)s_layout,
          (int)g_redux_settings.slot_metric[0],
          (int)g_redux_settings.slot_metric[1],
          (int)g_redux_settings.slot_metric[2],
          theme_mode ? (int)theme_mode->value->int32 : -1,
          theme ? (int)theme->value->int32 : -1,
          battery ? (int)battery->value->int32 : -1,
          g_redux_settings.show_bluetooth ? 1 : 0,
          (int)g_redux_settings.language);
}

static void inbox_dropped_handler(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Redux settings dropped: %d", (int)reason);
}

static void main_window_load(Window *window) {
  load_active_layout(window);
  bluetooth_overlay_load(window);
  raise_bluetooth_overlay();
}

static void main_window_unload(Window *window) {
  bluetooth_overlay_unload();
  unload_active_layout(window);
}

static void init(void) {
  redux_settings_set_defaults();

  if(persist_exists(PersistKeySettings) && persist_get_size(PersistKeySettings) == (int)sizeof(g_redux_settings)) {
    persist_read_data(PersistKeySettings, &g_redux_settings, sizeof(g_redux_settings));
    sanitize_settings();
  }
  if(persist_exists(PersistKeyLayout)) {
    s_layout = valid_layout(persist_read_int(PersistKeyLayout));
  }
  if(persist_exists(PersistKeyLanguage)) {
    g_redux_settings.language = redux_valid_language((uint8_t)persist_read_int(PersistKeyLanguage));
  }

  s_phone_connected = connection_service_peek_pebble_app_connection();
  connection_service_subscribe((ConnectionHandlers) {
    .pebble_app_connection_handler = bluetooth_connection_handler
  });

  app_message_register_inbox_received(inbox_received_handler);
  app_message_register_inbox_dropped(inbox_dropped_handler);
  app_message_open(512, 128);
  redux_weather_init();

  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorBlack);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
}

static void deinit(void) {
  redux_weather_deinit();
  redux_heart_rate_sampling_reset();
  connection_service_unsubscribe();
  app_message_deregister_callbacks();
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
