//  $Id: bomber.hxx,v 1.13 2002/10/01 19:53:45 grumbel Exp $
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
  Sprite sprite;
  static CL_Surface bomber_radius;
  static CL_Surface bomber_radius_gfx;

  CL_Surface explo_surf;

public:
  Bomber ();
  
  void   init (void);
  std::string get_name () const { return "Bomber"; }
  ActionName get_type() const { return Actions::Bomber; }
  ActionType get_activation_mode() const { return COUNTDOWN_TRIGGERED; }

  void draw (GraphicContext& gc);
  void update();

  void update_position();
  int  activation_time() { return 50; }
  void on_successfull_apply (Pingu*);
  
private:
  Bomber (const Bomber&);
  Bomber& operator= (const Bomber&);
};

} // namespace Actions

#endif

/* EOF */
