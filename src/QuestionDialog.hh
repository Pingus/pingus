//  $Id: QuestionDialog.hh,v 1.5 2001/05/18 19:17:08 grumbel Exp $
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

#ifndef QUESTIONDIALOG_HH
#define QUESTIONDIALOG_HH

#include <string>

#include <ClanLib/core.h>
#include "my_gettext.hh"


///
class QuestionDialog
{
private:
  ///
  CL_Font* font;
  ///
  int buttons;
  ///
  std::string question, button1, button2, button3, button4;
public:
  ///
  QuestionDialog(std::string q, std::string b1 = _("Yes"), std::string b2 = _("No"), std::string b3 = "", std::string b4="");
  ///
  int start();
  ///
  void draw();
  ///
  void wait_for_mouse();
  ///
  int  return_value(int i);
}///
;

#endif

/* EOF */
