//  $Id: World.cc,v 1.5 2000/02/16 03:06:28 grumbel Exp $
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

#include <vector>
#include <cstdio>
#include <ctime>
#include <algorithm>
#include <functional>

#include "PinguBmpMap.hh"
#include "PingusSpotMap.hh"
#include "PinguRandomMap.hh"

#include "PingusError.hh"
#include "algo.hh"
#include "PingusSound.hh"
#include "globals.hh"
#include "Background.hh"
#include "World.hh"
#include "traps/traps.hh"
#include "Result.hh"
#include "Liquid.hh"
#include "ActionHolder.hh"
#include "entrances/entrances.hh"
#include "FVec.hh"

// Workaround for a VC bug
#ifdef WIN32
#  define for if(0);else for
#endif /* WIN32 */

// Structure for the sorting algorithm (stable_sort)
struct WorldObj_less : public binary_function<WorldObj*, WorldObj*, bool>
{
  bool operator() (WorldObj* a, WorldObj* b) const 
    {
      return (*a) < (*b);
    }
};

World::World()
{
  released_pingus = 0;

  exit_world = false;
}

World::World(PLF* plf)
{ 
  init(plf);
}

World::~World()
{
  // Bug: This crashes, if pingus is exited
  delete map;
  delete background;

  for(std::vector<Exit*>::size_type i = 0; i < exits.size(); ++i) 
    delete exits[i];
  
  for(std::vector<Entrance*>::size_type i = 0; i < entrance.size(); ++i) 
    delete entrance[i];
 
  for(std::vector<Trap*>::size_type i = 0; i < traps.size(); ++i)
    delete traps[i];

  for(std::vector<Hotspot*>::size_type i = 0; i < hotspot.size(); ++i)
    delete hotspot[i];
  
  for(std::vector<Liquid*>::size_type i = 0; i < liquid.size(); ++i) {
    delete liquid[i];
  }
}

// Merge the different layers on the screen together
void 
World::draw_offset(int x_of, int y_of, float s)
{
  background->draw_offset(x_of, y_of, s);

  for(std::vector<WorldObj*>::iterator obj = world_obj.begin(); obj != world_obj.end(); obj++)
    {
      (*obj)->draw_offset(x_of, y_of, s);
    }
  
  particle.draw_offset(x_of, y_of, s);
}

void 
World::let_move()
{
  if (!exit_world && (allowed_pingus == released_pingus || do_armageddon)
      && pingus.size() == 0) 
    {
      if (verbose) std::cout << "World: world finished, going down in the next seconds..." << std::endl;
      exit_world = true;
      exit_time = GameTime::get_time() + 75;
    }

  if (do_armageddon && armageddon_count != pingus.end())
    {
      (*armageddon_count)->set_action(ActionHolder::get_uaction("bomber"));
      armageddon_count++;
    }
  
  // Create new pingus, if enough time is passed
  if (!do_armageddon && (unsigned int)pingus.total_size() < allowed_pingus)
    {
      for(std::vector<Entrance*>::iterator i = entrance.begin(); i != entrance.end(); i++) 
	{
	  if ((*i)->pingu_ready())
	    {
	      pingus.push_back((*i)->get_pingu());
	      ++released_pingus;
	    }
	}
    }
  
  // Let all pingus move and
  // Let the pingus catch each other and
  // Let the traps catch the pingus and
  // Let the exit catch the pingus

  for(PinguIter pingu = pingus.begin(); pingu != pingus.end(); ++pingu)
    {
      (*pingu)->let_move();

      if ((*pingu)->need_catch()) {
	for(PinguIter i = pingus.begin(); i != pingus.end(); i++) {
	  (*pingu)->catch_pingu(*i);
	}
      }
    
      for(std::vector<Trap*>::iterator obj = traps.begin(); obj != traps.end(); obj++)
	(*obj)->catch_pingu(*pingu);
      
      for(std::vector<Exit*>::iterator obj = exits.begin(); obj != exits.end(); obj++) 
	(*obj)->catch_pingu(*pingu);
    }

  for(std::vector<trap_data>::size_type i=0; i < traps.size(); ++i)
    traps[i]->let_move();
  
  for(std::vector<entrance_data>::size_type i2=0; i2 < entrance.size(); ++i2) 
    entrance[i2]->let_move();
  
  particle.let_move();
  background->let_move();
  
  // Clear the explosion force list
  if (force_vectors)
    ForcesHolder::clear_explo_list();
}

void 
World::init(PLF* plf_data)
{
  plf = plf_data;
  do_armageddon = false;
  allowed_pingus = plf->get_pingus();
  number_to_save = plf->get_number_to_save();
  released_pingus = 0;

  exit_time = plf->get_time();
  
  std::cout << "World: exit_time = " << exit_time << std::endl;

  if (verbose)
    std::cout << "World init" << std::endl;

  init_map();
  init_background();
  init_worldobjs();
}

