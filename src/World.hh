//  $Id: World.hh,v 1.40 2002/06/12 11:04:03 torangan Exp $
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

#include <list>
#include <string>
#include "WorldObj.hh"

// Forward declarations
class CL_Vector;
class ActionHolder;
class Entrance;
class Exit;
class Hotspot;
class Liquid;
class PLF;
class ParticleHolder;
class PinguHolder;
class PinguMap;
class Pingu;
class Trap;
class View;
class ColMap;
class WorldImpl;

/** The World holds all objects of the pingu enviroment. 
    
    It holds the pingus, traps, exits, entrances, etc.. It keeps
    control that all objects become time to move and can catch each
    other. */
class World
{
private:
  WorldImpl* impl;

  PinguMap* gfx_map;
  bool do_armageddon;
  std::list<Pingu*>::iterator armageddon_count;

  unsigned int released_pingus;
  unsigned int allowed_pingus;
  unsigned int number_to_save;
  bool exit_world;
  
  /** End the world when the given time is reached, this is set by
      armageddon or similar events.  */
  int  shutdown_time;  

  /// The time you have to finish a level
  int exit_time;

  std::list<WorldObj*> * world_obj;

  // These pointers hold objects and must be deleted
  ParticleHolder* particle_holder;
  PinguHolder*    pingus;

  // Pointers which are references to objects from other classes
  ActionHolder*   action_holder;
  ColMap*         colmap;
  PLF*            plf;
  View*           view;

  void    init_worldobjs (void);
  void    init_map (void);

public:
  World(PLF*);
  virtual ~World();

  /** Draws the world onto the screen
      @param x1 The left corner of the drawing area.
      @param y1 The upper corner of the drawing area.
      @param w  The width of the drawing area.
      @param h  The height of the drawing area. */
  void    draw(int x1, int y1, int w, int h,
	       int x_of, int y_of, float s);
  ///
  void    update (float delta);

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

  PLF* get_plf();

  unsigned int get_released_pingus() { return released_pingus; }
  void         inc_released_pingus() { ++released_pingus; }
  unsigned int check_armageddon() { return do_armageddon; }
  unsigned int get_allowed_pingus() { return allowed_pingus; }
  unsigned int get_pingus_out();
  unsigned int get_saved_pingus();
  unsigned int get_number_to_save() { return number_to_save; }

  /** Play a sound as if it would have been generated at the given
      position, adjust panning and volume by the position relative to
      the center of the screen 
      @param name Filename of the wav file to play
      @param pos Position from which the sound seems to come (z-pos is going to be ignored) void play_wav (std::string name, const
      @param volume The volume of the sound
  */
  void play_wav (std::string name, const CL_Vector& pos, float volume = 0.5f);

  /** Sets the main view, it is needed to play stereo wave and for
      other screen orientated effects */
  void set_view (View* v);

  PinguHolder* get_pingu_p(void);
  void set_action_holder(ActionHolder*);

  /** @return the pingu at the given word coordinates, an empty
      shared_ptr is returned if none is there */
  Pingu* get_pingu (const CL_Vector& pos);
};

#endif

/* EOF */
