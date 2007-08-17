//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#include "math/vector3f.hpp"
#include "string_util.hpp"
#include "pingu.hpp"
#include "resource.hpp"
#include "capture_rectangle.hpp"
#include "pingu_action.hpp"
#include "components/button_panel.hpp"
#include "globals.hpp"
#include "display/scene_context.hpp"
#include "fonts.hpp"

CaptureRectangle::CaptureRectangle (ButtonPanel* arg_button_panel)
  : pingu(0),
    owner_id(0),
    good(Resource::load_sprite("game/cursors/capgood")),
    bad(Resource::load_sprite("game/cursors/capbad")),
    arrow_left(Resource::load_sprite("game/cursors/arrow_left")),
    arrow_right(Resource::load_sprite("game/cursors/arrow_right")),
    button_panel(arg_button_panel),
    font(Fonts::courier_small)
{
}

CaptureRectangle::~CaptureRectangle ()
{
}

void
CaptureRectangle::draw(SceneContext& sc)
{
  if (pingu && pingu->catchable())
    {
      // Draw the capture rectangle
      if (pingu->change_allowed(button_panel->get_action_name()))
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
          sc.color().draw(arrow_left, pingu->get_center_pos() + Vector3f(0, 0, 1000));
        }
      else
        {
          sc.color().draw(arrow_right, pingu->get_center_pos() + Vector3f(0, 0, 1000));
        }

      sc.color().print_center(font, 
                              pingu->get_center_pos().x,
                              pingu->get_center_pos().y - 32,
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

      if (maintainer_mode)
        {
          action_str += " Id: ";
          action_str += StringUtil::to_string(pingu->get_id());
        }
    }
}

/* EOF */
