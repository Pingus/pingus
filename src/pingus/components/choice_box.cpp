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

#include "pingus/components/choice_box.hpp"

#include "engine/display/drawing_context.hpp"
#include "pingus/fonts.hpp"
#include "pingus/globals.hpp"

ChoiceBox::ChoiceBox(const Rect& rect_) :
  RectComponent(rect_),
  current_choice(0),
  choices(),
  on_change()
{
}

void
ChoiceBox::draw(DrawingContext& gc)
{
  if (globals::developer_mode)
    gc.draw_rect(rect, Color(0, 255, 255));

  if (!choices.empty())
  {
    if (current_choice >= 0 && current_choice < int(choices.size()))
    {
      //if (current_choice != 0)
      gc.print_left(Fonts::chalk_normal,  Vector2i(rect.left, rect.top), "<");

      //if (current_choice != int(choices.size())-1)
      gc.print_right(Fonts::chalk_normal, Vector2i(rect.right, rect.top), ">");

      gc.print_center(Fonts::chalk_normal, Vector2i(rect.left + rect.get_width()/2, rect.top),
                      choices[static_cast<size_t>(current_choice)]);
    }
  }
}

void
ChoiceBox::on_primary_button_press(int x, int y)
{
  if (!choices.empty())
  {
    int last_current_choice = current_choice;
    x -= rect.left;

    if (x > rect.get_width()/2)
    {
      current_choice += 1;
      if (current_choice >= int(choices.size()))
        current_choice = 0;
      //current_choice = choices.size() - 1;
    }
    else
    {
      current_choice -= 1;
      if (current_choice < 0)
        current_choice = static_cast<int>(choices.size()) - 1;
      //current_choice = 0;
    }

    if (last_current_choice != current_choice)
      on_change(choices[static_cast<size_t>(current_choice)]);
  }
}

void
ChoiceBox::add_choice(const std::string& str)
{
  choices.push_back(str);
}

void
ChoiceBox::set_current_choice(int choice)
{
  if (choice >= 0 && choice < int(choices.size()))
    current_choice = choice;
}

/* EOF */
