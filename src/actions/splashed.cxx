//  $Id: splashed.cxx,v 1.5 2002/09/04 14:55:12 torangan Exp $
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

#include <ClanLib/Core/Math/cl_vector.h>
#include "../world.hxx"
#include "../pingu.hxx"
#include "../sound.hxx"
#include "../worldobj.hxx"
#include "splashed.hxx"

namespace Actions {

  Splashed::Splashed (void)
  {
  }

  void
  Splashed::init (void)
  {
    sprite = Sprite("Pingus/splat0", "pingus", 30.0f,
		    Sprite::NONE, Sprite::ONCE);
    sprite.set_align_center_bottom();
  
    sound_played    = false;
    particle_thrown = false;
  }

  void
  Splashed::update (float delta)
  {
    sprite.update (delta);

    if (!particle_thrown)
      {
        particle_thrown = true;
        WorldObj::get_world()->play_wav("sounds/splash.wav", pingu->get_pos());
      }

    if (sprite.finished())
    {
      pingu->set_status(PS_DEAD);
    }
  }

  void 
  Splashed::draw_offset (int x_of, int y_of, float s)
  {
    sprite.put_screen (static_cast<int>(pingu->get_x() + x_of),
		       static_cast<int>(pingu->get_y() + y_of));
    UNUSED_ARG(s);
  }

}

/* EOF */
