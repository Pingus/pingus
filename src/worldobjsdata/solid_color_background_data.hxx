//  $Id: solid_color_background_data.hxx,v 1.8 2003/10/18 23:17:28 grumbel Exp $
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

#ifndef HEADER_PINGUS_WORLDOBJSDATA_SOLID_COLOR_BACKGROUND_DATA_HXX
#define HEADER_PINGUS_WORLDOBJSDATA_SOLID_COLOR_BACKGROUND_DATA_HXX

#include <ClanLib/Display/color.h>
#include "../vector.hxx"
#include "../worldobj_data.hxx"
#include "../libxmlfwd.hxx"

namespace Pingus {
namespace WorldObjsData {

class SolidColorBackgroundData : public WorldObjData
{
public:
  Vector pos;
  CL_Colorf     color;

  // FIXME: Add z_pos handling here

  SolidColorBackgroundData () {}
  SolidColorBackgroundData (xmlDocPtr doc, xmlNodePtr cur);
  SolidColorBackgroundData (const SolidColorBackgroundData& old);

  virtual ~SolidColorBackgroundData() {}

  void insert_WorldObjs (World* world);

private:
  SolidColorBackgroundData& operator= (const SolidColorBackgroundData&);
};

} // namespace WorldObjsData
} // namespace Pingus

#endif

/* EOF */
