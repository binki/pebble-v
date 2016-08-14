#include <pebble.h>
#include "pin_window.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_gothic_28_bold;
static TextLayer *s_textlayer_1;
static TextLayer *s_textlayer_pin;
static TextLayer *s_textlayer_3;

static void initialise_ui(void) {
  s_window = window_create();
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, false);
  #endif
  
  s_res_gothic_28_bold = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
  // s_textlayer_1
  s_textlayer_1 = text_layer_create(GRect(20, 20, 100, 20));
  text_layer_set_text(s_textlayer_1, "PIN:");
  text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
  
  // s_textlayer_pin
  s_textlayer_pin = text_layer_create(GRect(20, 43, 100, 42));
  text_layer_set_text(s_textlayer_pin, "PIN");
  text_layer_set_text_alignment(s_textlayer_pin, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_pin, s_res_gothic_28_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_pin);
  
  // s_textlayer_3
  s_textlayer_3 = text_layer_create(GRect(20, 88, 100, 20));
  text_layer_set_text(s_textlayer_3, "Visit");
  text_layer_set_text_alignment(s_textlayer_3, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_3);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_textlayer_1);
  text_layer_destroy(s_textlayer_pin);
  text_layer_destroy(s_textlayer_3);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
  pin_window_shown = 0;
}

static short pin_window_shown = 0;

void show_pin_window(const char *pin) {
  if (!pin_window_shown) {
    initialise_ui();
    window_set_window_handlers(s_window, (WindowHandlers) {
      .unload = handle_window_unload,
    });
    window_stack_push(s_window, true);
    pin_window_shown = 1;
  }
  text_layer_set_text(s_textlayer_pin, pin);
}

void hide_pin_window(void) {
  window_stack_remove(s_window, true);
}
