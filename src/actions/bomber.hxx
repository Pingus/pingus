//  $Id: bomber.hxx,v 1.18 2003/03/21 22:08:06 grumbel Exp $
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

#ifndef HEADER_PINGUS_ACTIONS_BOMBER_HXX
#define HEADER_PINGUS_ACTIONS_BOMBER_HXX

#include "../sprite.hxx"
#include "../pingu_action.hxx"

namespace Actions {

/** An action with lets the Pingu explode. After the explosion the the
    Pingu leaves a hole inside the ground. */
class Bomber : public PinguAction
{
private:
  bool particle_thrown;
  bool sound_played;
  bool gfx_exploded; 
  bool colmap_exploded; 

  static bool static_surface_loaded;
  static CL_Surface bomber_radius;
  static CL_Surface bomber_radius_gfx;

  Sprite     sprite;
  CL_Surface explo_surf;

public:
  Bomber (Pingu* p);

  std::string get_name () const { return "Bomber"; }
  ActionName get_type() const { return Actions::Bomber; }
  ActionType get_activation_mode() const { return COUNTDOWN_TRIGGERED; }

  void draw (GraphicContext& gc);
  void update();

  int  activation_time() { return 150; }
  void on_successfull_apply ();
  
private:
  Bomber (const Bomber&);
  Bomber& operator= (const Bomber&);
};

} // namespace Actions

#endif

/* EOF */
