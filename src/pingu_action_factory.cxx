//  $Id: pingu_action_factory.cxx,v 1.15 2003/04/19 10:23:17 torangan Exp $
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

using namespace Actions;

PinguActionFactory* PinguActionFactory::instance_ = 0;

PinguActionFactory::PinguActionFactory ()
{
}

PinguActionFactory::~PinguActionFactory ()
{
  delete_actions ();

  // Delete the action factories
  for (std::map<Actions::ActionName, PinguActionAbstractFactory*>::iterator i = factories.begin();
       i != factories.end();
       ++i)
    {
      delete i->second;
    }
  factories.clear();
}

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
PinguActionFactory::init()
{
  instance_ = 0;
}

void
PinguActionFactory::deinit()
{
  delete instance_;
}

void
PinguActionFactory::register_core_actions ()
{
  new PinguActionFactoryImpl<class Angel>          (Angel);
  new PinguActionFactoryImpl<class Basher>         (Basher);
  new PinguActionFactoryImpl<class Blocker>        (Blocker);
  new PinguActionFactoryImpl<class Boarder>        (Boarder);
  new PinguActionFactoryImpl<class Bomber>         (Bomber);
  new PinguActionFactoryImpl<class Bridger>        (Bridger);
  new PinguActionFactoryImpl<class Climber>        (Climber);
  new PinguActionFactoryImpl<class Digger>         (Digger);
  new PinguActionFactoryImpl<class Drown>          (Drown);
  new PinguActionFactoryImpl<class Exiter>         (Exiter);
  new PinguActionFactoryImpl<class Faller>         (Faller);
  new PinguActionFactoryImpl<class Floater>        (Floater);
  new PinguActionFactoryImpl<class Jumper>         (Jumper);
  new PinguActionFactoryImpl<class LaserKill>      (Laserkill);
  new PinguActionFactoryImpl<class Miner>          (Miner);
  new PinguActionFactoryImpl<class RocketLauncher> (Rocketlauncher);
  new PinguActionFactoryImpl<class Slider>         (Slider);
  new PinguActionFactoryImpl<class Smashed>        (Smashed);
  new PinguActionFactoryImpl<class Splashed>       (Splashed);
  new PinguActionFactoryImpl<class Superman>       (Superman);
  new PinguActionFactoryImpl<class Teleported>     (Teleported);
  new PinguActionFactoryImpl<class Waiter>         (Waiter);
  new PinguActionFactoryImpl<class Walker>         (Walker);
}

PinguAction*
PinguActionFactory::create (Pingu* pingu, ActionName id)
{
  //std::cout << "PinguActionFactory: number of actions: " << all_actions.size() << std::endl;
  std::map<ActionName, PinguActionAbstractFactory*>::iterator it = factories.find(id);

  if (it == factories.end())
    PingusError::raise("PinguActionFactory: Invalid id: " + id);
  else
    {
      PinguAction* action = it->second->create (pingu);
      all_actions.push_back (action);
      return action;
    }

  return 0; // never reached
}

void
PinguActionFactory::delete_actions ()
{
  //std::cout << "PinguActionFactory::delete_actions (): Deleting all Actions: "
  //<< all_actions.size() << std::endl;
  for (std::vector<PinguAction*>::iterator i = all_actions.begin ();
       i != all_actions.end (); ++i)
    {
      delete *i;
    }
  all_actions.clear ();
  //std::cout << "PinguActionFactory::delete_actions (): Deleting all Actions: done" << std::endl;
}

void
PinguActionFactory::register_factory (ActionName id, PinguActionAbstractFactory* factory)
{
  // FIXME: Could need some more error checking
  factories[id] = factory;
}

/* EOF */
