#include <pebble.h>
#include "main_menu.h"

static ActionMenuConfig main_menu_config;
static ActionMenu *main_menu;

void main_menu_show()
{
  if (!main_menu) {
    if (!main_menu_config.root_level) {
      // Calculate how big our menu needs to be. We
      // only show Launch events in it for now, so start
      // with that.
      
      //main_menu_config.root_level = action_menu_level_create();
    }
  }
}

void main_menu_hide()
{
  
}
