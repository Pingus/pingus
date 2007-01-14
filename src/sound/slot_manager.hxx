//  $Id: slot_manager.hxx,v 1.3 2003/10/21 11:01:52 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_SLOT_MANAGER_HXX
#define HEADER_PINGUS_SLOT_MANAGER_HXX

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
