//  $Id: fonts.hxx,v 1.6 2003/04/22 16:40:41 grumbel Exp $
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

#include "gui/graphic_context.hxx"

namespace Fonts {

extern FontHandle chalk_large;
extern FontHandle chalk_normal;
extern FontHandle chalk_small;

extern FontHandle courier_small;

extern FontHandle pingus_small;
extern FontHandle pingus_small_fix_num;
extern FontHandle pingus_large;

extern FontHandle smallfont;
extern FontHandle smallfont_h;
extern FontHandle xterm;
extern FontHandle lcd;

/** Load all fonts */
void init ();

/** Unload fonts */
void deinit ();

} // namespace Fonts

#endif

/* EOF */
