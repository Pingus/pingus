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

#ifndef HEADER_INPUTBOX_HPP
#define HEADER_INPUTBOX_HPP

#include <boost/signal.hpp>
#include "gui/rect_component.hpp"

namespace Editor {

/** */
class Inputbox : public GUI::RectComponent
{
private:
  std::string text;
  
public:
  Inputbox(const Rect& rect);

  void draw(DrawingContext& gc);
  
  void set_text(const std::string& text);
  std::string get_text() const { return text; }
  void on_key_pressed(const unsigned short c);

  void update_layout() {}

  boost::signal<void (const std::string&)> on_change;
private:
  Inputbox (const Inputbox&);
  Inputbox& operator= (const Inputbox&);
};

} // namespace Editor

#endif

/* EOF */
