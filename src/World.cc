//  $Id: World.cc,v 1.61 2001/11/18 00:03:29 grumbel Exp $
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

#include "ActionHolder.hh"
#include "Exit.hh"
#include "FVec.hh"
#include "Hotspot.hh"
#include "Liquid.hh"
#include "PLF.hh"
#include "PinguHolder.hh"
#include "PingusError.hh"
#include "PingusSound.hh"
#include "PingusSpotMap.hh"
#include "Result.hh"
#include "Timer.hh"
#include "View.hh"
#include "World.hh"
#include "WorldObjData.hh"
#include "algo.hh"
#include "globals.hh"
#include "GameTime.hh"
#include "particles/ParticleHolder.hh"
#include "particles/WeatherGenerator.hh"

using namespace std;
using boost::shared_ptr;

// Workaround for a VC bug
#ifdef WIN32
#  define for if(0);else for
#endif /* WIN32 */

// Structure for the sorting algorithm (stable_sort)
struct WorldObj_less
{
  bool operator() (shared_ptr<WorldObj>& a, shared_ptr<WorldObj>& b) const 
  {
    return a->get_z_pos () < b->get_z_pos ();
  }
};

World::World()
  : view (0)
{
  released_pingus = 0;
  exit_world = false;
  WorldObj::set_world(this);
}

World::World(boost::shared_ptr<PLF> plf)
{ 
  WorldObj::set_world(this);
  init (plf);
}

World::~World()
{
}

// Merge the different layers on the screen together
void 
World::draw(int x1, int y1, int w, int h,
	    int x_of, int y_of, float s)
{
  x_of += x1;
  y_of += y1;
  
  for(WorldObjIter obj = world_obj.begin(); obj != world_obj.end(); ++obj)
    (*obj)->draw_offset(x_of, y_of, s);
  
  //gfx_map->draw(x1, y1, w, h, x_of, y_of, s);
  
  particle_holder->draw_offset(x_of, y_of, s);
}

void 
World::update(float delta)
{
  //std::cout << "World::update (" << delta << ")" << std::endl;

  if (!exit_world && (allowed_pingus == released_pingus || do_armageddon)
      && pingus->size() == 0) 
    {
      if (verbose) cout << "World: world finished, going down in the next seconds..." << endl;
      exit_world = true;
      shutdown_time = GameTime::get_time() + 75;
    }

  if (do_armageddon && armageddon_count != pingus->end())
    {
      (*armageddon_count)->set_action("bomber");
      armageddon_count++;
    }
  
  // Create new pingus, if enough time is passed
  /*
    if (!do_armageddon)
    {
    for(vector<shared_ptr<Entrance> >::iterator i = entrance.begin(); i != entrance.end(); i++) 
    {
    if ((*i)->pingu_ready() && (unsigned int)pingus->total_size() < allowed_pingus)
    {
    pingus->push_back((*i)->get_pingu());
    ++released_pingus;
    }
    }
    }*/
  
  // Let all pingus move and
  // Let the pingus catch each other and
  // Let the traps catch the pingus and
  // Let the exit catch the pingus
  for(WorldObjIter obj = world_obj.begin(); obj != world_obj.end(); ++obj)
    {
      (*obj)->update(delta);
    }
  
  for(PinguIter pingu = pingus->begin(); pingu != pingus->end(); ++pingu)
    {
      (*pingu)->update(delta);
      
      if ((*pingu)->need_catch()) {
	for(PinguIter i = pingus->begin(); i != pingus->end(); i++) {
	  (*pingu)->catch_pingu(i->get());
	}
      }
      
      //for(vector<shared_ptr<Trap> >::iterator obj = traps.begin(); obj != traps.end(); obj++)
      //(*obj)->catch_pingu(*pingu);
      
      //for(vector<shared_ptr<Exit> >::iterator obj = exits.begin(); obj != exits.end(); obj++) 
      //(*obj)->catch_pingu(*pingu);
    }

  /*  for(vector<TrapData>::size_type i=0; i < traps.size(); ++i)
      traps[i]->update(delta);

      for(vector<EntranceData>::size_type i2=0; i2 < entrance.size(); ++i2) 
      entrance[i2]->update(delta);
  */    
  particle_holder->update(delta);

  // Clear the explosion force list
  ForcesHolder::clear_explo_list();
}

