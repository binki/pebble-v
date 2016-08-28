#include <pebble.h>
#include "composition.h"
#include "event.h"
#include "pin_window.h"

// Called when a message is received from PebbleKitJS
static void in_received_handler(DictionaryIterator *iterator, void *context) {
  Tuple *tuple = dict_read_first(iterator);
  while (tuple) {
    if (tuple->key == MESSAGE_KEY_PIN) {
      // Receiving empty string indicates should no longer display.
      // (Seems that API does not support the concept of null/NULL).
      if (tuple->value->cstring[0]) {
        show_pin_window(tuple->value->cstring);
      } else {
        hide_pin_window();
      }
    }
    tuple = dict_read_next(iterator);
  }
}

// Called when an incoming message from PebbleKitJS is dropped
static void in_dropped_handler(AppMessageResult reason, void *context) {	
}

// Called when PebbleKitJS does not acknowledge receipt of a message
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
}

void init(void) {
  composition_init();
	// Register AppMessage handlers
	app_message_register_inbox_received(in_received_handler); 
	app_message_register_inbox_dropped(in_dropped_handler);
	app_message_register_outbox_failed(out_failed_handler);

  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

  event_init();

  // Now do launch reason-specific action
  switch (launch_reason())
  {
    case APP_LAUNCH_WAKEUP:
      // Launch the question.
      break;
    case APP_LAUNCH_USER:
    default:
      // Launch the menu.
      break;
  }
}

void deinit(void) {
	app_message_deregister_callbacks();
  event_deinit();
  composition_deinit();
}

int main( void ) {
	init();
	app_event_loop();
	deinit();
}
