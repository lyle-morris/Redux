#include <pebble.h>

#include "layout_horizontal.h"
#include "layout_two_slot.h"
#include "layout_vertical.h"

typedef enum {
  ReduxLayoutHorizontalTwo = 0,
  ReduxLayoutHorizontalThree = 1,
  ReduxLayoutVerticalThree = 2,
} ReduxLayout;

enum { PersistKeyLayout = 100 };

static Window *s_main_window;
static ReduxLayout s_layout = ReduxLayoutVerticalThree;
static bool s_layout_loaded;

static ReduxLayout valid_layout(int32_t value) {
  if(value < ReduxLayoutHorizontalTwo || value > ReduxLayoutVerticalThree) {
    return ReduxLayoutVerticalThree;
  }
  return (ReduxLayout)value;
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
    case ReduxLayoutVerticalThree:
    default:
      vertical_layout_load(window);
      APP_LOG(APP_LOG_LEVEL_INFO, "Redux layout loaded: vertical_3");
      break;
  }
  s_layout_loaded = true;
}

static void unload_active_layout(Window *window) {
  if(!s_layout_loaded) return;
  switch(s_layout) {
    case ReduxLayoutHorizontalTwo: two_slot_layout_unload(window); break;
    case ReduxLayoutHorizontalThree: horizontal_layout_unload(window); break;
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
  Tuple *layout_tuple = dict_find(iterator, MESSAGE_KEY_layout);
  if(layout_tuple) apply_layout(valid_layout(layout_tuple->value->int32));

  Tuple *theme_mode = dict_find(iterator, MESSAGE_KEY_theme_mode);
  Tuple *theme = dict_find(iterator, MESSAGE_KEY_theme);
  Tuple *battery = dict_find(iterator, MESSAGE_KEY_show_battery_indicator);
  APP_LOG(APP_LOG_LEVEL_INFO,
          "Redux settings received: layout=%d theme_mode=%d theme=%d battery=%d",
          (int)s_layout,
          theme_mode ? (int)theme_mode->value->int32 : -1,
          theme ? (int)theme->value->int32 : -1,
          battery ? (int)battery->value->int32 : -1);
}

static void inbox_dropped_handler(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Redux settings dropped: %d", (int)reason);
}

static void main_window_load(Window *window) { load_active_layout(window); }
static void main_window_unload(Window *window) { unload_active_layout(window); }

static void init(void) {
  if(persist_exists(PersistKeyLayout)) {
    s_layout = valid_layout(persist_read_int(PersistKeyLayout));
  }
  app_message_register_inbox_received(inbox_received_handler);
  app_message_register_inbox_dropped(inbox_dropped_handler);
  app_message_open(512, 128);

  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorBlack);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
}

static void deinit(void) {
  app_message_deregister_callbacks();
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
