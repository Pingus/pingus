//  $Id: pingu_action_factory.hxx,v 1.12 2003/04/19 10:23:17 torangan Exp $
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

#ifndef HEADER_PINGUS_PINGU_ACTION_FACTORY_HXX
#define HEADER_PINGUS_PINGU_ACTION_FACTORY_HXX

#include <map>
#include <vector>
#include "pingu.hxx"

class PinguAction;
class PinguActionAbstractFactory;

class PinguActionFactory
{
private:
  /** This vector saves all allocated actions to delete them at a later point */
  std::vector<PinguAction*> all_actions;

  std::map<Actions::ActionName, PinguActionAbstractFactory*> factories;
  static PinguActionFactory* instance_;

  PinguActionFactory ();
  ~PinguActionFactory ();
  void register_core_actions ();

public:
  static PinguActionFactory* instance ();
  static void init();
  static void deinit();

  /** \a f will get deleted in destructor! */
  void register_factory (Actions::ActionName id, PinguActionAbstractFactory* f);

  /** Delete all actions which this class has allocated. This needs to
      be called seperatly from the constructor, due to the used
      singleton pattern. [FIXME: if not called memory leak will result] */
  void delete_actions ();

  /** Allocate the given action */
  PinguAction* create (Pingu* p, Actions::ActionName id);

private:
  PinguActionFactory (const PinguActionFactory&);
  PinguActionFactory& operator= (const PinguActionFactory&);
};

class PinguActionAbstractFactory
{
public:
  PinguActionAbstractFactory (Actions::ActionName id) {
    PinguActionFactory::instance ()-> register_factory (id, this);
  }

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

#endif

/* EOF */
