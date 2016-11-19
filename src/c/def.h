#include <pebble.h>

// In case we have a better way to calculate these plateform-specific constants at runtime
// do that. For now hardcoded.
#define DAILY_INT_MAX ((int)((unsigned int)-1) - (1 << (sizeof(int) * 8 - 1)))
