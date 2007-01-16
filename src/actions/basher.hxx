//  $Id: basher.hxx,v 1.19 2003/12/13 11:21:24 grumbel Exp $
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

#include "../state_sprite.hxx"
#include "../pingu_action.hxx"

namespace Actions {

class Basher : public PinguAction
{
private:
  StateSprite sprite;
  PixelBuffer bash_radius;
  PixelBuffer bash_radius_gfx;
  int basher_c;
  bool first_bash;

  /** Width of the bash_radius surface */
  unsigned int bash_radius_width;

  /** Width of the bash_radius_gfx surface */
  unsigned int bash_radius_gfx_width;

  /** The no. of pixels ahead that a Basher checks for something bashable.
      This is initialised using the size of the bash_radius surface. */
  int bash_reach;

  /** Defines the minimum "wall" height needed for a bash to happen. */
  enum { min_bash_height = 5 };

  /** Defines the maximum height up to which a check is made to see if there
      is anything to bash.  Best to make this at least (min_bash_height +
      pingu_height). */
  enum { max_bash_height = 33 };

  /** Defines the maximum no. of steps down a Basher can go down before it
      stops being a Basher and turns into a Faller. */
  enum { max_steps_down = 3 };

public:
  Basher (Pingu* p);

  ActionName get_type () const { return Actions::Basher; }

  void draw (SceneContext& gc);
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
