#ifndef __FCEU_SDL_VIDEO_H
#define __FCEU_SDL_VIDEO_H

unsigned int PtoV(unsigned short x, unsigned short y);
bool FCEUD_ShouldDrawInputAids();
bool FCEUI_AviDisableMovieMessages();
void FCEUI_SetAviDisableMovieMessages(bool disable);

#endif
