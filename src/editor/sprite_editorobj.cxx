//  $Id: sprite_editorobj.cxx,v 1.5 2002/09/10 14:33:07 grumbel Exp $
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

#include <assert.h>
#include <ClanLib/Display/Display/surfaceprovider.h>
#include "editor_view.hxx"
#include "sprite_editorobj.hxx"

SpriteEditorObj::SpriteEditorObj ()
  : pos_ref (0)
{
}

SpriteEditorObj::SpriteEditorObj (CL_Vector* arg_pos)
  : pos_ref (arg_pos)
{
}

SpriteEditorObj::SpriteEditorObj (const ResDescriptor& desc,
				  CL_Vector* arg_pos)
  : sprite (desc),
    pos_ref (arg_pos)
{
}

SpriteEditorObj::SpriteEditorObj (const std::string& sur_name,
				  const std::string& datafile,
				  CL_Vector* arg_pos)
  : sprite (sur_name, datafile),
    pos_ref (arg_pos)
{
}

SpriteEditorObj::SpriteEditorObj (const SpriteEditorObj& old)
  : RectEditorObj(old),
    sprite(old.sprite),
    pos_ref(old.pos_ref)
{
}

void
SpriteEditorObj::operator= (const SpriteEditorObj& old)
{
  if (this != &old)
    {    
      RectEditorObj::operator=(old);
  
      sprite  = old.sprite;
      pos_ref = old.pos_ref;
    }
}

float
SpriteEditorObj::get_z_pos() 
{
  assert (pos_ref);
  return pos_ref->z;
}

void 
SpriteEditorObj::draw(EditorView * view)
{
  assert (pos_ref);
  view->draw (sprite, *pos_ref);
}


void 
SpriteEditorObj::set_position_offset(const CL_Vector& offset)
{
  assert (pos_ref);
  *pos_ref += offset;
}

CL_Vector 
SpriteEditorObj::get_upper_left_corner() 
{ 
  assert (pos_ref);
  CL_Vector pos(*pos_ref);
  return pos + CL_Vector(sprite.get_x_align (), sprite.get_y_align ()); 
}

bool
SpriteEditorObj::is_over(const CL_Vector& pos)
{
  // FIXME: We don't handle animated objects special (do we need to?)
  if (RectEditorObj::is_over (pos))
    {
      //std::cout << "ClickPos: " << pos.x << ", " << pos.y
	//<< " ObjectPos: " << pos_ref.x << ", " << pos_ref.y << std::endl;
  
      CL_SurfaceProvider* provider = sprite.get_surface ().get_provider ();
      float r, g, b, a;
      // Position relative to the surface, not world
      CL_Vector provider_pos = pos;
      provider_pos -= *pos_ref;

      provider->get_pixel (int(provider_pos.x), int(provider_pos.y), &r, &g, &b, &a);
      //std::cout << "Color: " << r << " " << g << " " << b << " " << a << std::endl;
      return (a >= 0.0f);
    }
  else
    {
      return false;
    }
}

/* EOF */
