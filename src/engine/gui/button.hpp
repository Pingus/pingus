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

#ifndef HEADER_PINGUS_ENGINE_GUI_BUTTON_HPP
#define HEADER_PINGUS_ENGINE_GUI_BUTTON_HPP

namespace GUI {

class Button
{
protected:
  bool pressed;
  bool mouse_over;

public:
  void on_primary_button_press (int x, int y);
  void on_primary_button_release (int x, int y);

  void on_pointer_enter ();
  void on_pointer_leave ();
};

} // namespace GUI

#endif

/* EOF */
