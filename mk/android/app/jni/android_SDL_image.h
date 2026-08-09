// SPDX-FileCopyrightText: 2003-2006 SuperTux Development Team
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SDL_IMAGE_H_
#define SDL_IMAGE_H_

#include "SDL.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IMG_INIT_JPG 0x00000001
#define IMG_INIT_PNG 0x00000002
#define IMG_INIT_TIF 0x00000004
#define IMG_INIT_WEBP 0x00000008

extern DECLSPEC int SDLCALL IMG_Init(int flags);
extern DECLSPEC void SDLCALL IMG_Quit(void);
extern DECLSPEC SDL_Surface * SDLCALL IMG_Load(const char *file);
extern DECLSPEC SDL_Surface * SDLCALL IMG_Load_RW(SDL_RWops *src, int freesrc);
extern DECLSPEC const char * SDLCALL IMG_GetError(void);
extern DECLSPEC int SDLCALL IMG_SavePNG(SDL_Surface *surface, const char *file);

#ifdef __cplusplus
}
#endif

#endif /* SDL_IMAGE_H_ */
