//  $Id: smashed.hxx,v 1.6 2002/08/25 09:08:49 torangan Exp $
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

#ifndef HEADER_PINGUS_ACTIONS_SMASHED_HXX
#define HEADER_PINGUS_ACTIONS_SMASHED_HXX

#include "../sprite.hxx"
#include "../pingu_action.hxx"

namespace Actions {

  class Smashed : public PinguAction
  {
  private:
    bool particle_thrown;
    bool sound_played;
    Sprite sprite;
  
  public:
    Smashed () { }
    void init (void);
    std::string get_name () const { return "Smashed"; }
    ActionName get_type () const { return Actions::Smashed; }
    void update (float delta);
    void draw_offset (int x, int y, float s);
    bool catchable () { return false; }
  
  private:
    Smashed (const Smashed&);
    Smashed operator= (const Smashed&);
  };

}

#endif

/* EOF */
