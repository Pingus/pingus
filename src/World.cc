//  $Id: World.cc,v 1.31 2000/08/05 18:52:22 grumbel Exp $
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

//#include "PinguBmpMap.hh"
#include "PingusSpotMap.hh"
//#include "PinguRandomMap.hh"

#include "PingusError.hh"
#include "algo.hh"
#include "globals.hh"
#include "Background.hh"
#include "World.hh"
#include "traps/traps.hh"
#include "Result.hh"
#include "Liquid.hh"
#include "ActionHolder.hh"
#include "entrances/entrances.hh"
#include "FVec.hh"
#include "Timer.hh"
#include "particles/SnowGenerator.hh"

using namespace std;

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
  particle_holder = 0;
  released_pingus = 0;
  gfx_map = 0;
  background = 0;
  exit_world = false;
  WorldObj::set_world(this);
}

World::World(PLF* plf)
{ 
  particle_holder = 0;
  WorldObj::set_world(this);
  init(plf);
}

World::~World()
{
  std::cout << "World:~World" << std::endl;

  delete pingus;
  delete particle_holder;
  delete gfx_map;
  delete background;

  for(vector<WorldObj*>::iterator obj = world_obj_bg.begin();
      obj != world_obj_bg.end();
      obj++)
    {
      delete *obj;
    }
}

// Merge the different layers on the screen together
void 
World::draw(int x1, int y1, int w, int h,
	    int x_of, int y_of, float s)
{
  x_of += x1;
  y_of += y1;

  background->draw_offset(x_of, y_of, s);

  for(vector<WorldObj*>::iterator obj = world_obj_bg.begin(); obj != world_obj_bg.end(); obj++)
    {
      (*obj)->draw_offset(x_of, y_of, s);
    }

  gfx_map->draw(x1, y1, w, h, x_of, y_of, s);

  for(vector<WorldObj*>::iterator obj = world_obj_fg.begin(); obj != world_obj_fg.end(); obj++)
    {
      (*obj)->draw_offset(x_of, y_of, s);
    }
  
  particle_holder->draw_offset(x_of, y_of, s);
}

void 
World::let_move()
{
  if (!exit_world && (allowed_pingus == released_pingus || do_armageddon)
      && pingus->size() == 0) 
    {
      if (verbose) cout << "World: world finished, going down in the next seconds..." << endl;
      exit_world = true;
      shutdown_time = GameTime::get_time() + 75;
    }

  if (do_armageddon && armageddon_count != pingus->end())
    {
      (*armageddon_count)->set_action(action_holder->get_uaction("bomber"));
      armageddon_count++;
    }
  
  // Create new pingus, if enough time is passed
  if (!do_armageddon && (unsigned int)pingus->total_size() < allowed_pingus)
    {
      for(vector<Entrance*>::iterator i = entrance.begin(); i != entrance.end(); i++) 
	{
	  if ((*i)->pingu_ready())
	    {
	      pingus->push_back((*i)->get_pingu());
	      ++released_pingus;
	    }
	}
    }
  
  // Let all pingus move and
  // Let the pingus catch each other and
  // Let the traps catch the pingus and
  // Let the exit catch the pingus
  for(vector<WorldObj*>::iterator obj = world_obj_bg.begin(); 
      obj != world_obj_bg.end(); 
      obj++)
    {
      (*obj)->let_move();
    }

  for(vector<WorldObj*>::iterator obj = world_obj_fg.begin(); 
      obj != world_obj_fg.end(); 
      obj++)
    {
      (*obj)->let_move();
    }

  for(vector<WorldObj*>::iterator obj = world_obj_fg.begin(); 
      obj != world_obj_fg.end(); 
      obj++)
    {
      (*obj)->let_move();
    }

  for(PinguIter pingu = pingus->begin(); pingu != pingus->end(); ++pingu)
    {
      (*pingu)->let_move();

      if ((*pingu)->need_catch()) {
	for(PinguIter i = pingus->begin(); i != pingus->end(); i++) {
	  (*pingu)->catch_pingu(*i);
	}
      }
    
      for(vector<Trap*>::iterator obj = traps.begin(); obj != traps.end(); obj++)
	(*obj)->catch_pingu(*pingu);
      
      for(vector<Exit*>::iterator obj = exits.begin(); obj != exits.end(); obj++) 
	(*obj)->catch_pingu(*pingu);
    }

  /*  for(vector<TrapData>::size_type i=0; i < traps.size(); ++i)
    traps[i]->let_move();

  for(vector<EntranceData>::size_type i2=0; i2 < entrance.size(); ++i2) 
    entrance[i2]->let_move();
  */    
  particle_holder->let_move();
  background->let_move();

  // Clear the explosion force list
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
  shutdown_time = exit_time;

  init_map();
  init_background();

  particle_holder = new ParticleHolder();
  pingus = new PinguHolder();

  Timer timer;

  timer.start();
  cout << "World: Initing world objects... " << flush;
  init_worldobjs();
  cout << "done " << timer.stop() << endl;
}

void
World::init_map()
{
  // load the foreground map
  /*switch (plf->map_type())
    {
    case SPOT:*/
      gfx_map = new PingusSpotMap(plf);
      /*      break;
    case BMP:
    case RANDOM:
    default:
      throw PingusError("World: Error in PLF file: Undef or unsupported Maptype");
      break;
    } */
  colmap = gfx_map->get_colmap();
}

