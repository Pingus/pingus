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

#ifndef HEADER_PINGUS_ENGINE_SCREEN_GUI_SCREEN_HPP
#define HEADER_PINGUS_ENGINE_SCREEN_GUI_SCREEN_HPP

#include <memory>

#include "engine/screen/screen.hpp"

namespace GUI {
class GUIManager;
}

namespace Input {
struct ButtonEvent;
}

class GUIScreen : public Screen
{
protected:
  std::unique_ptr<GUI::GUIManager> gui_manager;

public:
  GUIScreen ();
  virtual ~GUIScreen ();

  /** Draw this screen */
  virtual void draw_foreground (DrawingContext&) {}
  virtual void draw_background (DrawingContext&) {}
  virtual void draw(DrawingContext& gc);

  virtual void update (const Input::Event& event);
  virtual void update (float);

  virtual void on_pause_press () {}
  virtual void on_single_step_press () {}
  virtual void on_fast_forward_press () {}
  virtual void on_armageddon_press () {}
  virtual void on_escape_press () {}
  virtual void on_action_up_press() {}
  virtual void on_action_down_press() {}

  virtual void on_pause_release () {}
  virtual void on_single_step_release () {}
  virtual void on_fast_forward_release () {}
  virtual void on_armageddon_release () {}
  virtual void on_escape_release () {}
  virtual void on_action_up_release() {}
  virtual void on_action_down_release() {}

  virtual void on_action_axis_move (float) {}

  virtual void resize(const Size& size);

private:
  void process_button_event (const Input::ButtonEvent& event);

  GUIScreen (const GUIScreen&);
  GUIScreen& operator= (const GUIScreen&);
};

#endif

/* EOF */
