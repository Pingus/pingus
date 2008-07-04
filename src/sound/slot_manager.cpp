//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "slot_manager.hpp"

namespace Sound {

SlotEntry&
SlotManager::find_slot(const std::string& name)
{

}

void
SlotManager::play_sound(const std::string& name)
{
  CL_SoundBuffer         * buffer;
  CL_SoundBuffer_Session sess;

  try {
    buffer = new CL_SoundBuffer (name.c_str());
    sess   = buffer -> prepare();
  } catch (const CL_Error & e) {
    perr(PINGUS_DEBUG_SOUND) << "Can't open file " << filename << " -- skipping\n"
			     << "  CL_Error: " << e.message << std::endl;
    return;
  }

  sess.set_volume(volume);
  sess.set_pan(panning);
  sess.set_looping(false);
  sess.play();
}

} // namespace Sound

/* EOF */
