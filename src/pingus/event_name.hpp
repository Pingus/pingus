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

#ifndef HEADER_PINGUS_PINGUS_EVENT_NAME_HPP
#define HEADER_PINGUS_PINGUS_EVENT_NAME_HPP

namespace pingus {

enum EventName {
  // Buttons
  PRIMARY_BUTTON,
  SECONDARY_BUTTON,
  PAUSE_BUTTON,
  FAST_FORWARD_BUTTON,
  SINGLE_STEP_BUTTON,
  ARMAGEDDON_BUTTON,
  ESCAPE_BUTTON,

  ACTION_1_BUTTON,
  ACTION_2_BUTTON,
  ACTION_3_BUTTON,
  ACTION_4_BUTTON,
  ACTION_5_BUTTON,
  ACTION_6_BUTTON,
  ACTION_7_BUTTON,
  ACTION_8_BUTTON,
  ACTION_9_BUTTON,
  ACTION_10_BUTTON,

  ACTION_UP_BUTTON,
  ACTION_DOWN_BUTTON,

  // Pointer
  STANDARD_POINTER,

  // Scroller
  STANDARD_SCROLLER,

  STANDARD_KEYBOARD,

  // Axis
  ACTION_AXIS
};

} // namespace pingus

#endif

/* EOF */
