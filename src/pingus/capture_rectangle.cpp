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

#include "pingus/capture_rectangle.hpp"

#include "engine/display/scene_context.hpp"
#include "pingus/fonts.hpp"
#include "pingus/globals.hpp"
#include "pingus/pingu.hpp"
#include "pingus/pingu_action.hpp"
#include "pingus/screens/game_session.hpp"
#include "util/string_util.hpp"

CaptureRectangle::CaptureRectangle(GameSession* session_) :
  session(session_),
  pingu(0),
  action_str(),
  owner_id(0),
  good("game/cursors/capgood"),
  bad("game/cursors/capbad"),
  arrow_left("game/cursors/arrow_left"),
  arrow_right("game/cursors/arrow_right"),
  font(Fonts::courier_small)
{
}

CaptureRectangle::~CaptureRectangle()
{
}

void
CaptureRectangle::draw(SceneContext& sc)
{
  if (pingu && pingu->catchable())
  {
    // Draw the capture rectangle
    if (session && pingu->change_allowed(session->get_action_name()))
    {
      sc.color().draw(good, pingu->get_center_pos() + Vector3f(0, 0, 1000));
    }
    else
    {
      sc.color().draw(bad, pingu->get_center_pos() + Vector3f(0, 0, 1000));
    }

    // Paint the direction arrow
    if (pingu->direction.is_left())
    {
      sc.color().draw(arrow_left, pingu->get_center_pos() + Vector3f(0, 2, 1000));
    }
    else
    {
      sc.color().draw(arrow_right, pingu->get_center_pos() + Vector3f(0, 2, 1000));
    }

    sc.color().print_center(font,
                            Vector2i(static_cast<int>(pingu->get_center_pos().x),
                                     static_cast<int>(pingu->get_center_pos().y - 46)),
                            action_str,
                            1000);
  }
}

void
CaptureRectangle::set_pingu (Pingu* p)
{
  pingu = p;

  if (pingu)
  {
    action_str = pingu->get_name();

    if (pingu->get_wall_action() || pingu->get_fall_action())
    {
      action_str += "[";

      if (pingu->get_wall_action())
        action_str += pingu->get_wall_action()->get_persistent_char();

      if (pingu->get_fall_action())
        action_str += pingu->get_fall_action()->get_persistent_char();

      action_str += "]";
    }

    if (globals::developer_mode)
    {
      action_str += " Id: ";
      action_str += StringUtil::to_string(pingu->get_id());
    }
  }
}

/* EOF */
