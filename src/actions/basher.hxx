//  $Id: basher.hxx,v 1.14 2002/10/13 16:40:00 grumbel Exp $
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

  /** Defines the maximum no. of steps down a Basher can go down before it
      stops being a Basher and turns into a Faller. */
  enum { max_steps_down = 3 };
    
public:
  Basher (Pingu* p);
    
  std::string get_name () const { return "Basher"; }
  ActionName get_type () const { return Actions::Basher; }
    
  void draw (GraphicContext& gc);
  void update ();
    
  bool have_something_to_dig ();
  void walk_forward ();
  void bash ();
  
private:
  Basher (const Basher&);
  Basher& operator= (const Basher&);
};

} // namespace Actions

#endif

/* EOF */
