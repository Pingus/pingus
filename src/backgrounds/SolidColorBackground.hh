//  $Id: SolidColorBackground.hh,v 1.9 2002/01/03 13:29:10 grumbel Exp $
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

#ifndef SOLIDCOLORBACKGROUND_HH
#define SOLIDCOLORBACKGROUND_HH

#include "../boost/smart_ptr.hpp"
#include "../Color.hh"
#include "../WorldObj.hh"
#include "../WorldObjData.hh"

class SolidColorBackgroundData : public WorldObjData
{
public:
  Color color;

  // FIXME: Add z_pos handling here

  SolidColorBackgroundData() {}
  virtual ~SolidColorBackgroundData() {}

  /** Writte the content of this object formated as xml to the given
      stream */
  virtual void write_xml(std::ofstream* xml);
  static boost::shared_ptr<WorldObjData> create (xmlDocPtr doc, xmlNodePtr cur);

  boost::shared_ptr<WorldObj> create_WorldObj();
  EditorObjLst create_EditorObj();
};

class SolidColorBackground : public WorldObj,
			     private SolidColorBackgroundData
{
public:
  SolidColorBackground (const SolidColorBackgroundData&);
  virtual ~SolidColorBackground () {}

  // FIXME: Make z_position editable
  float get_z_pos() const { return -10; }  
  void update (float delta) {}
  void draw_offset (int x_of, int y_of, float s = 1.0);
};

#endif

/* EOF */
