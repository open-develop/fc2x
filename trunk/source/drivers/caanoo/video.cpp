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

static unsigned char s_palette[256*3]; // colors..

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
	int x,xx,idx,y,yy;
	unsigned char * scrPtr, * palPtr;

    // refresh the palette if required
    if(s_paletterefresh) {
        RedoPalette();
        s_paletterefresh = 0;
    }
	
	// XXX soules - not entirely sure why this is being done yet
    //XBuf += s_srendline * 256;
	
	// Draw our X buffer to the screen buffer
	scrPtr = &(scrBuf[96]);
	for(y=0;y<s_tlines;++y)
	{
		for(x=0,xx=32; x < 256;x++,xx++)
		{
			palPtr = &(s_palette[(XBuf[y*256+x]*3)]);
			*scrPtr++ = *palPtr++;
			*scrPtr++ = *palPtr++;
			*scrPtr++ = *palPtr;
		}
		scrPtr += 192;
	}
	
// Will probably have to break this into assembly for speed..
//	Blit8ToHigh(XBuf + NOFFSET, mapped_mem, NWIDTH, s_tlines, 960, 1, 1);
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
	s_palette[index*3+2] = r;
	s_palette[index*3+1] = g;
	s_palette[index*3] = b;
	    
	s_paletterefresh = 1;
}

/**
 * Gets the color for a particular index in the palette.
 */
void FCEUD_GetPalette(unsigned char index, unsigned char *r, unsigned char *g, unsigned char *b)
{
    *r = s_palette[index*3+2];
    *g = s_palette[index*3+1];
    *b = s_palette[index*3];
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

