//  $Id: editor_help_screen.cxx,v 1.14 2003/10/18 23:17:27 grumbel Exp $
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

#include <ClanLib/display.h>
#include <ClanLib/core.h>
#include "../gettext.h"
#include "../pingus_resource.hxx"
#include "../fonts.hxx"
#include "editor_help_screen.hxx"

namespace Pingus {
namespace EditorNS {

EditorHelpScreen::EditorHelpScreen ()
  : font (Fonts::pingus_small),
    large_font (Fonts::pingus_large)
{

}

void
EditorHelpScreen::draw ()
{
  CL_Display::fill_rect (CL_Rect(25,0, CL_Display::get_width (), CL_Display::get_height ()),
			 CL_Color(0, 0, 0, 255));
  large_font.set_alignment(origin_center);
  large_font.draw(CL_Display::get_width ()/2, 32, _("Editor Helpscreen (hide with F1)"));

  int x_pos;
  int y_pos;

  x_pos = 64;
  y_pos = 100;
  large_font.set_alignment(origin_bottom_left);
  font.draw(x_pos, y_pos +   0, _("F1 - show/hide this help screen"));
  font.draw(x_pos, y_pos +  20, _("F2 - launch level building tutorial"));
  font.draw(x_pos, y_pos +  40, _("F3 - toggle background color"));
  font.draw(x_pos, y_pos +  60, _("F4 - play/test the level"));
  font.draw(x_pos, y_pos +  80, _("F5 - load a level"));
  font.draw(x_pos, y_pos + 100, _("F6 - save this level"));
  font.draw(x_pos, y_pos + 120, _("F7 - [unset]"));
  font.draw(x_pos, y_pos + 140, _("F8 - quick save/backup save?!"));
  font.draw(x_pos, y_pos + 160, _("F9 - change level width and height"));
  font.draw(x_pos, y_pos + 180, _("F10 - [unset]"));
  font.draw(x_pos, y_pos + 200, _("F11 - toggle fps counter"));
  font.draw(x_pos, y_pos + 220, _("F12 - make screenshot"));

  x_pos = CL_Display::get_width()/2 - 32;
  y_pos = 100;
  font.draw(x_pos, y_pos +   0, _("Home - increase object size"));
  font.draw(x_pos, y_pos +  20, _("End  - decrease object size"));
  font.draw(x_pos, y_pos +  40, _("Cursor Keys - Move object"));
  font.draw(x_pos, y_pos +  60, _("Shift 'Cursor Keys' - Move objects fast "));
  font.draw(x_pos, y_pos +  80, _("PageUp   - level object up"));
  font.draw(x_pos, y_pos + 100, _("PageDown - level object down"));
  font.draw(x_pos, y_pos + 120, _("Shift PageUp   - increase objects z-pos by 50"));
  font.draw(x_pos, y_pos + 140, _("Shift PageDown - decrease objects z-pos by 50"));
  font.draw(x_pos, y_pos + 160, _("Enter - Set default zoom (1:1)"));
  font.draw(x_pos, y_pos + 180, _("d - duplicate object"));
  font.draw(x_pos, y_pos + 200, _("a - mark all objects"));
  font.draw(x_pos, y_pos + 220, _("shift leftmouseclick - add object to selection"));
  font.draw(x_pos, y_pos + 240, _("leftmouseclick - select object"));
  font.draw(x_pos, y_pos + 260, _("Insert - insert new object"));
  font.draw(x_pos, y_pos + 280, _("Remove - remove selected object"));
  font.draw(x_pos, y_pos + 300, _("g - ungroup/group current selection"));
  font.draw(x_pos, y_pos + 320, _("Ctrl PageUp   - increase objects z-pos by 1"));
  font.draw(x_pos, y_pos + 340, _("Ctrl PageDown - decrease objects z-pos by 1"));
  //font.draw(x_pos, y_pos + 320, _(""));
  //font.draw(x_pos, y_pos + 340, _(""));

  x_pos = 64;
  y_pos = 500;
  font.draw(x_pos, y_pos + 0, _("Naming Convention: <LEVELNAME><NUMBER>-<CREATOR>.pingus"));

  font.draw(x_pos, y_pos + 30, _("When you have created a level and want to have it in the next Pingus release,\n"
					 "please mail it to pingus-devel@nongnu.org."));
}

void
EditorHelpScreen::update (float /*delta*/)
{
}

} // namespace EditorNS
} // namespace Pingus

/* EOF */
