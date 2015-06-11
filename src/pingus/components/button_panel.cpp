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

#include "pingus/components/button_panel.hpp"

#include <algorithm>

#include "engine/display/drawing_context.hpp"
#include "pingus/fonts.hpp"
#include "pingus/globals.hpp"
#include "pingus/server.hpp"
#include "util/string_util.hpp"

ButtonPanel::ButtonPanel(Server* s, const Vector2i& pos) :
  RectComponent(Rect()),
  server(s),
  background("core/buttons/buttonbackground"),
  highlight("core/buttons/buttonbackgroundhl"),
  buttons(),
  current_button(0),
  show_tip(false),
  tip_button(0)
{
  ActionHolder* aholder = server->get_action_holder();

  std::vector<ActionName::Enum> actions = aholder->get_available_actions();

  set_rect(Rect(Vector2i(pos.x, pos.y - (static_cast<int>(actions.size()) * 38)/2),
                Size(60, static_cast<int>(actions.size()) * 38)));

  // Sort the action so that they always have the same order in the panel
  std::sort(actions.begin(), actions.end());

  for(std::vector<ActionName::Enum>::size_type i = 0; i < actions.size(); ++i)
  {
    ActionButton button;
    button.name   = actions[i];
    button.sprite = Sprite("pingus/player0/" + ActionName::to_string(button.name) + "/right");
    button.sprite.set_hotspot(origin_center, 0, 0);
    button.sprite.set_play_loop(true);
    buttons.push_back(button);
  }
}

ButtonPanel::~ButtonPanel()
{
}

void
ButtonPanel::draw(DrawingContext& gc)
{
  ActionHolder* aholder = server->get_action_holder();

  for(std::vector<ActionButton>::size_type i = 0; i < buttons.size(); ++i)
  {
    if (current_button == i)
      gc.draw(highlight, Vector2i(rect.left, rect.top + 38*static_cast<int>(i)));
    else
      gc.draw(background, Vector2i(rect.left, rect.top + 38*static_cast<int>(i)));

    gc.draw(buttons[i].sprite, Vector2i(rect.left + 20, rect.top + 38*static_cast<int>(i) + 16));

    if (globals::developer_mode)
    {
      std::ostringstream out;
      out << aholder->get_available(buttons[i].name)
          << " / "
          << aholder->get_used(buttons[i].name);
      gc.print_left(Fonts::pingus_small, Vector2i(rect.left + 46, rect.top + 5 + 38*static_cast<int>(i)), out.str());
    }
    else
    {
      std::string str = StringUtil::to_string(aholder->get_available(buttons[i].name));
      gc.print_center(Fonts::pingus_small, Vector2i(rect.left + 46, rect.top + 5 + 38*static_cast<int>(i)), str);
    }

    if (show_tip && tip_button == i)
    {
      gc.print_left(Fonts::pingus_small, Vector2i(rect.left + 65, rect.top + 5 + 38*static_cast<int>(i)),
                    ActionName::to_screenname(buttons[i].name));
    }
  }
}

void
ButtonPanel::update (float delta)
{
  for(std::vector<ActionButton>::size_type i = 0; i < buttons.size(); ++i)
    if (i == current_button)
      buttons[i].sprite.update(delta);
    else
      buttons[i].sprite.set_frame(0);
}

ActionName::Enum
ButtonPanel::get_action_name()
{
  if (!buttons.empty())
  {
    return buttons[current_button].name;
  }
  // No actions available
  return ActionName::WALKER;
}

void
ButtonPanel::set_button(int n)
{
  if (n >= 0 || n < static_cast<int>(buttons.size()-1))
  {
    current_button = static_cast<size_t>(n);
  }
  else
  {
    // FIXME: Play 'boing' sound here
  }
}

void
ButtonPanel::next_action()
{
  if (!buttons.empty())
  {
    current_button = (current_button + 1 + buttons.size()) % buttons.size();
  }
}

void
ButtonPanel::previous_action()
{
  if (!buttons.empty())
  {
    current_button = (current_button - 1 + buttons.size()) % buttons.size();
  }
}

void
ButtonPanel::on_primary_button_press(int x, int y)
{
  int action = (y - rect.top) / 38;
  current_button = static_cast<size_t>(Math::clamp(0, action, int(buttons.size() - 1)));
}

void
ButtonPanel::on_primary_button_release(int x, int y)
{
}

void
ButtonPanel::on_pointer_enter()
{
  show_tip = true;
}

void
ButtonPanel::on_pointer_leave()
{
  show_tip = false;
}

void
ButtonPanel::on_pointer_move(int x, int y)
{
  int action = (y - rect.top) / 38;
  tip_button = static_cast<size_t>(Math::clamp(0, action, int(buttons.size()-1)));
}

void
ButtonPanel::set_pos(const Vector2i& pos)
{
  set_rect(Rect(Vector2i(pos.x, pos.y - (static_cast<int>(buttons.size()) * 38)/2),
                Size(60, static_cast<int>(buttons.size()) * 38)));
}

/* EOF */
