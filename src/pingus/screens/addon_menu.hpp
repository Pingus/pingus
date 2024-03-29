// Pingus - A free Lemmings clone
// Copyright (C) 2007 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_PINGUS_SCREENS_ADDON_MENU_HPP
#define HEADER_PINGUS_PINGUS_SCREENS_ADDON_MENU_HPP

#include "engine/display/sprite.hpp"
#include "engine/screen/gui_screen.hpp"

namespace pingus {

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

    AddOnEntry(std::string const& title_,
               std::string const& description_,
               std::string const& author_)
      : title(title_),
        description(description_),
        author(author_)
    {}
  };

public:
  AddOnMenu();
  ~AddOnMenu() override;

  void draw_background (DrawingContext& gc) override;
  void on_escape_press() override;

private:
  AddOnMenu (AddOnMenu const&);
  AddOnMenu& operator= (AddOnMenu const&);
};

} // namespace pingus

#endif

/* EOF */
