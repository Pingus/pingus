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

#include "xml_file_reader_old.hxx"
#include "libxmlfwd.hxx"

namespace Pingus {

class WorldObj;
class WorldObjAbstractFactory;

/** WorldObjFactory which can be used to create all kinds of
    WorldObj's by given its id */
class WorldObjFactory
{
private:
  std::map<std::string, WorldObjAbstractFactory*> factories;

  static WorldObjFactory* instance_;

  WorldObjFactory ();
  void free_factories();
public:
  /** Return the singleton instance */
  static WorldObjFactory* instance ();
  static void deinit();
  /** Register a factory for object creation */
  void register_factory(const std::string& id,
                        WorldObjAbstractFactory* factory);

  /** Create a WorldObj type from a given piece of xml, use the
      'type' property for determinating the object type. */
  WorldObj* create (xmlDocPtr doc, xmlNodePtr cur);

  /** Create a WorldObj type from a given piece of xml, use the
      given id value for determinating the object type instead of the
      'type' property. This is for backward compatibility only! */
  WorldObj* create (const std::string& id,
			xmlDocPtr doc, xmlNodePtr cur);

private:
  WorldObjFactory (const WorldObjFactory&);
  WorldObjFactory& operator= (const WorldObjFactory&);
};

/** WorldObjAbstractFactory, interface for creating factories */
class WorldObjAbstractFactory
{
public:
  WorldObjAbstractFactory (const std::string& id) {
    WorldObjFactory::instance ()->register_factory (id, this);
  }

  virtual WorldObj* create (xmlDocPtr doc, xmlNodePtr cur) =0;

private:
  WorldObjAbstractFactory (const WorldObjAbstractFactory&);
  WorldObjAbstractFactory& operator= (const WorldObjAbstractFactory&);
};

/** Template to create factories, usage:
    new WorldObjFactoryImpl<"liquid", Liquid>; */
template<class T>
class WorldObjFactoryImpl : public WorldObjAbstractFactory
{
public:
  WorldObjFactoryImpl (const std::string& id)
    : WorldObjAbstractFactory (id) {}

  WorldObj* create (xmlDocPtr doc, xmlNodePtr cur) {
    return new T(XMLFileReaderOld(doc, cur));
  }

private:
  WorldObjFactoryImpl (const WorldObjFactoryImpl&);
  WorldObjFactoryImpl& operator= (const WorldObjFactoryImpl&);
};

} // namespace Pingus

#endif

/* EOF */
