//  $Id: SpriteEditorObj.cc,v 1.1 2001/08/16 17:46:51 grumbel Exp $
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

#include "EditorView.hh"
#include "SpriteEditorObj.hh"


SpriteEditorObj::SpriteEditorObj (CL_Vector& arg_pos)
  : pos_ref (arg_pos)
{
}

SpriteEditorObj::SpriteEditorObj (const ResDescriptor& desc,
				  CL_Vector& arg_pos)
  : sprite (desc),
    pos_ref (arg_pos)
{
}

SpriteEditorObj::SpriteEditorObj (const std::string& sur_name,
				  const std::string& datafile,
				  CL_Vector& arg_pos)
  : sprite (sur_name, datafile),
    pos_ref (arg_pos)
{
}

SpriteEditorObj::~SpriteEditorObj ()
{
}

void 
SpriteEditorObj::draw(boost::dummy_ptr<EditorView> view)
{
  view->draw (sprite, pos_ref);
}


void 
SpriteEditorObj::set_position_offset(const CL_Vector& offset)
{
  pos_ref += offset;
}

CL_Vector 
SpriteEditorObj::get_upper_left_corner() 
{ 
  CL_Vector pos(pos_ref);
  return pos + CL_Vector(sprite.get_x_align (), sprite.get_y_align ()); 
}

/* EOF */
