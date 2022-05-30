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
#include "fwd.hpp"

namespace pingus {

class GUIScreen : public Screen
{
protected:
  std::unique_ptr<pingus::gui::GUIManager> gui_manager;

public:
  GUIScreen ();
  ~GUIScreen () override;

  /** Draw this screen */
  virtual void draw_foreground (DrawingContext&) {}
  virtual void draw_background (DrawingContext&) {}
  void draw(DrawingContext& gc) override;

  void update_input (pingus::input::Event const& event) override;
  void update(float delta) override;

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

  void resize(Size const& size) override;

private:
  void process_button_event (pingus::input::ButtonEvent const& event);

  GUIScreen (GUIScreen const&);
  GUIScreen& operator= (GUIScreen const&);
};

} // namespace pingus

#endif

/* EOF */
