//  $Id: audio.hh,v 1.3 2000/05/01 20:11:15 grumbel Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef AUDIO_HH
#define AUDIO_HH

#include <config.h>
#include "globals.hh"

// Simple wrapper header around SDL.h and SDL_mixer.h.

#ifdef HAVE_LIBSDL_MIXER
#  include <SDL/SDL.h>
#  include <SDL/SDL_mixer.h>
#else 
typedef unsigned short Uint16;
typedef void* Mix_Music;
typedef void* Mix_Chunk;

/* Audio format flags (defaults to LSB byte order) */
#define AUDIO_U8	0x0008	/* Unsigned 8-bit samples */
#define AUDIO_S8	0x8008	/* Signed 8-bit samples */
#define AUDIO_U16LSB	0x0010	/* Unsigned 16-bit samples */
#define AUDIO_S16LSB	0x8010	/* Signed 16-bit samples */
#define AUDIO_U16MSB	0x1010	/* As above, but big-endian byte order */
#define AUDIO_S16MSB	0x9010	/* As above, but big-endian byte order */
#define AUDIO_U16	AUDIO_U16LSB
#define AUDIO_S16	AUDIO_S16LSB
#endif

// Sound data
extern Uint16 pingus_audio_format;
extern int    pingus_audio_rate;
extern int    pingus_audio_channels;
extern int    pingus_audio_buffers;

#endif

/* EOF */