void 
World::init_background()
{
  // load the background map
  background = new Background(plf->get_background());
}

void
World::init_worldobjs()
{
  vector<ExitData>     exit_d     = plf->get_exit();
  vector<EntranceData> entrance_d = plf->get_entrance();
  vector<TrapData>     trap_d     = plf->get_traps();
  vector<HotspotData>  hspot_d    = plf->get_hotspot();
  vector<LiquidData>   liquid_d   = plf->get_liquids();
  vector<WeatherData>  weather_d  = plf->get_weather();
  
  // Creating Exit and Entrance
  for(vector<ExitData>::iterator i = exit_d.begin(); i != exit_d.end(); i++) 
    exits.push_back(new Exit(*i));
  
  for(vector<EntranceData>::size_type i = 0; i < entrance_d.size(); ++i) 
    entrance.push_back(get_entrance(entrance_d[i]));

  for(vector<TrapData>::size_type i=0; i < trap_d.size(); ++i)
    traps.push_back(get_trap(trap_d[i]));

  // Creating the foreground and background hotspots
  for(vector<HotspotData>::size_type i = 0; i < hspot_d.size(); ++i)
    hotspot.push_back(new Hotspot(hspot_d[i]));
  
  for(vector<LiquidData>::size_type i=0; i < liquid_d.size(); i++) 
    liquid.push_back(new Liquid(liquid_d[i]));
  
  for(vector<WeatherData>::iterator i = weather_d.begin();
      i != weather_d.end();
      i++)
    {
      WeatherGenerator* weather_gen = WeatherGenerator::create(*i);

      if (weather_gen)
	{
	  if (weather_gen->get_z_pos() > 0)
	    world_obj_fg.push_back(weather_gen);
	  else
	    world_obj_bg.push_back(weather_gen);
	}
    }

  // Push all objects to world_obj vector
  for(vector<HotspotData>::size_type i = 0; i < hotspot.size(); i++)
    {
      if (hotspot[i]->pos.z_pos <= 0)
	world_obj_bg.push_back(hotspot[i]);
      else 
	world_obj_fg.push_back(hotspot[i]);
    }

  for(vector<ExitData>::size_type i=0; i < exits.size(); i++)
    { 
      if (exits[i]->pos.z_pos <= 0)
	world_obj_bg.push_back(exits[i]);
      else 
	world_obj_fg.push_back(exits[i]);
    }  
  
  for(vector<EntranceData>::size_type i=0; i < entrance.size(); ++i)
    {
      if (entrance[i]->pos.z_pos <= 0)
	world_obj_bg.push_back(entrance[i]);
      else 
	world_obj_fg.push_back(entrance[i]);
    }

  for(vector<LiquidData>::size_type i=0; i < liquid_d.size(); ++i)
    {
      if (liquid[i]->pos.z_pos <= 0)
	world_obj_bg.push_back(liquid[i]);
      else 
	world_obj_fg.push_back(liquid[i]);
    }
  
  for(vector<TrapData>::size_type i=0; i < traps.size(); ++i) 
    {
      if (traps[i]->pos.z_pos <= 0)
	world_obj_bg.push_back(traps[i]);
      else 
	world_obj_fg.push_back(traps[i]);
    }

  world_obj_fg.push_back(pingus);

  // After all objects are in world_obj, sort them to there z_pos
  stable_sort(world_obj_bg.begin(), world_obj_bg.end(), WorldObj_less());
  stable_sort(world_obj_fg.begin(), world_obj_fg.end(), WorldObj_less());

  // Drawing all world objs to the colmap
  for(vector<WorldObj*>::iterator obj = world_obj_bg.begin(); obj != world_obj_bg.end(); obj++)
    (*obj)->draw_colmap();

  for(vector<WorldObj*>::iterator obj = world_obj_fg.begin(); obj != world_obj_fg.end(); obj++)
    (*obj)->draw_colmap();
  
  // Setup the gravity force
  // Clear all old forces
  ForcesHolder::clear_all_forces();
  ForcesHolder::add_force(GravityForce(grav));
}

PinguHolder*
World::get_pingu_p(void)
{
  return pingus;
}

int
World::get_width(void)
{
  assert(gfx_map);
  return gfx_map->get_width();  
}

int
World::get_height(void)
{
  assert(gfx_map);
  return gfx_map->get_height();
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

unsigned int
World::get_pingus_out()
{
  return pingus->size(); 
}

unsigned int 
World::get_saved_pingus() 
{
  return pingus->get_saved(); 
}

void 
World::armageddon(void)
{
  do_armageddon = true;
  armageddon_count = pingus->begin();
}

bool
World::is_finished(void)
{
  // Return true if the world is finished and some time has passed
  if (exit_time < GameTime::get_time()
      || shutdown_time < GameTime::get_time())
    {
      return true;
    } 
  else 
    {
      return false;
    }
}

ColMap*
World::get_colmap()
{
  return colmap;
}

PinguMap* 
World::get_gfx_map()
{
  return gfx_map;
}

ActionHolder*
World::get_action_holder()
{
  return action_holder;
}

ParticleHolder* 
World::get_particle_holder()
{
  return particle_holder;
}

PLF*    
World::get_plf()
{
  return plf;
}

void
World::set_action_holder(ActionHolder* a)
{
  action_holder = a;
}

/* EOF */
