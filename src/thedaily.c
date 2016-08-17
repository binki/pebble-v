#include <pebble.h>
#include "activity.h"
#include "yesno_question.h"
#include "pin_window.h"

// Write message to buffer & send
static void access_token_send(){
	DictionaryIterator *iter;
  char access_token[PERSIST_DATA_MAX_LENGTH] = "";

  persist_read_string(MESSAGE_KEY_ACCESS_TOKEN, access_token, sizeof(access_token));
	
	app_message_outbox_begin(&iter);
	dict_write_cstring(iter, MESSAGE_KEY_ACCESS_TOKEN, access_token);
	
	dict_write_end(iter);
  app_message_outbox_send();
}

// Called when a message is received from PebbleKitJS
static void in_received_handler(DictionaryIterator *iterator, void *context) {
  Tuple *tuple = dict_read_first(iterator);
  while (tuple) {
    if (tuple->key == MESSAGE_KEY_ACCESS_TOKEN) {
        /* Is the app setting or requesting? */
        if (tuple->value->cstring[0]) {
          /* set */
          persist_write_string(MESSAGE_KEY_ACCESS_TOKEN, tuple->value->cstring);
        } else {
          /* request */
          access_token_send();
        }
    } else if (tuple->key == MESSAGE_KEY_PIN) {
        show_pin_window(tuple->value->cstring);
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

static void handle_yesno_question_hidden_callback(yesno_question_answer answer) {
	DictionaryIterator *iter;
	
	app_message_outbox_begin(&iter);
	dict_write_uint8(iter, MESSAGE_KEY_PIN, answer);
	
	dict_write_end(iter);
	app_message_outbox_send();

  // Now pause for a minute and redisplay?
}

void init(void) {
	// Register AppMessage handlers
	app_message_register_inbox_received(in_received_handler); 
	app_message_register_inbox_dropped(in_dropped_handler); 
	app_message_register_outbox_failed(out_failed_handler);
		
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	
	show_yesno_question("This is my question", handle_yesno_question_hidden_callback);
}

void deinit(void) {
	app_message_deregister_callbacks();
}

int main( void ) {
	init();
	app_event_loop();
	deinit();
}
