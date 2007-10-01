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

#include "resource.hpp"
#include "screen/screen_manager.hpp"
#include "fonts.hpp"
#include "display/drawing_context.hpp"
#include "option_menu.hpp"

/*
  - option menu:

  ----------------------------------
  Language:                 [German]
  ----------------------------------
  Fast-Mode:                  Yes/No
  Fullscreen:                 Yes/No
  Allow Window Resize:        Yes/No
  Resolution:              [800x600]
  Frame Skip:              [Integer]
  Software Cursor:          [Yes/No] (could add different styled ones)
  ----------------------------------
  Master Volume:       [#######    ]
  Music Volume:        [####       ]
  Sound FX Volume:     [########## ]
  ----------------------------------
  Scroll Mode: [DragDrop/Rubberband]
  Enable Autoscrolling: [always, only in fullscreen, never]
  ----------------------------------

  [reset to defaults]
*/

OptionMenu::OptionMenu()
{
  background = Resource::load_sprite("core/menu/filedialog");
}

OptionMenu::~OptionMenu()
{
}
  
void
OptionMenu::draw_background(DrawingContext& gc)
{
  // gc.draw_fillrect(Rect(100, 100, 400, 400), Color(255, 0, 0));
  gc.draw(background, gc.get_width()/2 - background.get_width()/2, gc.get_height()/2 - background.get_height()/2);

  gc.print_center(Fonts::chalk_large, gc.get_width()/2, 90, "Option Menu");
  
  std::vector<std::string> strs;
  strs.push_back("Resolution:");
  strs.push_back("Fullscreen:");
  strs.push_back("Allow Resize:");
  strs.push_back("Fast Mode:");
  strs.push_back("Master Volume:");
  strs.push_back("Sound Volume:");
  strs.push_back("Music Volume:");

  int y = 130;
  for(std::vector<std::string>::iterator i = strs.begin(); i != strs.end(); ++i)
    {
      gc.print_left(Fonts::chalk_normal,  120, y += 32, *i);
      gc.print_right(Fonts::chalk_normal, gc.get_width()/2 - 32, y, "[X]");
    }

  std::vector<std::string> strs2;
  strs2.push_back("Software Cursor:");
  strs2.push_back("Scroll Mode:");
  strs2.push_back("Frame Skip:");

  y = 130;
  for(std::vector<std::string>::iterator i = strs2.begin(); i != strs2.end(); ++i)
    {
      gc.print_left(Fonts::chalk_normal,  gc.get_width()/2 + 32, y += 32, *i);
      gc.print_right(Fonts::chalk_normal, gc.get_width()/2 + 280, y, "[X]");
    }
}

void
OptionMenu::on_escape_press()
{
  std::cout << "OptionMenu: poping screen" << std::endl;
  ScreenManager::instance()->pop_screen();
}

/* EOF */
