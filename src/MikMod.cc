//  $Id: MikMod.cc,v 1.1 2000/03/20 18:55:26 grumbel Exp $
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

#include "PingusError.hh"
#include "globals.hh"
#include "MikMod.hh"

bool MikMod::is_init;
bool MikMod::delete_on_stop;

#if HAVE_LIBMIKMOD
  MODULE* MikMod::current_module;
#endif /* HAVE_LIBMIKMOD */

void
MikMod::init()
{
#if HAVE_LIBMIKMOD
  MikMod_RegisterAllDrivers();
  MikMod_RegisterAllLoaders();
  
#if LIBMIKMOD_VERSION >= ((3 << 16) | (1 << 8) | (7))
  MikMod_Init("");
#else
  MikMod_Init();
#endif

#endif /* HAVE_LIBMIKMOD */
}

void
MikMod::deinit()
{
#if HAVE_LIBMIKMOD
  MikMod_Exit();
#endif /* HAVE_LIBMIKMOD */
}
 
void
MikMod::keep_alive()
{
#if HAVE_LIBMIKMOD
  if (music_enabled)
    MikMod_Update();
#endif /* HAVE_LIBMIKMOD */
}
 
MODULE* 
MikMod::load(string filename, bool del_on_stop, int maxchan, bool curious)
{
#if HAVE_LIBMIKMOD
  if (music_enabled)
    {
      char* str;

      str = strdup(filename.c_str());
  
      current_module = Player_Load(str, maxchan, curious);
      delete_on_stop = del_on_stop;
  
      ::free(str);

      if (!current_module)
	{
	  throw PingusError("Couldn't find: " + filename);
	}
    }
  return current_module;
#endif /* HAVE_LIBMIKMOD */
}

void 
MikMod::play(MODULE* m)
{
#if HAVE_LIBMIKMOD
  if (music_enabled)
    {
      if (m)
	Player_Start(m);
      else
	Player_Start(current_module);
    }
#endif /* HAVE_LIBMIKMOD */
}

bool
MikMod::is_playing()
{
#if HAVE_LIBMIKMOD
  if (music_enabled)
    {
      return Player_Active();
    }
  else
    {
      return false;
    }
#endif /* HAVE_LIBMIKMOD */
}

void
MikMod::stop()
{
#if HAVE_LIBMIKMOD
  if (music_enabled)
    {
      Player_Stop();
    }
#endif /* HAVE_LIBMIKMOD */
}

void
MikMod::free(MODULE* module)
{
#if HAVE_LIBMIKMOD
  if (music_enabled)
    {
      Player_Free(module);
    }
#endif /* HAVE_LIBMIKMOD */
}

/* EOF */
