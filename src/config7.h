#pragma once

#include "common.h"

extern app_t *app;
extern int whichApp;
void custom_init();
void custom_deinit(Window *window);
bool custom_up();
bool custom_down();
bool custom_select();
bool custom_long_select();

enum {KEY_WHICH_APP, KEY_INVERTED, KEY_DATE}; 

#define APP_COUNT 7

