#include "pebble.h"
#include "common.h"
#include "launcher.h"
#include "config.h"
#include "config7.h"
#include "pebble_app_info.h"
extern const PebbleAppInfo __pbl_app_info;
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


// Number of overlays present 
#define NUM_OVERLAYS 7

// Record for current overlay 
int current_overlay = 0;

// struct to hold addresses and lengths
typedef struct overlay_region_t_struct
{
	void* load_ro_base;
	void* load_rw_base;
	void* exec_zi_base;
	unsigned int ro_length;
	unsigned int zi_length;
} overlay_region_t;
//ToDo: use this to allow Debug information.

uint8_t * OVL_P;
#define OVL_SIZE 0xB7C // 0x0C00 -0x0084

// Array describing the overlays 
extern const overlay_region_t overlay_regions[NUM_OVERLAYS];

int overlay_prepare(int app_num)
{
 	int size=-2;
	//const overlay_region_t * selected_region;
	if (app_num>NUM_OVERLAYS)
	{
		APP_LOG(APP_LOG_LEVEL_ERROR, "app_error:%d",app_num);
		return -1;
	}
	size= resource_load(resource_get_handle(OVL_RESOURCE_IDS[app_num]), OVL_P, OVL_SIZE);
	APP_LOG(APP_LOG_LEVEL_INFO, "app:%d size:%d",app_num, size);
	//dump_mem();
	if (size<0)
		return size;

	
	// this function will try to load the app into the reserved area.
	// on sucsess it will return app number
	// on fail it will return negative error code
	// This function should also save the active app for the debugger
	
	// ToDo: handle DebugData

	// set selected region 
	//selected_region = &overlay_regions[app_num-1];	
	
	return app_num;
}

	
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
	if ((whichApp>=0)&&(whichApp<APP_COUNT))
	{
		int ovl_rslt= overlay_prepare(whichApp);	
		if (ovl_rslt<=0)
		{
			APP_LOG(APP_LOG_LEVEL_ERROR,"fail to load app:%d, error:%d", whichApp, ovl_rslt);
			whichApp=5; // load some watch-face that is not overlay
		}
		APP_LOG(APP_LOG_LEVEL_INFO, "app:%d",whichApp);
	}
	current_app = app[whichApp];
	
	window_load(NULL);
	//APP_LOG(APP_LOG_LEVEL_INFO, "loaded");
	
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

	APP_LOG(APP_LOG_LEVEL_INFO, "Version %d.%d", __pbl_app_info.app_version.major, __pbl_app_info.app_version.minor);
	//APP_LOG(APP_LOG_LEVEL_INFO, "Pointer %p",(void*) & __pbl_app_info);
	//dump_mem();
	//ToDo: 
	//1. add option to disable overlay for testing without linker changes.
	//2. (Optionel), get address from linker data
	OVL_P = (void *) & __pbl_app_info;
	OVL_P+=0x84;
	int ovl_rslt= overlay_prepare(whichApp);	
	if (ovl_rslt<=0)
	{
		APP_LOG(APP_LOG_LEVEL_ERROR, "fail to load app:%d, error:%d",whichApp,ovl_rslt);
		whichApp=5; // load some watch-face that is not overlay
		//ToDo: define fall-back watchface as 0 and match OVL table to APP table( maybe simple -1)
	}
	//APP_LOG(APP_LOG_LEVEL_INFO, "startl %p",(void*) p);
	//APP_LOG(APP_LOG_LEVEL_INFO, "val %d", *p);

	custom_init();
	current_app = app[whichApp];
}
