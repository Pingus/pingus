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

#ifndef HEADER_PINGUS_PINGUS_COMPONENTS_CHOICE_BOX_HPP
#define HEADER_PINGUS_PINGUS_COMPONENTS_CHOICE_BOX_HPP

#include <sigc++/signal.h>

#include "engine/gui/rect_component.hpp"

namespace pingus {

class ChoiceBox : public pingus::gui::RectComponent
{
private:
  int current_choice;
  std::vector<std::string> choices;

public:
  ChoiceBox(Rect const& rect);

  void draw(DrawingContext& gc) override;
  void on_primary_button_press(int x, int y) override;
  void update_layout() override {}
  void add_choice(std::string const& str);
  void set_current_choice(int choice);

  sigc::signal<void (std::string)> on_change;

private:
  ChoiceBox (ChoiceBox const&);
  ChoiceBox& operator= (ChoiceBox const&);
};

} // namespace pingus

#endif

/* EOF */
