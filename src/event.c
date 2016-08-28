#include <pebble.h>
#include "event.h"
#include "question.h"

typedef struct {} LaunchEventDefinition;

// Using bits: http://stackoverflow.com/a/1631857/429091
typedef struct {
  // This is a launch event
  int type_launch : 1;
  // The question should be launched when this event starts.
  int launch : 1;
} EventFlags;

/*
 * At its core, an event just specifies a question to
 * be asked and indicates when it should be asked.
 */
typedef struct
{
  EventFlags flags;
  QuestionKey question_id;
  union {
    WakeupEventDefinition wakeup;
  } definition;
} Event;

void event_init()
{
}

void event_deinit()
{
}
