//  $Id: capture_rectangle.hxx,v 1.9 2003/10/21 11:01:52 grumbel Exp $
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

#ifndef HEADER_PINGUS_CAPTURE_RECTANGLE_HXX
#define HEADER_PINGUS_CAPTURE_RECTANGLE_HXX

class CL_Font;

namespace Pingus {

class Pingu;
class PinguAction;
class Pingu;
class ButtonPanel;

/** The rectangle that is shown when the mouse cursor is above a
    pingu. The rectangle shows the current pingu direction along with
    the current active action.
 */
class CaptureRectangle
{
private:
  Pingu*      pingu;
  std::string action_str;

  /// The id of the owner of this capture rectangle
  int owner_id;

  CL_Sprite good;
  CL_Sprite bad;
  CL_Sprite arrow_left;
  CL_Sprite arrow_right;

  ButtonPanel* button_panel;
  CL_Font font;

public:
  CaptureRectangle(ButtonPanel*);
  ~CaptureRectangle();

  void set_pingu(Pingu* pingu);

  void draw_offset(int x_offset, int y_offset, float s = 1.0);

private:
  CaptureRectangle (const CaptureRectangle&);
  CaptureRectangle& operator= (const CaptureRectangle&);
};

} // namespace Pingus

#endif

/* EOF */
