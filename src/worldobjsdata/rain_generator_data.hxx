//  $Id: rain_generator_data.hxx,v 1.1 2002/09/16 15:47:36 grumbel Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_RAIN_GENERATOR_DATA_HXX
#define HEADER_PINGUS_RAIN_GENERATOR_DATA_HXX

#include "../libxmlfwd.hxx"
#include "../worldobj_data.hxx"

namespace WorldObjsData {

/** */
class RainGeneratorData : public WorldObjData
{
private:
public:
  RainGeneratorData(xmlDocPtr doc, xmlNodePtr cur) {}

  WorldObj* create_WorldObj ();
  EditorObjLst create_EditorObj ();

  void write_xml(std::ostream& xml);
private:
  RainGeneratorData (const RainGeneratorData&);
  RainGeneratorData operator= (const RainGeneratorData&);
};

} // namespace WorldObjsData

#endif

/* EOF */
