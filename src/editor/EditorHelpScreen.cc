//  $Id: EditorHelpScreen.cc,v 1.7 2002/06/10 11:00:28 torangan Exp $
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

#include <ClanLib/Display/Display/display.h>
#include <ClanLib/Display/Font/font.h>
#include "../my_gettext.hh"
#include "../PingusResource.hh"
#include "EditorHelpScreen.hh"

EditorHelpScreen::EditorHelpScreen ()
  : font (PingusResource::load_font("Fonts/pingus_small", "fonts")),
    large_font (PingusResource::load_font("Fonts/pingus", "fonts"))
{
  
}

void
EditorHelpScreen::draw ()
{
  CL_Display::fill_rect (0,0, CL_Display::get_width (), CL_Display::get_height (),
			 0.0f, 0.0f, 0.0f, 0.5f);
  large_font->print_center (CL_Display::get_width ()/2, 32, "Helpscreen (hide with F1)");

  int x_pos;
  int y_pos;

  x_pos = 32;
  y_pos = 100;
  font->print_left (x_pos, y_pos +   0, _("F1 - show/hide this help screen"));
  font->print_left (x_pos, y_pos +  20, _("F2 - [unset]"));
  font->print_left (x_pos, y_pos +  40, _("F3 - toggle background color"));
  font->print_left (x_pos, y_pos +  60, _("F4 - play/test the level"));
  font->print_left (x_pos, y_pos +  80, _("F5 - load a level"));
  font->print_left (x_pos, y_pos + 100, _("F6 - save this level"));
  font->print_left (x_pos, y_pos + 120, _("F7 - [unset]"));
  font->print_left (x_pos, y_pos + 140, _("F8 - quick save/backup save?!"));
  font->print_left (x_pos, y_pos + 160, _("F9 - change level width and height"));
  font->print_left (x_pos, y_pos + 180, _("F10 - set/configure background"));
  font->print_left (x_pos, y_pos + 200, _("F11 - toggle fps counter"));
  font->print_left (x_pos, y_pos + 220, _("F12 - make screenshot"));

  x_pos = CL_Display::get_width()/2 - 32;
  y_pos = 100;
  font->print_left (x_pos, y_pos +   0, _("Pos1 - increase object size"));
  font->print_left (x_pos, y_pos +  20, _("End  - decrease object size"));
  font->print_left (x_pos, y_pos +  40, _("Cursor Keys - Move object"));
  font->print_left (x_pos, y_pos +  60, _("Shift 'Cursor Keys' - Move objects fast "));
  font->print_left (x_pos, y_pos +  80, _("PageUp   - level object up"));
  font->print_left (x_pos, y_pos + 100, _("PageDown - level object down"));
  font->print_left (x_pos, y_pos + 120, _("Shift PageUp   - increase objects z-pos by 50"));
  font->print_left (x_pos, y_pos + 140, _("Shift PageDown - decrease objects z-pos by 50"));
  font->print_left (x_pos, y_pos + 160, _("Enter - Set default zoom (1:1)"));
  font->print_left (x_pos, y_pos + 180, _("d - duplicate object"));
  font->print_left (x_pos, y_pos + 200, _("a - mark all objects"));
  font->print_left (x_pos, y_pos + 220, _("shift leftmouseclick - add object to selection"));
  font->print_left (x_pos, y_pos + 240, _("leftmouseclick - select object"));
  font->print_left (x_pos, y_pos + 260, _("Insert - insert selected object"));
  font->print_left (x_pos, y_pos + 280, _("Remove - remove selected object"));
  font->print_left (x_pos, y_pos + 300, _("g - ungroup/group current selection"));
  font->print_left (x_pos, y_pos + 320, _("Ctrl PageUp   - increase objects z-pos by 1"));
  font->print_left (x_pos, y_pos + 340, _("Ctrl PageDown - decrease objects z-pos by 1"));
  //font->print_left (x_pos, y_pos + 320, _(""));
  //font->print_left (x_pos, y_pos + 340, _(""));
  
  x_pos = 32;
  y_pos = 500;
  font->print_left (x_pos, y_pos + 0, _("Naming Convention: <LEVELNAME><NUMBER>-<CREATOR>.xml"));

  font->print_left (x_pos, y_pos + 30, _("When you have create a level and want to have it in the next Pingus release,\n"
					 "please mail it to grumbel@gmx.de. "));
}

void
EditorHelpScreen::update (float /*delta*/)
{
}

/* EOF */
