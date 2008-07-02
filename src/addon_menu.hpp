//  $Id: addon_menu.hpp 3623 2008-07-02 17:59:58Z grumbel $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_ADDON_MENU_HPP
#define HEADER_ADDON_MENU_HPP

#include "sprite.hpp"
#include "screen/gui_screen.hpp"

/** */
class AddOnMenu : public GUIScreen
{
private:
  Sprite background;
  Sprite ok_button;

  struct AddOnEntry
  {
    std::string title;
    std::string description;
    std::string author;

    AddOnEntry(const std::string& title,
               const std::string& description,
               const std::string& author)
      : title(title),
        description(description),
        author(author)     
    {}
  };

public:
  AddOnMenu();
  ~AddOnMenu();
  
  void draw_background (DrawingContext& gc);
  void on_escape_press ();

private:
  AddOnMenu (const AddOnMenu&);
  AddOnMenu& operator= (const AddOnMenu&);
};

#endif

/* EOF */
