//  $Id: capture_rectangle.cxx,v 1.17 2003/10/22 11:11:22 grumbel Exp $
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

#include <ClanLib/Display/font.h>
#include <ClanLib/Core/System/clanstring.h>
#include "vector.hxx"
#include "pingu.hxx"
#include "resource.hxx"
#include "capture_rectangle.hxx"
#include "pingu_action.hxx"
#include "components/button_panel.hxx"
#include "globals.hxx"
#include "display/scene_context.hxx"
#include "fonts.hxx"

namespace Pingus {

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
          sc.color().draw(good, pingu->get_center_pos() + Vector(0, 0, 1000));
        }
      else
        {
          sc.color().draw(bad, pingu->get_center_pos() + Vector(0, 0, 1000));
        }
      
      // Paint the direction arrow
      if (pingu->direction.is_left())
        {
          sc.color().draw(arrow_left, pingu->get_center_pos() + Vector(0, 0, 1000));
        }
      else
        {
          sc.color().draw(arrow_right, pingu->get_center_pos() + Vector(0, 0, 1000));
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
          action_str += CL_String::to(pingu->get_id());
        }
    }
}

} // namespace Pingus

/* EOF */
