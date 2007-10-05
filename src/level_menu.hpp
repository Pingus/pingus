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

#ifndef HEADER_LEVEL_MENU_HPP
#define HEADER_LEVEL_MENU_HPP

#include "sprite.hpp"
#include "screen/gui_screen.hpp"

/** */
class LevelMenu : public GUIScreen
{
private:
  Sprite background;
  Sprite ok_button;
  Sprite marker;

  struct LevelsetEntry
  {
    std::string title;
    std::string description;

    int completion;
    int number_of_levels;

    LevelsetEntry(const std::string& title,
                  const std::string& description,
                  int completion,
                  int number_of_levels)
      : title(title),
        description(description),
        completion(completion),
        number_of_levels(number_of_levels)
    {}
  };

  typedef std::vector<LevelsetEntry> Levelsets;
  Levelsets levelsets;

public:
  LevelMenu();
  ~LevelMenu();
  
  void draw_background (DrawingContext& gc);
  void update (const GameDelta& delta);
  void on_escape_press ();

private:
  LevelMenu (const LevelMenu&);
  LevelMenu& operator= (const LevelMenu&);
};

#endif

/* EOF */
