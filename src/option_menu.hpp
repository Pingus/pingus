//  $Id$
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_OPTION_MENU_HPP
#define HEADER_OPTION_MENU_HPP

#include "sprite.hpp"
#include "screen/gui_screen.hpp"
#include "components/label.hpp"
#include "gui/rect_component.hpp"

class CheckBox;

/** */
class OptionMenu : public GUIScreen
{
private:
  Sprite background;
  Sprite ok_button;
  int x_pos;
  int y_pos;

  struct Option {
    Label*         label;
    GUI::RectComponent* control;

    Option(Label* label, GUI::RectComponent* control)
      : label(label), control(control)
    {}
  };

  typedef std::vector<Option> Options;
  Options options;

  CheckBox* fullscreen_box;
  CheckBox* swcursor_box;
  
public:
  OptionMenu();
  ~OptionMenu();
  
  void draw_background (DrawingContext& gc);
  void update (const GameDelta& delta);
  void on_escape_press ();

  void add_item(const std::string& label, GUI::RectComponent* control);

  void resize(const Size&);

  void on_swcursor_change(bool v);
  void on_fullscreen_change(bool v);
private:
  OptionMenu (const OptionMenu&);
  OptionMenu& operator= (const OptionMenu&);
};

#endif

/* EOF */
