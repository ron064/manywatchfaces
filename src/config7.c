#include "pebble.h"
#include "config.h"
#include "config7.h"

/*
	This config demonstrates how to define the apps and how to do
	an app specific configuration using select (Retro has different date styles)
	
	Custom configuration using buttons can be implemented by overriding 
	custom_up() custom_down() custom_select() custom_long_select().
*/
#define RETRO_APP 1

int dateStyle = 0;


#define ENTRYPOINTROW(NAME) { .load = load_ ## NAME, .unload = unload_ ## NAME, .redraw = redraw_ ## NAME }

static app_t appConfig[APP_COUNT] = {
	ENTRYPOINTROW(words),
	ENTRYPOINTROW(retro),
	ENTRYPOINTROW(Perspective),
	ENTRYPOINTROW(bit),
	ENTRYPOINTROW(ill),
	ENTRYPOINTROW(fuzzy),
	ENTRYPOINTROW(toe),
};

bool custom_select() {
	if (whichApp == RETRO_APP) {
		if (++dateStyle >= 3) {
			dateStyle = 0;
		}
		configRedraw();
		persist_write_int(KEY_DATE, dateStyle);
		return true;
	}
	return false;
}

bool custom_up() {
	return false;
}

bool custom_down() {
	return false;
}

bool custom_long_select() {
	return false;
}

void custom_deinit(Window *window) {
	// Entire app is quitting. Deallocate everything that config allocated.
}

void custom_init(){
	dateStyle = persist_read_int(KEY_DATE);
	app = appConfig;
}


