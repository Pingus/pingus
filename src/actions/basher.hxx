//  $Id: basher.hxx,v 1.8 2002/09/04 14:55:12 torangan Exp $
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

#ifndef HEADER_PINGUS_ACTIONS_BASHER_HXX
#define HEADER_PINGUS_ACTIONS_BASHER_HXX

#include "../sprite.hxx"
#include "../pingu_action.hxx"

namespace Actions {

  class Basher : public PinguAction
  {
  private:
    Sprite sprite;
    CL_Surface bash_radius;
    CL_Surface bash_radius_gfx;
    int basher_c;
    bool first_bash;
    
    /// Defines "wall" height needed so as to determine whether it should be bashed.
    enum { bash_height = 4 };
    
  public:
    Basher ();
    void   init (void);
    
    std::string get_name () const { return "Basher"; }
    ActionName get_type () const { return Actions::Basher; }
    
    void draw_offset (int x, int y, float s);
    void update (float delta);
    
    bool have_something_to_dig ();
    void walk_forward ();
    void bash ();
  
  private:
    Basher (const Basher&);
    Basher operator= (const Basher&);
  };

}

#endif

/* EOF */
