//  $Id: MikMod.hh,v 1.2 2000/04/08 20:20:25 grumbel Exp $
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

#ifndef MIKMOD_HH
#define MIKMOD_HH

#include <string>
#include <config.h>

#if HAVE_LIBMIKMOD
#  include <mikmod.h>
#else
// Using this as a dummy
typedef void MODULE;
#endif /* HAVE_LIBMIKMOD */

class MikMod
{
private:
  static bool is_init;
  static bool delete_on_stop;

  static MODULE* current_module;

public:
  static void init();
  static void deinit();
  
  static MODULE* load(string, bool del_on_stop = true, int maxchan = 64, bool curious = 0);
  static void play(MODULE* m = 0);
  static void stop();
  static void free(MODULE*);
  static bool is_playing();
  static void keep_alive();
};

#endif

/* EOF */
