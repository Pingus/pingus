// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
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

#include "engine/sound/sound_dummy.hpp"

#include "pingus/globals.hpp"
#include "util/log.hpp"

namespace Sound {

void
PingusSoundDummy::real_play_sound(const std::string & filename, float /*volume*/, float /*panning*/)
{
  log_info("PingusSoundDummy::real_play_sound: %1%", filename);
}

void
PingusSoundDummy::real_play_music(const std::string & filename, float /*volume*/, bool /*loop*/)
{
  log_info("PingusSoundDummy::real_play_music: %1%", filename);
}

void
PingusSoundDummy::real_stop_music()
{
}

} // namespace Sound

/* EOF */

