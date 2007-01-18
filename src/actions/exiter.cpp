//  $Id: exiter.cxx,v 1.20 2003/10/21 11:01:52 grumbel Exp $
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

#include "../math/vector3f.hpp"
#include "../display/scene_context.hxx"
#include "../resource.hxx"
#include "../pingu.hxx"
#include "../sound/sound.hxx"
#include "exiter.hxx"

namespace Actions {

Exiter::Exiter (Pingu* p)
  : PinguAction(p),
    sound_played(false)
{
  sprite.load(Direction::LEFT,  Resource::load_sprite("pingus/player" + 
    pingu->get_owner_str() + "/exit/left"));
  sprite.load(Direction::RIGHT, Resource::load_sprite("pingus/player" + 
    pingu->get_owner_str() + "/exit/right"));
}

void
Exiter::update ()
{
  sprite[pingu->direction].update();

  if (!sound_played)
    {
      sound_played = true;
      Sound::PingusSound::play_sound("yipee");
    }

  if (sprite[pingu->direction].is_finished())
    {
      if (pingu->get_status() != PS_EXITED)
	{
	  pingu->set_status(PS_EXITED);
	}
    }
}

void
Exiter::draw (SceneContext& gc)
{
  gc.color().draw(sprite[pingu->direction], pingu->get_pos());
}

} // namespace Actions

/* EOF */
