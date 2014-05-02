#pragma once

// Each added app must include config.h

void configRedraw();

extern GColor backColor;
extern GColor foreColor;
extern Layer *rootLayer;
extern int dateStyle;

#define ENTRYPOINTS(NAME) void redraw_ ## NAME (); void load_ ## NAME (); void unload_ ## NAME ();

	ENTRYPOINTS(words)
	ENTRYPOINTS(retro)
	ENTRYPOINTS(Perspective)
	ENTRYPOINTS(bit)
	ENTRYPOINTS(ill)
	ENTRYPOINTS(fuzzy)
	ENTRYPOINTS(toe)

static const int8_t OVL_RESOURCE_IDS[] = { // Change if you have >127 in RESOURCE_ID
		RESOURCE_ID_OVL_WORDS,
		RESOURCE_ID_OVL_RETRO,
		RESOURCE_ID_OVL_PRESP,
		RESOURCE_ID_OVL_BIT,
		RESOURCE_ID_OVL_ILL,
		RESOURCE_ID_OVL_FUZZY,
		RESOURCE_ID_OVL_TOE
	};