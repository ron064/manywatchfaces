#pragma once
extern Layer *rootLayer;

typedef struct {
void (*load)();
void (*unload)();
void (*redraw)();
} app_t;
