//  $Id: EditorWorldObj.hh,v 1.2 2000/09/25 16:29:43 grumbel Exp $
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

#ifndef EDITORWORLDOBJ_HH
#define EDITORWORLDOBJ_HH

#include "../WorldObjData.hh"
#include "PLFObj.hh"

///
class EditorWorldObj : public PLFObj
{
private:
public:
  ///
  EditorWorldObj ();
  ///
  virtual ~EditorWorldObj () {}
  ///
  static EditorWorldObj* create (WorldObjData*);
  ///
  virtual EditorObj* duplicate();
  ///
  virtual void save(std::ofstream* plf, std::ofstream* psm) {}
  ///
  virtual void save_xml(std::ofstream* xml);
  ///
  virtual std::string obj_type() { return "WorldObj"; };
};

#endif

/* EOF */
