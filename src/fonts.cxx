//  $Id: fonts.cxx,v 1.7 2003/12/13 15:10:44 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#include "pingus_resource.hxx"
#include "fonts.hxx"

namespace Pingus {
namespace Fonts {

CL_Font chalk_large;
CL_Font chalk_normal;
CL_Font chalk_small;

CL_Font pingus_small;
CL_Font pingus_small_fix_num;
CL_Font pingus_large;

CL_Font smallfont;
CL_Font smallfont_h;

CL_Font courier_small;
CL_Font xterm;
CL_Font lcd;

void
init ()
{
#ifndef CLANLIB_0_6
  // FIXME: Just a workaround, no fix
  pingus_small = PingusResource::load_font("Fonts/pingus_small","fonts");
  xterm        = pingus_small;

  pingus_small_fix_num = pingus_small;
  pingus_large = pingus_small;

  chalk_large  = pingus_small;
  chalk_normal = pingus_small;
  chalk_small  = pingus_small;

  courier_small = pingus_small;

  smallfont    = pingus_small;
  smallfont_h  = pingus_small;

  lcd          = pingus_small;
#else 
  pingus_small = PingusResource::load_font("Fonts/pingus_small", "fonts");
  pingus_small_fix_num = PingusResource::load_font("Fonts/pingus_small_fix_num", "fonts");
  pingus_large = PingusResource::load_font("Fonts/pingus","fonts");

  chalk_large  = PingusResource::load_font("Fonts/chalk_large","fonts");
  chalk_normal = PingusResource::load_font("Fonts/chalk_normal","fonts");
  chalk_small  = PingusResource::load_font("Fonts/chalk_small","fonts");

  courier_small = PingusResource::load_font("Fonts/courier_small", "fonts");

  xterm        = PingusResource::load_font("Fonts/xterm","fonts");
  smallfont    = PingusResource::load_font("Fonts/smallfont","fonts");
  smallfont_h  = PingusResource::load_font("Fonts/smallfont_h","fonts");

  lcd          = PingusResource::load_font("Fonts/numbers", "fonts");
#endif
}

void deinit () {}

} // namespace Fonts
} // namespace Pingus

/* EOF */