void
World::init_map()
{
  // load the foreground map
  switch (plf->map_type()) 
    {
    case SPOT:
      map = new PingusSpotMap(plf);
      if (verbose) std::cout << "World: Loaded pingu map" << std::endl;
      break;
    case BMP:
    case RANDOM:
    default:
      throw PingusError("World: Error in PLF file: Undef or unsupported Maptype");
      break;
    }
  
  if (verbose) std::cout << "World: Using AutoColMap" << std::endl;
  
  colmap = map->get_colmap();

  if (verbose) std::cout << "World: Loaded Colmap" << std::endl;
}

void 
World::init_background()
{
  // load the background map
  background = new Background(plf->get_bg());

  if (verbose)
    std::cout << "Loaded Background" << std::endl;
}

void
World::init_worldobjs()
{
  vector<exit_data>     exit_d     = plf->get_exit();
  vector<entrance_data> entrance_d = plf->get_entrance();
  vector<trap_data>     trap_d     = plf->get_traps();
  vector<hotspot_data>  hspot_d    = plf->get_hotspot();
  vector<liquid_data>   liquid_d   = plf->get_liquids();

  // Creating Exit and Entrance
  for(std::vector<exit_data>::iterator i = exit_d.begin(); i != exit_d.end(); i++) 
    exits.push_back(new Exit(*i));
  
  for(std::vector<entrance_data>::size_type i = 0; i < entrance_d.size(); ++i) 
    entrance.push_back(get_entrance(entrance_d[i]));

  for(std::vector<trap_data>::size_type i=0; i < trap_d.size(); ++i)
    traps.push_back(get_trap(trap_d[i]));

  // Creating the foreground and background hotspots

  for(std::vector<hotspot_data>::size_type i = 0; i < hspot_d.size(); ++i)
    hotspot.push_back(new Hotspot(hspot_d[i]));
  
  for(std::vector<liquid_data>::size_type i=0; i < liquid_d.size(); i++) 
    liquid.push_back(new Liquid(liquid_d[i]));
  
  Pingu::set_colmap(colmap);
  Pingu::set_map(map);
  Pingu::SetParticleHolder(&particle);
  Trap::SetParticleHolder(&particle);
  Entrance::SetParticleHolder(&particle);

  world_obj.push_back(map);

  // Push all objects to world_obj vector
  for(std::vector<hotspot_data>::size_type i = 0; i < hotspot.size(); i++)
    world_obj.push_back(hotspot[i]);

  for(std::vector<exit_data>::size_type i=0; i < exits.size(); i++)
    world_obj.push_back(exits[i]);
  
  for(std::vector<entrance_data>::size_type i=0; i < entrance.size(); ++i)
    world_obj.push_back(entrance[i]);

  for(std::vector<liquid_data>::size_type i=0; i < liquid_d.size(); ++i)
    world_obj.push_back(liquid[i]);

  for(std::vector<trap_data>::size_type i=0; i < traps.size(); ++i) 
    world_obj.push_back(traps[i]);

  world_obj.push_back(&pingus);

  // After all objects are in world_obj, sort them to there z_pos
  stable_sort(world_obj.begin(), world_obj.end(), WorldObj_less());
  
  if (verbose) 
    std::cout << "Finished Word init" << std::endl;
  
  if (sound_enabled) PingusSound::play(PingusSound::LETSGO);

  // Setup the gravity force
  if (force_vectors)
    {
      // Clear all old forces
      ForcesHolder::clear_all_forces();
      ForcesHolder::add_force(GravityForce(grav));
    }
}

PinguHolder*
World::get_pingu_p(void)
{
  return &pingus;
}

int
World::get_width(void)
{
  assert(map);
  return map->get_width();  
}

int
World::get_height(void)
{
  assert(map);
  return map->get_height();
}

int
World::get_time(void)
{
  if (exit_time) // There is a time limit
    {
      return exit_time - GameTime::get_time();
    }
  else // No timelimit given, lets run forever
    {
      return GameTime::get_time();
    }
}

void 
World::armageddon(void)
{
  std::cout << "Doing armo" << std::endl;
  do_armageddon = true;
  armageddon_count = pingus.begin();
}

bool
World::is_finished(void)
{
  // Return true if the world is finished and some time has passed
  if (exit_time < GameTime::get_time()) 
    {
      return true;
    } 
  else if (exit_world)
    {
      //      std::cout << "The world will exit in: " << exit_time << " : " << GameTime::get_time() << std::endl;
      return true;
    } 
  else 
    {
      return false;
    }
}

/* EOF */
