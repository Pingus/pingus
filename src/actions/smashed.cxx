//  $Id: smashed.cxx,v 1.4 2002/09/04 14:55:12 torangan Exp $
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

#include "../pingu.hxx"
#include "smashed.hxx"

namespace Actions {

  void
  Smashed::init (void)
  {
    sprite = Sprite ("Pingus/bomber0", "pingus");
    sound_played = false;  
  }

  void 
  Smashed::draw_offset (int x, int y, float /*s*/)
  {
    sprite.put_screen (x, y);
  }

  void
  Smashed::update(float /*delta*/)
  {
    //  pingu->particle->add_pingu_explo(pingu->x_pos, pingu->y_pos - 16);
    pingu->set_status(PS_DEAD);
  }

}
/* EOF */
