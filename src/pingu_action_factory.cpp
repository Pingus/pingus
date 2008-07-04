//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include "pingus_error.hpp"

#include "actions/rocket_launcher.hpp"
#include "actions/boarder.hpp"
#include "actions/superman.hpp"
#include "actions/angel.hpp"
#include "actions/basher.hpp"
#include "actions/blocker.hpp"
#include "actions/bomber.hpp"
#include "actions/bridger.hpp"
#include "actions/climber.hpp"
#include "actions/digger.hpp"
#include "actions/floater.hpp"
#include "actions/miner.hpp"
#include "actions/jumper.hpp"
#include "actions/slider.hpp"
#include "actions/teleported.hpp"
#include "actions/exiter.hpp"
#include "actions/smashed.hpp"
#include "actions/laser_kill.hpp"
#include "actions/splashed.hpp"
#include "actions/waiter.hpp"
#include "actions/drown.hpp"
#include "actions/faller.hpp"
#include "actions/walker.hpp"

#include "pingu_action_factory.hpp"


using namespace Actions;

PinguActionFactory* PinguActionFactory::instance_ = 0;

class PinguActionAbstractFactory
{
public:
  PinguActionAbstractFactory (Actions::ActionName id) {
    PinguActionFactory::instance ()-> register_factory (id, this);
  }
  virtual ~PinguActionAbstractFactory() {}

  virtual PinguAction* create (Pingu* p) =0;

private:
  PinguActionAbstractFactory (const PinguActionAbstractFactory&);
  PinguActionAbstractFactory& operator= (const PinguActionAbstractFactory&);
};

template<class T>
class PinguActionFactoryImpl : public PinguActionAbstractFactory
{
public:
  PinguActionFactoryImpl (Actions::ActionName id)
    : PinguActionAbstractFactory (id)
  {
  }

  PinguAction* create (Pingu* p) {
    return new T (p);
  }

private:
  PinguActionFactoryImpl (const PinguActionFactoryImpl&);
  PinguActionFactoryImpl& operator= (const PinguActionFactoryImpl&);
};

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
	instance_ = 0;
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
