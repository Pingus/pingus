//  $Id: pingu_action_factory.cxx,v 1.3 2002/06/18 10:04:11 torangan Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#include <iostream>
#include "pingus_error.hxx"

#include "actions/rocket_launcher.hxx"
#include "actions/boarder.hxx"
#include "actions/superman.hxx"
#include "actions/angel.hxx"
#include "actions/basher.hxx"
#include "actions/blocker.hxx"
#include "actions/bomber.hxx"
#include "actions/bridger.hxx"
#include "actions/climber.hxx"
#include "actions/digger.hxx"
#include "actions/floater.hxx"
#include "actions/miner.hxx"
#include "actions/jumper.hxx"
#include "actions/slider.hxx"
#include "actions/teleported.hxx"
#include "actions/exiter.hxx"
#include "actions/smashed.hxx"
#include "actions/laser_kill.hxx"
#include "actions/splashed.hxx"
#include "actions/waiter.hxx"
#include "actions/drown.hxx"
#include "actions/faller.hxx"
#include "actions/walker.hxx"

#include "pingu_action_factory.hxx"

PinguActionFactory::PinguActionFactory ()
{
}

PinguActionFactory* PinguActionFactory::instance_ = 0;

PinguActionFactory* 
PinguActionFactory::instance ()
{
  if ( ! instance_)
    {
      instance_ = new PinguActionFactory ();
      instance_->register_core_actions ();
    }
  return instance_;
}

void 
PinguActionFactory::register_core_actions ()
{
  new PinguActionFactoryImpl<RocketLauncher> ("rocketlauncher");
  new PinguActionFactoryImpl<Boarder> ("boarder");
  new PinguActionFactoryImpl<Superman> ("superman");
  new PinguActionFactoryImpl<Angel> ("angel");
  new PinguActionFactoryImpl<Basher> ("basher");
  new PinguActionFactoryImpl<Blocker> ("blocker");
  new PinguActionFactoryImpl<Bomber> ("bomber");
  new PinguActionFactoryImpl<Bridger> ("bridger");
  new PinguActionFactoryImpl<Climber> ("climber");
  new PinguActionFactoryImpl<Digger> ("digger");
  new PinguActionFactoryImpl<Exiter> ("exiter");
  new PinguActionFactoryImpl<Floater> ("floater");
  new PinguActionFactoryImpl<Miner> ("miner");
  new PinguActionFactoryImpl<Jumper> ("jumper");
  new PinguActionFactoryImpl<Slider> ("slider");
  new PinguActionFactoryImpl<Teleported> ("teleporter");
  new PinguActionFactoryImpl<Exiter> ("exiter");
  new PinguActionFactoryImpl<Smashed> ("smashed");
  new PinguActionFactoryImpl<LaserKill> ("laserkill");
  new PinguActionFactoryImpl<Splashed> ("splashed");
  new PinguActionFactoryImpl<Waiter> ("waiter");
  new PinguActionFactoryImpl<Drown> ("drown");
  new PinguActionFactoryImpl<Faller> ("faller");
  new PinguActionFactoryImpl<Walker> ("walker");
}

PinguAction* 
PinguActionFactory::create (const std::string& id)
{
  std::map<std::string, PinguActionAbstractFactory*>::iterator it = factories.find(id);
  
  if (it == factories.end())
    throw PingusError("PinguActionFactory: Invalid id: " + id);
  else 
    {
      PinguAction* action = it->second->create ();
      all_actions.push_back (action);
      return action;
    }
}

void
PinguActionFactory::delete_actions ()
{
  std::cout << "PinguActionFactory::delete_actions (): Deleting all Actions" << std::endl;
  for (std::vector<PinguAction*>::iterator i = all_actions.begin ();
       i != all_actions.end (); ++i)
    {
      delete *i;
    }
  all_actions.clear ();
  std::cout << "PinguActionFactory::delete_actions (): Deleting all Actions: done" << std::endl;
}

void 
PinguActionFactory::register_factory (const std::string& id, PinguActionAbstractFactory* factory)
{
  // FIXME: Could need some more error checking
  factories[id] = factory;
}

/* EOF */
