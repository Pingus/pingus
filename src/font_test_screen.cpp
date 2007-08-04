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

#include "fonts.hpp"
#include "string_util.hpp"
#include "display/drawing_context.hpp"
#include "font_description.hpp"
#include "font_test_screen.hpp"

FontTestScreen::FontTestScreen(const std::string& fontfile)
  : scrollx(0),
    scrolly(0)
{
  std::cout << "### Loading font file: " << fontfile << std::endl;
  font = Font(FontDescription(fontfile));
  reference = Font(FontDescription("images/fonts/reference-iso-8859-1.font"));
}

bool
FontTestScreen::draw(DrawingContext& gc)
{
  float checker = 40.f;
  for(int y = 0; y < gc.get_height()/checker; y += 1)
    for(int x = 0; x < gc.get_width()/checker; x += 1)
      {
        if ((x+y) % 2 != 0)
          gc.draw_fillrect(x*checker, y*checker,
                           x*checker + checker, y*checker + checker,
                           Color(50, 50, 50));
        else
          gc.draw_fillrect(x*checker, y*checker,
                           x*checker + checker, y*checker + checker,
                           Color(0, 0, 0));          
      }

  gc.print_left(Fonts::chalk_large, 10, 10, "Pingus - Font Test");

  gc.push_modelview();
  gc.translate(scrollx, scrolly);

  for(int i = 0; i < 256; ++i)
    {
      float x = 64.0f + (i%20)*(font.get_height() + 12);
      float y = 64.0f + (i/20)*(font.get_height() + 12);

      gc.print_right(reference,
                    x + font.get_height(), y,
                    std::string(1, char(i)));

      gc.print_right(reference,
                    x + font.get_height(), 
                     y + font.get_height() - reference.get_height(),
                     StringUtil::to_string(i));
      
      if (font.get_width(char(i)))
        {
          gc.draw_rect(x, y, 
                       x+font.get_height(), 
                       y+font.get_height(), 
                       Color(0,0,255));
          gc.draw_rect(x, y, 
                       x+font.get_width(char(i)), 
                       y+font.get_height(), 
                       Color(255,0,255));
          gc.print_left(font,
                        x, y,
                        std::string(1, char(i)));
        }
      else
        {
          gc.draw_rect(x, y, 
                       x+font.get_height(), 
                       y+font.get_height(), 
                       Color(255,0,0));
        }
    }
  gc.pop_modelview();

  return true;
}

void
FontTestScreen::update (const GameDelta& delta)
{
  const Input::EventLst& events = delta.get_events ();

  for (Input::EventLst::const_iterator i = events.begin ();
       i != events.end ();
       ++i)
    {
      switch (i->type)
	{
	case Input::ScrollEventType:
          scrollx += i->scroll.x_delta;
          scrolly += i->scroll.y_delta;
	  break;
          
        default:
          break;
        }
    } 
}

/* EOF */
