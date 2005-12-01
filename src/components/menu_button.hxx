//  $Id: menu_button.hxx,v 1.8 2003/12/14 00:30:04 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_MENU_BUTTON_HXX
#define HEADER_PINGUS_MENU_BUTTON_HXX

#include <string>
#include <ClanLib/Display/sprite.h>
#include <ClanLib/Display/font.h>
#include "../gui/component.hxx"

namespace Pingus {

class PingusMenu;

/** Framework for menu buttons */
class MenuButton : public GUI::Component
{
protected:
  CL_Sprite surface_p;
  CL_Font     font;
  CL_Font     font_large;

  int x_pos;
  int y_pos;

  std::string desc;
  std::string text;

  bool mouse_over;
  bool pressed;

  CL_Signal_v0 click;

public:
  MenuButton(const CL_Point& pos, const CL_Sprite& sprite, 
             const std::string& text, const std::string& desc);
  MenuButton();
  virtual ~MenuButton();

  void draw(DrawingContext& gc);
  void update (float delta);

  bool is_at (int x, int y);

  void on_pointer_enter ();
  void on_pointer_leave ();
  void on_pointer_press ();
  void on_pointer_release ();

  void on_primary_button_click (int x, int y) { on_click (); UNUSED_ARG(x); UNUSED_ARG(y); }

  virtual void on_click ();
  CL_Signal_v0& sig_click() { return click; }
private:
  MenuButton (const MenuButton&);
  MenuButton& operator= (const MenuButton&);
};

} // namespace Pingus

#endif

/* EOF */
