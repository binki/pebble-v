#include <pebble.h>
#include "composition.h"
#include "def.h"
#include "v.h"

static V message_handlers;

typedef struct {
  uint32_t key;
  AppMessageInboxReceived handler;
  void *context;
} MessageHandler;

void composition_init()
{
  v_init(&message_handlers, sizeof(MessageHandler));
}

void composition_deinit()
{
  v_deinit(&message_handlers);
}

void composition_app_message_register_handler(uint32_t key, AppMessageInboxReceived handler, void *context)
{
  MessageHandler buf = {
    .key = key,
    .handler = handler,
    .context = context,
  };
  if (v_add(&message_handlers, &buf)) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Unable to add message handler for key %d", key);
    exit(1);
  }
}

static int cmp_message_handler_by_key(MessageHandler *a, MessageHandler *b) {
  return (a->key - b->key) % DAILY_INT_MAX;
}

void composition_app_message_unregister_handler(uint32_t key)
{
  
}
