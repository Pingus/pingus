//  $Id: PinguActionFactory.cc,v 1.3 2002/01/26 00:36:39 grumbel Exp $
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

#include "PingusError.hh"

#include "actions/RocketLauncher.hh"
#include "actions/Boarder.hh"
#include "actions/Superman.hh"
#include "actions/Angel.hh"
#include "actions/basher.hh"
#include "actions/blocker.hh"
#include "actions/bomber.hh"
#include "actions/bridger.hh"
#include "actions/climber.hh"
#include "actions/digger.hh"
#include "actions/exiter.hh"
#include "actions/floater.hh"
#include "actions/miner.hh"
#include "actions/Jumper.hh"
#include "actions/Slider.hh"
#include "actions/teleported.hh"
#include "actions/exiter.hh"
#include "actions/smashed.hh"
#include "actions/LaserKill.hh"
#include "actions/Splashed.hh"
#include "actions/Waiter.hh"
#include "actions/Drown.hh"
#include "actions/Faller.hh"
#include "actions/Walker.hh"

#include "PinguActionFactory.hh"

PinguActionFactory::PinguActionFactory ()
{
}

PinguActionFactory* PinguActionFactory::instance_ = 0;

PinguActionFactory* 
PinguActionFactory::instance ()
{
  if (instance_ == 0)
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
    return it->second->create ();
}

boost::shared_ptr<PinguAction> 
PinguActionFactory::create_sp (const std::string& id)
{
  return boost::shared_ptr<PinguAction>(create (id));
}

void 
PinguActionFactory::register_factory (const std::string& id, PinguActionAbstractFactory* factory)
{
  // FIXME: Could need some more error checking
  factories[id] = factory;
}

/* EOF */
