// $Id: object_selector.hxx,v 1.2 2002/06/23 11:17:40 grumbel Exp $
//
// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef OBJECTSELECTOR_HH
#define OBJECTSELECTOR_HH

#include <config.h>
#include <list>
#include "../groundpiece_data.hxx"
#include "surface_selector.hxx"

class EditorObj;

///
class ObjectSelector
{
private:
  CL_Font* font;
  
  int x_offset;
  int y_offset;

  CL_Vector pos;

  std::string last_object;
  std::map<std::string, bool> data_loaded;
  
public:
  ObjectSelector();
  ~ObjectSelector();
  
  std::list<boost::shared_ptr<EditorObj> > select_obj_type();
  std::string select_surface(std::vector<surface_obj>& sur_list);
  std::string select_surface(const std::string & resource_file);
  int    read_key();
  std::string read_string(const std::string &, const std::string &);
  
  std::list<boost::shared_ptr<EditorObj> > get_obj(int, int);
  std::list<boost::shared_ptr<EditorObj> > get_trap();
  std::list<boost::shared_ptr<EditorObj> > get_groundpiece(const GroundpieceData::GPType & gptype);
  std::list<boost::shared_ptr<EditorObj> > get_hotspot(const std::string&);
  std::list<boost::shared_ptr<EditorObj> > get_entrance();
  std::list<boost::shared_ptr<EditorObj> > get_exit();
  std::list<boost::shared_ptr<EditorObj> > get_liquid();
  std::list<boost::shared_ptr<EditorObj> > get_weather();
  std::list<boost::shared_ptr<EditorObj> > get_worldobj();
  std::list<boost::shared_ptr<EditorObj> > get_from_file();
  std::list<boost::shared_ptr<EditorObj> > get_background();
};

#endif

/* EOF */


