// Pingus - A free Lemmings clone
// Copyright (C) 2007 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_PINGUS_PINGUS_FONTS_HPP
#define HEADER_PINGUS_PINGUS_FONTS_HPP

#include "engine/display/font.hpp"

namespace Fonts {

extern Font chalk_large;
extern Font chalk_normal;
extern Font chalk_small;

extern Font courier_small;

extern Font pingus_small;
extern Font pingus_small_fixnum;

extern Font verdana11;

extern Font lcd;

/** Load all fonts */
void init ();

/** Unload fonts */
void deinit ();

} // namespace Fonts

#endif

/* EOF */
