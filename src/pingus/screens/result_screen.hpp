// Pingus - A free Lemmings clone
// Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_SCREENS_RESULT_SCREEN_HPP
#define HEADER_PINGUS_PINGUS_SCREENS_RESULT_SCREEN_HPP

#include "engine/screen/gui_screen.hpp"
#include "pingus/result.hpp"

namespace GUI {
class SurfaceButton;
} // namespace GUI

class ResultScreen : public GUIScreen
{
private:
  Result result;
  GUI::SurfaceButton*   ok_button;
  GUI::SurfaceButton* abort_button;
  GUI::SurfaceButton* retry_button;

public:
  ResultScreen(Result result);
  void on_startup();

  void on_pause_press();
  void on_fast_forward_press();
  void on_escape_press();

  void retry_level();
  void close_screen();

  void resize(const Size&);
private:
  ResultScreen (const ResultScreen&);
  ResultScreen& operator= (const ResultScreen&);
};

#endif

/* EOF */
