#include <pebble.h>

#include "layout_horizontal.h"

static Window *s_main_window;

static void main_window_load(Window *window) {
  horizontal_layout_load(window);
}

static void main_window_unload(Window *window) {
  horizontal_layout_unload(window);
}

static void init(void) {
  s_main_window = window_create();
  window_set_background_color(s_main_window, GColorBlack);
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
}

static void deinit(void) {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
