#include "pebble.h"
#include "common.h"
#include "launcher.h"
#include "config.h"
#include "config7.h"

/*
	This config demonstrates the basic functionality:
	1. App definition and switching using up/down
	2. Screen inversion using long select
	
	Custom configuration using buttons can be implemented by overriding 
	custom_up() custom_down() custom_select() custom_long_select().
*/

int whichApp = 0;
static bool inverted = false;

Layer *rootLayer;
GColor backColor = GColorBlack;
GColor foreColor = GColorWhite;


app_t *app;

void configRedraw() {
	window_redraw_with_background(backColor);
}

static void do_invert() {
	inverted = !inverted;
	backColor = inverted ? GColorWhite : GColorBlack;
	foreColor = inverted ? GColorBlack : GColorWhite;
	configRedraw();
  	persist_write_int(KEY_INVERTED, inverted);
}


static void config_swap(bool up) {
	window_unload(NULL);
	if (up) {
		if (++whichApp >= APP_COUNT) {
			whichApp = 0;
		}
	} else if (--whichApp < 0) {
		whichApp += APP_COUNT;
	}
	current_app = app[whichApp];
	window_load(NULL);
	persist_write_int(KEY_WHICH_APP, whichApp);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
	if (!custom_down()) {
		config_swap(false);
	}
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
	if (!custom_up()) {
		config_swap(true);
	}
}

static void long_select_click_handler(ClickRecognizerRef recognizer, void *context) {
	if (!custom_long_select()) {
		do_invert();
	}
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	if (!custom_select()) {
		// default behavior turns on back light :-)
	}
}


static void click_config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
	window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
	
	window_long_click_subscribe(BUTTON_ID_SELECT, 0, long_select_click_handler, NULL);
}

void config_deinit(Window *window) {
	// Entire app is quitting. Deallocate everything that config allocated.
	custom_deinit(window);
}

void config_init(Window *window) {
	rootLayer = window_get_root_layer(window); // Every app needs this
	window_set_click_config_provider(window, click_config_provider);

	inverted = persist_read_int(KEY_INVERTED);
	backColor = inverted ? GColorWhite : GColorBlack;
	foreColor = inverted ? GColorBlack : GColorWhite;
	
	whichApp = persist_read_int(KEY_WHICH_APP);
	if (whichApp < 0 || whichApp >= APP_COUNT) {
		whichApp = 0;
	}

	custom_init();
	current_app = app[whichApp];
}
