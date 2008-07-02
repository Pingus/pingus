
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

#ifndef HEADER_PINGUS_SLOT_MANAGER_HPP
#define HEADER_PINGUS_SLOT_MANAGER_HPP

#include <string>
#include <vector>
#include <map>

#include <ClanLib/sound.h>

namespace Sound {

struct SlotEntry
{
  /** Name of the sound effect */
  std::string name;

  /** Time when the last sound playback started */
  unsigned int last_time;

  /** The data of this sound effect */
  CL_SoundBuffer* buffer;

  /** Currently active sessinos playing the sound effect */
  std::vector<CL_SoundBuffer_Session> sessions;
};

/** Manager class for sound slots, if a sound effect gets played it
    will be placed in a slot, if the same effect gets played again and
    the slot is still busy it won't get played again */
class SlotManager
{
private:
  std::map<std::string, SlotEntry> slots;

public:
  SlotManager();

  void play_sound(const std::string& name);

private:
  SlotEntry& find_slot(const std::string& name);

  SlotManager (const SlotManager&);
  SlotManager& operator= (const SlotManager&);
};

} // namespace Sound

#endif

/* EOF */
