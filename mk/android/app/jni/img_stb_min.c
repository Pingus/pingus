// SPDX-FileCopyrightText: 2003-2006 SuperTux Development Team
// SPDX-FileCopyrightText: 2024-2026 SuperTux Milestone 1 port contributors
// SPDX-License-Identifier: GPL-3.0-or-later

/* Minimal SDL_image API for Android using upstream stb_image.h */
#include <SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_THREAD_LOCALS
#define STBI_NO_HDR
#define STBI_NO_LINEAR
#define STBI_FAILURE_USERMSG
#include "stb_image.h"

static char img_error[256];

const char *IMG_GetError(void)
{
  const char *sdl = SDL_GetError();
  if (sdl && sdl[0])
    return sdl;
  return img_error[0] ? img_error : "";
}

static void set_err(const char *msg)
{
  if (msg && msg[0])
    SDL_SetError("%s", msg);
  snprintf(img_error, sizeof(img_error), "%s", msg ? msg : "");
}

int IMG_Init(int flags)
{
  img_error[0] = '\0';
  return flags;
}

void IMG_Quit(void)
{
  img_error[0] = '\0';
}

static SDL_Surface *surface_from_rgba(unsigned char *data, int w, int h)
{
  SDL_Surface *surf = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA32);
  if (!surf) {
    stbi_image_free(data);
    set_err(SDL_GetError());
    return NULL;
  }
  memcpy(surf->pixels, data, (size_t)w * (size_t)h * 4u);
  stbi_image_free(data);
  return surf;
}

SDL_Surface *IMG_Load_RW(SDL_RWops *src, int freesrc);

SDL_Surface *IMG_Load(const char *file)
{
  /* On Android, fopen cannot see APK assets. SDL_RWFromFile uses AssetManager. */
  SDL_RWops *rw;

  img_error[0] = '\0';
  if (!file) {
    set_err("IMG_Load: NULL filename");
    return NULL;
  }

  rw = SDL_RWFromFile(file, "rb");
  if (!rw) {
    set_err(SDL_GetError());
    return NULL;
  }
  return IMG_Load_RW(rw, 1);
}

SDL_Surface *IMG_Load_RW(SDL_RWops *src, int freesrc)
{
  int w = 0, h = 0, n = 0;
  unsigned char *data;
  unsigned char *buf = NULL;
  size_t cap = 0, len = 0;
  unsigned char chunk[4096];
  size_t got;

  img_error[0] = '\0';
  if (!src) {
    set_err("IMG_Load_RW: NULL RWops");
    return NULL;
  }

  /* Read whole stream into memory for stbi_load_from_memory. */
  while ((got = SDL_RWread(src, chunk, 1, sizeof(chunk))) > 0) {
    if (len + got > cap) {
      size_t ncap = cap ? cap * 2 : 8192;
      while (ncap < len + got)
        ncap *= 2;
      {
        unsigned char *nb = (unsigned char *)realloc(buf, ncap);
        if (!nb) {
          free(buf);
          if (freesrc)
            SDL_RWclose(src);
          set_err("IMG_Load_RW: out of memory");
          return NULL;
        }
        buf = nb;
        cap = ncap;
      }
    }
    memcpy(buf + len, chunk, got);
    len += got;
  }

  if (freesrc)
    SDL_RWclose(src);

  if (!buf || len == 0) {
    free(buf);
    set_err("IMG_Load_RW: empty or unreadable stream");
    return NULL;
  }

  data = stbi_load_from_memory(buf, (int)len, &w, &h, &n, 4);
  free(buf);
  if (!data) {
    const char *why = stbi_failure_reason();
    set_err(why ? why : "stbi_load_from_memory failed");
    return NULL;
  }
  return surface_from_rgba(data, w, h);
}
