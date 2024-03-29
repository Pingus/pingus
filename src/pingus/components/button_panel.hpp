// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_COMPONENTS_BUTTON_PANEL_HPP
#define HEADER_PINGUS_PINGUS_COMPONENTS_BUTTON_PANEL_HPP

#include <vector>

#include "engine/display/sprite.hpp"
#include "engine/gui/rect_component.hpp"
#include "pingus/action_name.hpp"
#include "pingus/components/action_button.hpp"

namespace pingus {

class Server;

class ButtonPanel : public pingus::gui::RectComponent
{
private:
  struct ActionButton
  {
    ActionName::Enum name;
    Sprite     sprite;

    ActionButton() :
      name(),
      sprite()
    {}
  };

  Server* server;
  Sprite background;
  Sprite highlight;

  std::vector<ActionButton> buttons;
  std::vector<ActionButton>::size_type current_button;

  bool show_tip;
  std::vector<ActionButton>::size_type tip_button;

public:
  ButtonPanel(Server* s, Vector2i const& pos);
  ~ButtonPanel() override;

  void draw(DrawingContext& gc) override;
  void update (float delta) override;

  ActionName::Enum get_action_name();

  // Set the n'th button active
  void set_button(int n);

  void next_action();
  void previous_action();

  void on_primary_button_press(int x, int y) override;
  void on_primary_button_release(int x, int y) override;

  void on_pointer_enter() override;
  void on_pointer_leave() override;
  void on_pointer_move(int x, int y) override;

  void set_pos(Vector2i const& pos);

private:
  ButtonPanel (ButtonPanel const&);
  ButtonPanel& operator= (ButtonPanel const&);
};

} // namespace pingus

#endif

/* EOF */
