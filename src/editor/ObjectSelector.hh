// $Id: ObjectSelector.hh,v 1.8 2000/06/18 17:01:50 grumbel Exp $
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

#include <string>
#include <list>

#include "EditorObj.hh"
#include "PLFObj.hh"
#include "PSMObj.hh"
#include "SurfaceSelector.hh"

///
class ObjectSelector
{
private:
  ///
  CL_Font* font;
  ///
  int x_offset;
  ///
  int y_offset;
  ///
  std::string last_object;
  ///
  bool data_loaded;
  
public:
  ///
  ObjectSelector();
  ///
  ~ObjectSelector();
  
  ///
  EditorObj* select_obj_type();
  ///
  std::string select_surface(std::vector<surface_obj>& sur_list);
  ///
  int    read_key();
  ///
  std::string read_string(std::string, std::string);
  
  ///
  EditorObj* get_obj(int, int);
  ///
  EditorObj* get_trap();
  ///
  EditorObj* get_groundpiece(surface_data::Type type);
  ///
  EditorObj* get_hotspot();
  ///
  EditorObj* get_entrance();
  ///
  EditorObj* get_exit();
}///
;

#endif

/* EOF */


