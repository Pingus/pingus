//  $Id: pingu_action_factory.cxx,v 1.5 2002/08/16 15:13:59 torangan Exp $
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
  new PinguActionFactoryImpl<class Angel>          (Pingus::Actions::Angel);
  new PinguActionFactoryImpl<class Basher>         (Pingus::Actions::Basher);
  new PinguActionFactoryImpl<class Blocker>        (Pingus::Actions::Blocker);
  new PinguActionFactoryImpl<class Boarder>        (Pingus::Actions::Boarder);
  new PinguActionFactoryImpl<class Bomber>         (Pingus::Actions::Bomber);
  new PinguActionFactoryImpl<class Bridger>        (Pingus::Actions::Bridger);
  new PinguActionFactoryImpl<class Climber>        (Pingus::Actions::Climber);
  new PinguActionFactoryImpl<class Digger>         (Pingus::Actions::Digger);
  new PinguActionFactoryImpl<class Drown>          (Pingus::Actions::Drown);
  new PinguActionFactoryImpl<class Exiter>         (Pingus::Actions::Exiter);
  new PinguActionFactoryImpl<class Faller>         (Pingus::Actions::Faller);
  new PinguActionFactoryImpl<class Floater>        (Pingus::Actions::Floater);
  new PinguActionFactoryImpl<class Jumper>         (Pingus::Actions::Jumper);
  new PinguActionFactoryImpl<class LaserKill>      (Pingus::Actions::Laserkill);
  new PinguActionFactoryImpl<class Miner>          (Pingus::Actions::Miner);
  new PinguActionFactoryImpl<class RocketLauncher> (Pingus::Actions::Rocketlauncher);
  new PinguActionFactoryImpl<class Slider>         (Pingus::Actions::Slider);
  new PinguActionFactoryImpl<class Smashed>        (Pingus::Actions::Smashed);
  new PinguActionFactoryImpl<class Splashed>       (Pingus::Actions::Splashed);
  new PinguActionFactoryImpl<class Superman>       (Pingus::Actions::Superman);
  new PinguActionFactoryImpl<class Teleported>     (Pingus::Actions::Teleported);
  new PinguActionFactoryImpl<class Waiter>         (Pingus::Actions::Waiter);
  new PinguActionFactoryImpl<class Walker>         (Pingus::Actions::Walker);
}

PinguAction* 
PinguActionFactory::create (ActionName id)
{
  std::map<ActionName, PinguActionAbstractFactory*>::iterator it = factories.find(id);
  
  if (it == factories.end())
    PingusError::raise("PinguActionFactory: Invalid id: " + id);
  else 
    {
      PinguAction* action = it->second->create ();
      all_actions.push_back (action);
      return action;
    }
    
  return 0; // never reached
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
PinguActionFactory::register_factory (ActionName id, PinguActionAbstractFactory* factory)
{
  // FIXME: Could need some more error checking
  factories[id] = factory;
}

/* EOF */
