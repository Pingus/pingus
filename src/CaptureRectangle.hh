//  $Id: CaptureRectangle.hh,v 1.12 2002/06/07 13:25:02 torangan Exp $
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

#ifndef CAPTURERECTANGLE_HH
#define CAPTURERECTANGLE_HH

#include "Sprite.hh"

class PinguAction;
class CL_Font;

namespace boost {

  template <class T> class shared_ptr;
}

/** The rectangle that is shown when the mouse cursor is above a
    pingu. The rectangle shows the current pingu direction along with
    the current active action.
 */
class CaptureRectangle
{
private:
  static boost::shared_ptr<Pingu>       pingu; 
  static boost::shared_ptr<PinguAction> button_action;

  /// The id of the owner of this capture rectangle
  int owner_id;
  
  Sprite good; 
  Sprite bad;
  Sprite arrow_left;
  Sprite arrow_right;

  CL_Font* font;
public:
  ///
  CaptureRectangle();  
  ///
  virtual ~CaptureRectangle(); 
  
  ///
  static void load_data();
  ///
  static void set_pingu(boost::shared_ptr<Pingu> pingu);  
  ///
  static void set_action(boost::shared_ptr<PinguAction>);

  ///
  void draw_offset(int x_offset, int y_offset, float s = 1.0); 
};

#endif

/* EOF */
