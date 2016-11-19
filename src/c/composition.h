#pragma once
#include <pebble.h>

extern void composition_init();
extern void composition_deinit();


extern void composition_app_message_register_handler(uint32_t key, AppMessageInboxReceived handler, void *context);
extern void composition_app_message_unregister_handler(uint32_t key);
