//  $Id: fonts.cxx,v 1.8 2003/12/13 23:41:11 grumbel Exp $
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

#include "resource.hxx"
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
  chalk_large  = Resource::load_font("fonts/chalk_large"); 
  chalk_normal = Resource::load_font("fonts/chalk_normal");
  chalk_small  = Resource::load_font("fonts/chalk_small");

  pingus_small = Resource::load_font("fonts/pingus_small");
  pingus_small_fix_num = Resource::load_font("fonts/pingus_small_fix_num");
  pingus_large = Resource::load_font("fonts/pingus");

  courier_small = Resource::load_font("fonts/courier_small");
  xterm        = Resource::load_font("fonts/courier_small");
  smallfont    = Resource::load_font("fonts/courier_small"); //PingusResource::load_font("Fonts/smallfont","fonts");
  smallfont_h  = Resource::load_font("fonts/courier_small"); // PingusResource::load_font("Fonts/smallfont_h","fonts");

  lcd          = Resource::load_font("fonts/courier_small"); // PingusResource::load_font("Fonts/numbers", "fonts");
}

void deinit () {}

} // namespace Fonts
} // namespace Pingus

/* EOF */
