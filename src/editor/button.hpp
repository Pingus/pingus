/*  $Id$
**   __      __ __             ___        __   __ __   __
**  /  \    /  \__| ____    __| _/_______/  |_|__|  | |  |   ____
**  \   \/\/   /  |/    \  / __ |/  ___/\   __\  |  | |  | _/ __ \
**   \        /|  |   |  \/ /_/ |\___ \  |  | |  |  |_|  |_\  ___/
**    \__/\  / |__|___|  /\____ /____  > |__| |__|____/____/\___  >
**         \/          \/      \/    \/                         \/
**  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
** 
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
**  02111-1307, USA.
*/

#ifndef HEADER_BUTTON_HPP
#define HEADER_BUTTON_HPP

#include <boost/signal.hpp>
#include "gui/rect_component.hpp"

namespace Editor {

/** */
class Button : public GUI::RectComponent
{
private:
  std::string text;
  bool mouse_over;
  bool mouse_down;
  bool enabled;

public:
  Button(const Rect& rect, const std::string& text);

  void draw (DrawingContext& gc);
  void update (float delta);
  void update_layout() {}
  
  void on_pointer_enter();
  void on_pointer_leave();
  void on_primary_button_press(int x, int y);
  void on_primary_button_release(int x, int y);

  void enable()  { enabled = true; }
  void disable() { enabled = false; }

  boost::signal<void()> on_click;

private:
  Button (const Button&);
  Button& operator= (const Button&);
};

} // namespace Editor

#endif

/* EOF */
