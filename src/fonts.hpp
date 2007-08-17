//  $Id$
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

#ifndef HEADER_PINGUS_FONTS_HXX
#define HEADER_PINGUS_FONTS_HXX

#include "display/drawing_context.hpp"
#include "font.hpp"

namespace Fonts {

extern std::string encoding;

extern Font chalk_large;
extern Font chalk_normal;
extern Font chalk_small;

extern Font courier_small;

extern Font pingus_small;
extern Font pingus_small_fix_num;
extern Font pingus_large;

extern Font lcd;

/** Load all fonts */
void init ();

/** Unload fonts */
void deinit ();

} // namespace Fonts

#endif

/* EOF */
