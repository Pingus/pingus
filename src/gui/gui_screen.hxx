//  $Id: gui_screen.hxx,v 1.3 2003/10/18 23:17:28 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_GUISCREEN_HXX
#define HEADER_PINGUS_GUISCREEN_HXX

#include "screen.hxx"

namespace Pingus {

namespace GUI {
  class GUIManager;
}

namespace Input {
  struct ButtonEvent;
}

class GUIScreen : public Screen
{
protected:
  GUI::GUIManager* gui_manager;

public:
  GUIScreen ();
  virtual ~GUIScreen ();

  /** Draw this screen */
  virtual void draw_foreground (DrawingContext& gc) { UNUSED_ARG(gc); }
  virtual void draw_background (DrawingContext& gc) { UNUSED_ARG(gc); }
  virtual bool draw (DrawingContext& gc);

  /** Pass a game delta to the screen */
  virtual void update (const GameDelta& delta);

  /** */
  virtual void update (float) {}

  virtual void on_pause_press () {}
  virtual void on_fast_forward_press () {}
  virtual void on_armageddon_press () {}
  virtual void on_escape_press () {}

  virtual void on_pause_release () {}
  virtual void on_fast_forward_release () {}
  virtual void on_armageddon_release () {}
  virtual void on_escape_release () {}

  virtual void on_action_axis_move (float) {}

private:
  void process_button_event (const Input::ButtonEvent& event);

  GUIScreen (const GUIScreen&);
  GUIScreen& operator= (const GUIScreen&);
};

} // namespace Pingus

#endif

/* EOF */
