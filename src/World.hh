//  $Id: World.hh,v 1.18 2000/09/29 16:21:17 grumbel Exp $
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

#ifndef WORLD_HH
#define WORLD_HH

#include <ClanLib/core.h>
#include <vector>
#include <iterator>

#include "WorldObj.hh"
#include "PinguHolder.hh"
#include "PLF.hh"
#include "Pingu.hh"
#include "backgrounds/Background.hh"
#include "ColMap.hh"
#include "Entrance.hh"
#include "Exit.hh"
#include "Hotspot.hh"
#include "Liquid.hh"
#include "Trap.hh"
#include "traps/traps.hh"
#include "particles/ParticleHolder.hh"
#include "PinguMap.hh"

class WorldObj;
class Exit;
class Entrance;
class Liquid;
class Hotspot;
class PinguHolder;
class Trap;
class ParticleHolder;
class ActionHolder;
class Background;

/** The World holds all objects of the pingu enviroment. 
    
    It holds the pingus, traps, exits, entrances, etc.. It keeps
    control that all objects become time to move and can catch each
    other. */
class World
{
private:
  ///
  PinguMap* gfx_map;
  ///
  bool do_armageddon;
  ///
  std::list<Pingu*>::iterator armageddon_count;

  ///
  unsigned int released_pingus;
  ///
  unsigned int allowed_pingus;

  ///
  unsigned int number_to_save;

  ///
  bool exit_world;
  
  /** End the world when the given time is reached, this is set by
      armageddon or similar events.  */
  int  shutdown_time;  

  /// The time you have to finish a level
  int exit_time;

  ///
  std::vector<Background*> backgrounds;
  ///
  std::vector<WorldObj*> world_obj_bg;
  ///
  std::vector<WorldObj*> world_obj_fg;
  ///
  std::vector<Entrance*> entrance;
  ///
  std::vector<Exit*>     exits;
  ///
  std::vector<Hotspot*>  hotspot;
  ///
  std::vector<Liquid*>   liquid;
  ///
  std::vector<Trap*>     traps;

  ///
  ParticleHolder* particle_holder;
  ///
  ActionHolder* action_holder;
  ///
  PinguHolder* pingus;
  ///
  ColMap* colmap;
  ///
  PLF*    plf;

public:
  ///
  World();
  ///
  World(PLF*);
  ///
  virtual ~World();

  ///
  void    init (PLF*);
  ///
  void    init_background (void);
  ///
  void    init_worldobjs (void);
  ///
  void    init_map (void);

  /** Draws the world onto the screen
      @param x1 The left corner of the drawing area.
      @param y1 The upper corner of the drawing area.
      @param w  The width of the drawing area.
      @param h  The height of the drawing area. */
  void    draw(int x1, int y1, int w, int h,
	       int x_of, int y_of, float s);
  ///
  void    let_move ();

  /// Issue an armageddon, all Pingus will explode in some seconds.
  void    armageddon ();

  /** @return The absolute height of the world. */
  int     get_height ();
  /** @return The absolute width of the world */
  int     get_width();
  ///
  bool    is_finished ();

  /** Returns the time passed since the level was started */
  int get_time_passed();

  /** Returns the time left until the time is out or -1 if there is no
      time limit */
  int get_time_left();

  /** True if there is a time limit in this world */
  bool    has_time_limit();

  /** @return A pointer to the collision map used in this world */
  ColMap* get_colmap();

  /** @return A pointer to the gfx map of this world */
  PinguMap* get_gfx_map();

  /** @return A pointer to the worlds particle holder */
  ParticleHolder* get_particle_holder();
  
  /** @return Pointer to the ActionHolder of the world */
  ActionHolder* get_action_holder();

  ///
  PLF*    get_plf();

  ///
  unsigned int get_released_pingus() { return released_pingus; }
  ///
  unsigned int get_allowed_pingus() { return allowed_pingus; }
  ///
  unsigned int get_pingus_out();
  ///
  unsigned int get_saved_pingus();
  ///
  unsigned int get_number_to_save() { return number_to_save; }

  ///
  PinguHolder* get_pingu_p(void);
  ///
  void set_action_holder(ActionHolder*);
};

#endif

/* EOF */
