/*
**  Windstille - A Sci-Fi Action-Adventure Game
**  Copyright (C) 2018 Ingo Ruhnke <grumbel@gmail.com>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iostream>

#include <al.h>

#include <wstsound/openal_system.hpp>
#include <wstsound/openal_real_device.hpp>
#include <wstsound/effect_slot.hpp>

using namespace wstsound;

int main(int argc, char** argv)
{
  OpenALSystem openal;
  auto& device = openal.open_real_device();

  openal.print_openal_version(std::cout);

  if (device.is_extension_present("ALC_EXT_EFX"))
  {
    std::cout << "EFX not supported" << std::endl;
  }

  { // check how many EffectsSlots there are
    int max_effect_slots = 0;
    try {
      std::vector<EffectSlotPtr> slots;
      for (int i = 0; i < 1000; ++i) {
        slots.emplace_back(std::make_unique<EffectSlot>());
        max_effect_slots += 1;
      }
    } catch (...) {}

    std::cout << "Effects Slots: " << max_effect_slots << std::endl;
  }

  std::cout << "Max Auxiliary Sends: " << device.max_auxiliary_sends() << std::endl;

  return 0;
}

/* EOF */
