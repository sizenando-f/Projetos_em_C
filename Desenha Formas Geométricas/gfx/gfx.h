/*

Simple GFX (graphics) library based on SDL library

Author: Fabrício Sérgio de Paula (fabricio.paula@gmail.com)

Year: 2013

Use: Before using this library you must install the SDL_gfx and SDL_ttf libraries.
After that, make sure all gfx library files are at the YOUR-PROGRAM-DIR/gfx directory.
Then, your program must include gfx.h and be linked with the -lgfx -lSDL_gfx -lSDL_ttf
options.

*/

#ifndef _GFX_H_
#define _GFX_H_

#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_ttf.h>
#include <assert.h>

#define FONT_NAME "gfx/FreeSans.ttf"

void gfx_init(unsigned width, unsigned height, const char* caption); /* graphic window initialization */
void gfx_quit(); /* graphic window termination */
int gfx_get_event(SDL_Event* event); /* get event (keyboard/mouse/window events). return 0 if there's no event, otherwise return 1 */ 
void gfx_paint(); /* apply the changes to screen: must be called to show the graphic elements */
unsigned gfx_get_width(); /* get window width */
unsigned gfx_get_height(); /* get window height */
void gfx_get_color(unsigned short* r, unsigned short* g, unsigned short* b); /* get current text/object color */
void gfx_set_color(unsigned short r, unsigned short g, unsigned short b); /* set text/object color */
unsigned short gfx_get_font_size(); /* get current text font size */
void gfx_set_font_size(unsigned short size); /* set the text font size */
void gfx_get_text_size(const char* text, int* width, int* height); /* get the width and height of text to be printed on screen */
void gfx_clear(); /* clear screen */
void gfx_text(int x, int y, const char* msg); /* print message at (x,y) point */
void gfx_line(int x1, int y1, int x2, int y2); /* draw a line from (x1,y1) to (x2,y2) */
void gfx_rectangle(int x1, int y1, int x2, int y2); /* draw a rectangle of top-left corner (x1,y1) and bottom-right corner (x2,y2) */
void gfx_filled_rectangle(int x1, int y1, int x2, int y2); /* draw and fill */
void gfx_ellipse(int x, int y, int rx, int ry); /* draw ellipse at center (x,y) and x radius rx and y radius ry */
void gfx_filled_ellipse(int x, int y, int rx, int ry); /* draw and fill */

#endif
