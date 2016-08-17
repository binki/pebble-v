#include <pebble.h>
#include "yesno_question.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static TextLayer *s_textlayer_question;
static ActionBarLayer *s_actionbarlayer_actions;

static void initialise_ui(void) {
  s_window = window_create();

  // s_textlayer_question
  s_textlayer_question = text_layer_create(GRect(19, 20, 100, 129));
  text_layer_set_text(s_textlayer_question, "Question?");
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_question);
  
  // s_actionbarlayer_actions
  s_actionbarlayer_actions = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbarlayer_actions, s_window);
  action_bar_layer_set_background_color(s_actionbarlayer_actions, GColorBlack);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbarlayer_actions);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_textlayer_question);
  action_bar_layer_destroy(s_actionbarlayer_actions);
}
// END AUTO-GENERATED UI CODE

static yesno_question_answer answer;
static void button_down_click_handler(ClickRecognizerRef recognizer, void *context) {
	// User clicked No
	answer = YESNO_QUESTION_ANSWER_NO;
	hide_yesno_question();
}

static void button_up_previous_click_handler(ClickRecognizerRef recognizer, void *context) {
	// User clicked Yes
	answer = YESNO_QUESTION_ANSWER_YES;
	hide_yesno_question();
}

static void (*hidden_callback_)(yesno_question_answer);

static void click_config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_DOWN, button_down_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, button_up_previous_click_handler);
}

static void handle_window_unload(Window* window) {
  destroy_ui();
	hidden_callback_(answer);
}

void show_yesno_question(const char *question_text, void hidden_callback(yesno_question_answer)) {
	initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  text_layer_set_text(s_textlayer_question, question_text);
	action_bar_layer_set_click_config_provider(s_actionbarlayer_actions, click_config_provider);
	answer = YESNO_QUESTION_ANSWER_NONE;
	hidden_callback_ = hidden_callback;
  window_stack_push(s_window, true);
}

void hide_yesno_question(void) {
  window_stack_remove(s_window, true);
}

