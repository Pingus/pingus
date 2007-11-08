//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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

#include "globals.hpp"
#include "fonts.hpp"
#include "display/drawing_context.hpp"
#include "label.hpp"

Label::Label(const std::string& label, const Rect& rect)
  : RectComponent(rect),
    label(label)
{
}

void
Label::draw(DrawingContext& gc)
{
  if (maintainer_mode)
    gc.draw_rect(rect, Color(0, 255, 255));

  gc.print_left(Fonts::chalk_normal, rect.left, rect.top, label);
}

/* EOF */