void 
World::init(boost::shared_ptr<PLF> plf_data)
{
  plf = plf_data;
  do_armageddon = false;
  allowed_pingus = plf->get_pingus();
  number_to_save = plf->get_number_to_save();
  released_pingus = 0;

  exit_time = plf->get_time();
  if (exit_time != -1 && !(exit_time > 100))
    std::cout << "World: Time is not in the tolerated range: " << exit_time << std::endl;
  shutdown_time = -1;

  init_map();

  particle_holder = shared_ptr<ParticleHolder>(new ParticleHolder());
  pingus = shared_ptr<PinguHolder>(new PinguHolder());

  //Timer timer;
  //timer.start();
  //cout << "World: Inititializing world objects... " << flush;
  init_worldobjs();
  //cout << "done " << timer.stop() << endl;
}

void
World::init_map()
{
  // load the foreground map
  /*switch (plf->map_type())
    {
    case SPOT:*/
  gfx_map = shared_ptr<PinguMap>(new PingusSpotMap(plf));

  world_obj.push_back (gfx_map);

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
World::init_worldobjs()
{
  vector<WeatherData>  weather_d  = plf->get_weather();
  vector<shared_ptr<WorldObjData> > worldobj_d = plf->get_worldobjs_data ();

  for(vector<WeatherData>::iterator i = weather_d.begin();
      i != weather_d.end();
      i++)
    {
      shared_ptr<WeatherGenerator> weather_gen = WeatherGenerator::create(*i);
      world_obj.push_back(weather_gen);
    }

  for (vector<shared_ptr<WorldObjData> >::iterator i = worldobj_d.begin ();
       i != worldobj_d.end ();
       i++)
    {
      shared_ptr<WorldObj> obj = (*i)->create_WorldObj ();
      if (obj.get ())
	world_obj.push_back(obj);
    }

  world_obj.push_back(pingus);

  // After all objects are in world_obj, sort them to there z_pos
  world_obj.sort(WorldObj_less());
  // FIXME: If the above thing causes throuble under windows we could
  // use a vector instead of a list and use stable_sort() instead.

  // Drawing all world objs to the colmap
  for(WorldObjIter obj = world_obj.begin(); obj != world_obj.end(); ++obj)
    (*obj)->draw_colmap();

  // Setup the gravity force
  // Clear all old forces
  ForcesHolder::clear_all_forces();
  ForcesHolder::add_force(GravityForce(grav));
}

PinguHolder*
World::get_pingu_p(void)
{
  return pingus.get();
}

int
World::get_width(void)
{
  assert(gfx_map.get());
  return gfx_map->get_width();  
}

int
World::get_height(void)
{
  assert(gfx_map.get());
  return gfx_map->get_height();
}

int
World::get_time_left()
{
  if (exit_time != -1) // There is a time limit
    {
      return exit_time - GameTime::get_time();
    }
  else // No timelimit given
    {
      return -1;
    }  
}

int
World::get_time_passed()
{
  return GameTime::get_time();
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
  if (((exit_time != -1) && (exit_time < (GameTime::get_time())))
      || ((shutdown_time != -1) && shutdown_time < GameTime::get_time()))
    {
      std::cout << "ExitTime: " << exit_time << std::endl
		<< "GameTime: " << GameTime::get_time() << std::endl
		<< "ShutDown: " << shutdown_time << std::endl;
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
World::get_gfx_map ()
{
  return gfx_map.get();
}

ActionHolder*
World::get_action_holder ()
{
  return action_holder;
}

ParticleHolder* 
World::get_particle_holder()
{
  return particle_holder.get();
}

boost::shared_ptr<PLF>
World::get_plf()
{
  return plf;
}

void
World::set_action_holder(ActionHolder* a)
{
  action_holder = a;
}

void 
World::play_wav (std::string name, const CL_Vector& pos, float volume = 0.5f)
{
  if (view.get ())
    {
      CL_Vector center = view->get_center ();
      float panning = pos.x - center.x;
      panning /= view->get_width ()/2;

      if (panning > 1.0f)
	panning = 1.0f;

      if (panning < -1.0f)
	panning = -1.0f;
#warning Ziemlich dummer Code
      PingusSound::play (name + ".wav", volume, panning);
    }
  else // No view available, so no stereo enabled
    {
      PingusSound::play (name + ".wav", volume);
    }
}

void
World::set_view (shared_ptr<View> v)
{
  view = v;
}

boost::shared_ptr<Pingu> 
World::get_pingu (const CL_Vector& pos)
{
  boost::shared_ptr<Pingu> current_pingu;
  double distance = -1.0;

  for (PinguIter i = pingus->begin (); i != pingus->end (); ++i) {
    if ((*i)->is_over (int(pos.x), int(pos.y)))
      {
	if (distance == -1.0f || distance >= (*i)->dist ((int) pos.x, (int)pos.y))
	  {
	    current_pingu = (*i);
	    distance = (*i)->dist ((int)pos.x, (int)pos.y);
	  }
      }
  }
  
  return current_pingu;
}

/* EOF */
