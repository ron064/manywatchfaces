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
