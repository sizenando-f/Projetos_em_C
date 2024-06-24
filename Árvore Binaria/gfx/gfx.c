#include "gfx.h"

static SDL_Surface* g_gfx_screen = NULL;
static SDL_Surface* g_gfx_text = NULL;
static unsigned g_gfx_window_width = 0;
static unsigned g_gfx_window_height = 0;
static TTF_Font* g_gfx_font = NULL;
static SDL_Color g_gfx_color = {255, 255, 255, 255}; 
static unsigned short g_gfx_font_size = 14;

void gfx_init(unsigned width, unsigned height, const char* caption)
{
	SDL_Init(SDL_INIT_VIDEO);	

	assert( (g_gfx_screen = SDL_SetVideoMode(width, height, 0, SDL_HWSURFACE|SDL_DOUBLEBUF)) != NULL );
	g_gfx_window_width = width;
	g_gfx_window_height = height;
	SDL_WM_SetCaption(caption, 0);

	SDL_FillRect(g_gfx_screen, NULL, SDL_MapRGB(g_gfx_screen->format, 0, 0, 0));
	
	TTF_Init();
	assert( (g_gfx_font = TTF_OpenFont(FONT_NAME, g_gfx_font_size)) != NULL );
}

void gfx_quit()
{
	SDL_FreeSurface(g_gfx_text);
	TTF_CloseFont(g_gfx_font);
	TTF_Quit();
	SDL_Quit();
}

int gfx_get_event(SDL_Event* event)
{
	return SDL_PollEvent(event);
}

void gfx_paint()
{
	SDL_Flip(g_gfx_screen);
}

unsigned gfx_get_width()
{
	return g_gfx_window_width;
}

unsigned gfx_get_height()
{
	return g_gfx_window_height;
}

void gfx_get_color(unsigned short* r, unsigned short* g, unsigned short* b)
{
	*r = g_gfx_color.r;	
	*g = g_gfx_color.g;	
	*b = g_gfx_color.b;	
}

void gfx_set_color(unsigned short r, unsigned short g, unsigned short b)
{
	g_gfx_color.r = r;	
	g_gfx_color.g = g;	
	g_gfx_color.b = b;	
}

unsigned short gfx_get_font_size()
{
	return g_gfx_font_size;
}

void gfx_set_font_size(unsigned short size)
{
	g_gfx_font_size = size;

	if( g_gfx_font != NULL )
	{	
		TTF_CloseFont(g_gfx_font);
		assert( (g_gfx_font = TTF_OpenFont(FONT_NAME, g_gfx_font_size)) != NULL );
	}
}

void gfx_get_text_size(const char* text, int* width, int* height)
{
	TTF_SizeText(g_gfx_font, text, width, height);
}

void gfx_clear()
{
	SDL_FillRect(g_gfx_screen, NULL, 0x000000);
}

void gfx_text(int x, int y, const char* text)
{	
	SDL_Rect position;

	position.x = x;
	position.y = y;
	position.w = position.h = 0;

	g_gfx_text = TTF_RenderText_Blended(g_gfx_font, text, g_gfx_color);
 	SDL_BlitSurface(g_gfx_text, NULL, g_gfx_screen, &position);
}

void gfx_line(int x1, int y1, int x2, int y2)
{
	lineRGBA(g_gfx_screen, x1, y1, x2, y2, g_gfx_color.r, g_gfx_color.g, g_gfx_color.b, 255);
}

void gfx_rectangle(int x1, int y1, int x2, int y2)
{
	rectangleRGBA(g_gfx_screen, x1, y1, x2, y2, g_gfx_color.r, g_gfx_color.g, g_gfx_color.b, 255);
}

void gfx_filled_rectangle(int x1, int y1, int x2, int y2)
{
	boxRGBA(g_gfx_screen, x1, y1, x2, y2, g_gfx_color.r, g_gfx_color.g, g_gfx_color.b, 255);
}

void gfx_ellipse(int x, int y, int rx, int ry)
{
	ellipseRGBA(g_gfx_screen, x, y, rx, ry, g_gfx_color.r, g_gfx_color.g, g_gfx_color.b, 255);
}

void gfx_filled_ellipse(int x, int y, int rx, int ry)
{
	filledEllipseRGBA(g_gfx_screen, x, y, rx, ry, g_gfx_color.r, g_gfx_color.g, g_gfx_color.b, 255);
}

/* end of gfx.c */
