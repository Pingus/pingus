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

#include "pingus/actions/exiter.hpp"

#include "engine/display/scene_context.hpp"
#include "engine/sound/sound.hpp"
#include "pingus/pingu.hpp"

namespace Actions {

Exiter::Exiter (Pingu* p) :
  PinguAction(p),
  sprite(),
  sound_played(false)
{
  sprite.load(Direction::LEFT,  Sprite("pingus/player" +
                                       pingu->get_owner_str() + "/exit/left"));
  sprite.load(Direction::RIGHT, Sprite("pingus/player" +
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
    if (pingu->get_status() != Pingu::PS_EXITED)
    {
      pingu->set_status(Pingu::PS_EXITED);
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
