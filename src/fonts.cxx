//  $Id: fonts.cxx,v 1.3 2003/03/21 22:08:06 grumbel Exp $
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

namespace Fonts {

FontHandle chalk_large;
FontHandle chalk_normal;
FontHandle chalk_small;

FontHandle pingus_small;
FontHandle pingus_large;
FontHandle smallfont;
FontHandle xterm;
FontHandle lcd;

void
init ()
{
  pingus_small = PingusResource::load_font("Fonts/pingus_small", "fonts");
  pingus_large = PingusResource::load_font("Fonts/pingus","fonts");

  chalk_large  = PingusResource::load_font("Fonts/chalk_large","fonts");
  chalk_normal = PingusResource::load_font("Fonts/chalk_normal","fonts");
  chalk_small  = PingusResource::load_font("Fonts/chalk_small","fonts");

  xterm        = PingusResource::load_font("Fonts/xterm","fonts");
  smallfont    = PingusResource::load_font("Fonts/smallfont","fonts");

  lcd          = PingusResource::load_font("Fonts/numbers", "fonts");
}

void deinit () {}

} // namespace Fonts

/* EOF */
