sevenwatchfaces
===============

Pebble watch app demonstrating multiple watchfaces

Note: the button functionality in this source code is very basic. I refactored the code I wrote so that it can be easily understood and extended by other developers.

- The launcher code is complete and should not require any modifications.
- The config code provides basic app switching and may need only minor tweaks.
- The config7 code is where developers can define the list of watch faces they want packaged together and customize the button functionality as desired. A simple example which provides date style switching for one of the apps is shown along with the hooks for each of the buttons to extend funcitonality.

Watch face modification steps
=============================

- Remove references to Window in init() and deinit(). Let the launcher create and destroy the main window.
- #include "config.h"
- Provide entrypoints for creating (load_xxx), destroying (unload_xxx),  and redrawing (redraw_xxx) the window content. he last line of load_xxx should call configRedraw(); Add ENTRYPOINTS(xxx) into config7.h and ENTRYPOINTROW(xxx), into config.h
- Make all the remaining global variables and functions in your app static - to prevent identical named globals from other apps from clashing.
- Change all GColorBlack to backColor and GColorWhite to foreColor
- Test your watchface for leaks. Use pebble logs and make sure you see Still allocated <0B> when you exit the app.

I am aware of one issue with the final 2.0 firmware. It seems to happen when one app ore even unrelated apps use bitmaps and animation effects.
