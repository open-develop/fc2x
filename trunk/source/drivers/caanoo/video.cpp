#include <stdio.h>

#include "video.h"
#include "config.h"
#include "main.h"

#include "../common/vidblit.h"

// GLOBALS
//extern Config * g_config;

static int s_curbpp;
static int s_srendline, s_erendline;
static int s_tlines;
static int s_inited;

static double s_exs, s_eys;
static int s_eefx;
static int s_clipSides;
static int s_fullscreen;
static int noframe;

#define NWIDTH	(256 - (s_clipSides ? 16 : 0))
#define NOFFSET	(s_clipSides ? 8 : 0)

static int s_paletterefresh;

struct _S_PALETTE {
	unsigned char r, g, b, dummy_byte;
} s_palette[256];

extern bool MaxSpeed;

//draw input aids if we are fullscreen
bool FCEUD_ShouldDrawInputAids()
{
	return s_fullscreen!=0;
}

int KillVideo()
{
}

// this variable contains information about the special scaling filters
static int s_sponge;

void FCEUD_VideoChanged()
{
	int buf;
	//g_config->getOption("SDL.PAL", &buf);
	//if(buf)
	//	PAL = 1;
	//else
	//	PAL = 0;
}

/** 
 * Pushes the palette structure into the underlying video subsystem.
 */
static void
RedoPalette()
{
    SetPaletteBlitToHigh((uint8*)s_palette);
}

void BlitScreen(uint8 *XBuf, uint8 *scrBuf)
{
	int x,y;
	unsigned int spixel1, spixel2, spixel3, spixel4;
	
	unsigned char * s_ptr;
	unsigned int * pal_ptr;
	unsigned int * d_ptr;

    // refresh the palette if required
    if(s_paletterefresh) {
        RedoPalette();
        s_paletterefresh = 0;
    }
	
	// Thanks to senquack for help speeding this up
	
	// Draw our X buffer to the screen buffer
	s_ptr = XBuf;
	d_ptr = (unsigned int*)&(scrBuf[96]);
	pal_ptr = (unsigned int*)s_palette;
	
	for(y=s_tlines; y>0; --y)
	{
		for(x=64; x > 0; --x)
		{
			spixel1 = pal_ptr[*s_ptr++];	// Grab 4 pixels at a time
			spixel2 = pal_ptr[*s_ptr++];
			spixel3 = pal_ptr[*s_ptr++];
			spixel4 = pal_ptr[*s_ptr++];
			*d_ptr++ = spixel1 | (spixel2 << 24);			// Write 4 pixels in 3-byte per pixel format (24-bit)
			*d_ptr++ = (spixel2>>8) | (spixel3 << 16);  	// [ 3 2 1 0 ] [ 3 2 1 0 ] [ 3 2 1 0 ] << actual bytes
			*d_ptr++ = (spixel3 >> 16) | (spixel4 << 8);	// [ 3 3 3 2 ] [ 2 2 1 1 ] [ 1 0 0 0 ] << pixels filled
		}
		d_ptr += 48; // 64 pixels * 3 bytes per pixel = 192.. / 4 = 48 pixel offset
	}
}

int InitVideo(FCEUGI *gi)
{
   // XXX soules - const?  is this necessary?
    int error, flags = 0;
    int doublebuf, xstretch, ystretch, xres, yres;

    FCEUI_printf("Initializing video...");
/*
    // load the relevant configuration variables
    g_config->getOption("SDL.Fullscreen", &s_fullscreen);
    g_config->getOption("SDL.DoubleBuffering", &doublebuf);
#ifdef OPENGL
    g_config->getOption("SDL.OpenGL", &s_useOpenGL);
#endif
    g_config->getOption("SDL.SpecialFilter", &s_sponge);
    g_config->getOption("SDL.XStretch", &xstretch);
    g_config->getOption("SDL.YStretch", &ystretch);
    g_config->getOption("SDL.XResolution", &xres);
    g_config->getOption("SDL.YResolution", &yres);
    g_config->getOption("SDL.ClipSides", &s_clipSides);
    g_config->getOption("SDL.NoFrame", &noframe);
*/
	s_fullscreen = 0;
	doublebuf = 0;
	s_sponge = 0;
	s_clipSides = 0;
	noframe = 0;

    // check the starting, ending, and total scan lines
    FCEUI_GetCurrentVidSystem(&s_srendline, &s_erendline);
    s_tlines = s_erendline - s_srendline + 1;

    s_inited = 1;
	
    s_curbpp = 24; // Caanoo screen is 24 bits

    if(s_curbpp != 8 && s_curbpp != 16 && s_curbpp != 24 && s_curbpp != 32) {
        FCEU_printf("  Sorry, %dbpp modes are not supported by FCE Ultra.  Supported bit depths are 8bpp, 16bpp, and 32bpp.\n", s_curbpp);
        KillVideo();
        return -1;
    }

    s_paletterefresh = 1;
	
	InitBlitToHigh(s_curbpp >> 3,0xFF0000,0xFF00,0xFF,0,0,0);
	
    return 0;
}

void FCEUD_SetPalette(unsigned char index, unsigned char r, unsigned char g, unsigned char b)
{
	// Palette business
	s_palette[index].b = r;
	s_palette[index].g = g;
	s_palette[index].r = b;
	    
	s_paletterefresh = 1;
}

/**
 * Gets the color for a particular index in the palette.
 */
void FCEUD_GetPalette(unsigned char index, unsigned char *r, unsigned char *g, unsigned char *b)
{
    *r = s_palette[index].b;
    *g = s_palette[index].g;
    *b = s_palette[index].r;
}

///Currently unimplemented.
void LockConsole(){}

///Currently unimplemented.
void UnlockConsole(){}

unsigned int PtoV(unsigned short x, unsigned short y)
{
    y = (uint16)((double)y / s_eys);
    x = (uint16)((double)x / s_exs);
    if(s_clipSides) {
        x += 8;
    }
    y += s_srendline;
    return (x | (y << 16));
}

bool disableMovieMessages = false;
bool FCEUI_AviDisableMovieMessages()
{
	if (disableMovieMessages)
		return true;

	return false;
}

void FCEUI_SetAviDisableMovieMessages(bool disable)
{
	disableMovieMessages = disable;
}

