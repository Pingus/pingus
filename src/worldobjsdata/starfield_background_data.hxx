//  $Id: starfield_background_data.hxx,v 1.3 2003/02/26 17:08:30 grumbel Exp $
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

#ifndef HEADER_PINGUS_WORLDOBJSDATA_STARFIELD_BACKGROUND_DATA_HXX
#define HEADER_PINGUS_WORLDOBJSDATA_STARFIELD_BACKGROUND_DATA_HXX

#include "../worldobj_data.hxx"
#include "../libxmlfwd.hxx"

namespace WorldObjsData {

class StarfieldBackgroundData : public WorldObjData
{
public:
  int  small_stars_count;
  int middle_stars_count;
  int  large_stars_count;

  StarfieldBackgroundData ();
  StarfieldBackgroundData (const StarfieldBackgroundData& old);
  StarfieldBackgroundData (xmlDocPtr doc, xmlNodePtr cur);

  void write_xml (std::ostream& xml);

  void    insert_WorldObjs (World*);
  EditorObjLst create_EditorObj ();

private:
  StarfieldBackgroundData& operator= (const StarfieldBackgroundData&);
};

} // namespace WorldObjsData

#endif

/* EOF */
