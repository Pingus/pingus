//  $Id: worldobj_data_factory.hxx,v 1.10 2003/10/19 12:25:47 grumbel Exp $
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

#ifndef HEADER_PINGUS_WORLDOBJ_DATA_FACTORY_HXX
#define HEADER_PINGUS_WORLDOBJ_DATA_FACTORY_HXX

#include <map>
#include <string>

#include "libxmlfwd.hxx"

namespace Pingus {

class WorldObjData;
class WorldObjDataAbstractFactory;

/** WorldObjDataFactory which can be used to create all kinds of
    WorldObjData's by given its id */
class WorldObjDataFactory
{
private:
  std::map<std::string, WorldObjDataAbstractFactory*> factories;

  static WorldObjDataFactory* instance_;

  WorldObjDataFactory ();
  void free_factories();
public:
  /** Return the singleton instance */
  static WorldObjDataFactory* instance ();
  static void deinit();
  /** Register a factory for object creation */
  void register_factory(const std::string& id,
                        WorldObjDataAbstractFactory* factory);

  /** Create a WorldObjData type from a given piece of xml, use the
      'type' property for determinating the object type. */
  WorldObjData* create (xmlDocPtr doc, xmlNodePtr cur);

  /** Create a WorldObjData type from a given piece of xml, use the
      given id value for determinating the object type instead of the
      'type' property. This is for backward compatibility only! */
  WorldObjData* create (const std::string& id,
			xmlDocPtr doc, xmlNodePtr cur);

private:
  WorldObjDataFactory (const WorldObjDataFactory&);
  WorldObjDataFactory& operator= (const WorldObjDataFactory&);
};

/** WorldObjDataAbstractFactory, interface for creating factories */
class WorldObjDataAbstractFactory
{
public:
  WorldObjDataAbstractFactory (const std::string& id) {
    WorldObjDataFactory::instance ()->register_factory (id, this);
  }

  virtual WorldObjData* create (xmlDocPtr doc, xmlNodePtr cur) =0;

private:
  WorldObjDataAbstractFactory (const WorldObjDataAbstractFactory&);
  WorldObjDataAbstractFactory& operator= (const WorldObjDataAbstractFactory&);
};

/** Template to create factories, usage:
    new WorldObjDataFactoryImpl<"liquid", Liquid>; */
template<class T>
class WorldObjDataFactoryImpl : public WorldObjDataAbstractFactory
{
public:
  WorldObjDataFactoryImpl (const std::string& id)
    : WorldObjDataAbstractFactory (id) {}

  WorldObjData* create (xmlDocPtr doc, xmlNodePtr cur) {
    return new T (doc, cur);
  }

private:
  WorldObjDataFactoryImpl (const WorldObjDataFactoryImpl&);
  WorldObjDataFactoryImpl& operator= (const WorldObjDataFactoryImpl&);
};

} // namespace Pingus

#endif

/* EOF */
