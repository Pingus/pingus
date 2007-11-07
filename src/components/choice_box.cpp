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

#include "fonts.hpp"
#include "display/drawing_context.hpp"
#include "choice_box.hpp"

ChoiceBox::ChoiceBox(const Rect& rect)
  : RectComponent(rect)
{
  current_choice = 0;
  choices.push_back("Choice 1");
  choices.push_back("Choice 2");
  choices.push_back("Choice 3");
}

void
ChoiceBox::draw(DrawingContext& gc)
{
  if (current_choice >= 0 && current_choice < int(choices.size()))
    {
      gc.print_center(Fonts::chalk_normal, rect.left+rect.get_width()/2, rect.top, 
                      "< " + choices[current_choice] + " >");
    }
}

void
ChoiceBox::on_primary_button_press(int x, int y)
{
  current_choice += 1;
  if (current_choice >= int(choices.size()))
    current_choice = choices.size() - 1;
}

void
ChoiceBox::on_secondary_button_press(int x, int y)
{
  current_choice -= 1;
  if (current_choice < 0)
    current_choice = 0;
}

/* EOF */
