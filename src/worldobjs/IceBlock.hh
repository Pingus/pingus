//  $Id: IceBlock.hh,v 1.19 2002/06/07 20:35:14 torangan Exp $
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

#ifndef ICEBLOCK_HH
#define ICEBLOCK_HH

#include <ClanLib/core.h>

#include "../WorldObjData.hh"
#include "../editor/SpriteEditorObj.hh"

class WorldObj;

namespace std {
  template <class T> class char_traits;
  template <class T1, class T2> struct basic_ofstream;
  typedef struct basic_ofstream<char, char_traits<char> > ofstream;
}

namespace boost {
  template <class T> class shared_ptr;
}


class IceBlockData : public WorldObjData
{
public:
  /// The upper/left position  of the iceblock's
  CL_Vector pos;
  /** The number of iceblocks, only complete blocks are supported */
  int width;

  int last_contact;

  IceBlockData ();

  /** Write the content of this object formatted as xml to the given
      stream */
  void write_xml(std::ofstream* xml);
  ///
  static boost::shared_ptr<WorldObjData> create(xmlDocPtr doc, xmlNodePtr cur);

  /** Create an WorldObj from the given data object */
  boost::shared_ptr<WorldObj> create_WorldObj ();

  /** Create an EditorObj from the given data object */
  std::list<boost::shared_ptr<EditorObj> > create_EditorObj ();
};

class IceBlock : public IceBlockData, 
		 public WorldObj
{
private:
  CL_Surface block_sur;
  float thickness;
  bool is_finished;
public:
  IceBlock (const IceBlockData& data);

  float get_z_pos() const { return 100; }
  void draw_colmap();
  void draw_offset(int x, int y, float s = 1.0);
  void update(float delta);
};


class EditorIceBlockObj : public IceBlockData,
			  public SpriteEditorObj			  
{
private:
  
public:
  EditorIceBlockObj (const IceBlockData& data);

  /** Create the object with resonable defaults */
  static std::list<boost::shared_ptr<EditorObj> > create (const CL_Vector& pos);

  void write_xml(std::ofstream* xml) { IceBlockData::write_xml (xml); }
  boost::shared_ptr<EditorObj> duplicate();
  std::string status_line();
};

#endif

/* EOF */
