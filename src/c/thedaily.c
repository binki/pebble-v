#include <pebble.h>
#include "composition.h"
#include "event.h"
#include "main_menu.h"
#include "pin_window.h"

void init(void) {
  APP_LOG(APP_LOG_LEVEL_INFO, "01");
  composition_init();
  APP_LOG(APP_LOG_LEVEL_INFO, "02");

  event_init();
  APP_LOG(APP_LOG_LEVEL_INFO, "03");

  // Now do launch reason-specific action
  switch (launch_reason())
  {
    case APP_LAUNCH_WAKEUP:
      // Launch the question.
      break;
    case APP_LAUNCH_USER:
    default:
      // Launch the menu.
      main_menu_show();
      break;
  }
  APP_LOG(APP_LOG_LEVEL_INFO, "04");

  // Open after initializing everything else so that all handlers are ready.
  // To manage small message sizes, we’ll do streaming and use versioning
  // to incrementally store data into storage and switch when streaming is
  // done.
  int in_max = app_message_inbox_size_maximum();
  if (in_max > 512) {
    in_max = 512;
  }
  int out_max = app_message_outbox_size_maximum();
  if (out_max > 512) {
    out_max = 512;
  }
  app_message_open(in_max, out_max);
  APP_LOG(APP_LOG_LEVEL_INFO, "05");
}

void deinit(void) {
	app_message_deregister_callbacks();
  main_menu_hide();
  event_deinit();
  composition_deinit();
}

int main( void ) {
	init();
	app_event_loop();
	deinit();
}
