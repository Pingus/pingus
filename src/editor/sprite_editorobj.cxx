//  $Id: sprite_editorobj.cxx,v 1.9 2003/03/31 23:26:13 grumbel Exp $
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

#include <iostream>
#include <assert.h>
#include <ClanLib/Display/Display/surfaceprovider.h>
#include "../math.hxx"
#include "editor_view.hxx"
#include "sprite_editorobj.hxx"

SpriteEditorObj::SpriteEditorObj ()
  : pos_ref (0)
{
}

SpriteEditorObj::SpriteEditorObj (Vector* arg_pos)
  : pos_ref (arg_pos)
{
}

SpriteEditorObj::SpriteEditorObj (const ResDescriptor& desc,
				  Vector* arg_pos)
  : sprite (desc),
    pos_ref (arg_pos)
{
}

SpriteEditorObj::SpriteEditorObj (const std::string& sur_name,
				  const std::string& datafile,
				  Vector* arg_pos)
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

SpriteEditorObj&
SpriteEditorObj::operator= (const SpriteEditorObj& old)
{
  if (this != &old)
    {    
      RectEditorObj::operator=(old);
  
      sprite  = old.sprite;
      pos_ref = old.pos_ref;
    }
    
  return *this;
}

float
SpriteEditorObj::get_z_pos() 
{
  assert (pos_ref);
  return pos_ref->z;
}

void 
SpriteEditorObj::draw(EditorNS::EditorView * view)
{
  assert (pos_ref);
  view->draw (sprite, *pos_ref);
}


void 
SpriteEditorObj::set_position_offset(const Vector& offset)
{
  assert (pos_ref);
  *pos_ref += offset;
}

Vector 
SpriteEditorObj::get_upper_left_corner() 
{ 
  assert (pos_ref);
  Vector pos(*pos_ref);
  return pos + Vector(sprite.get_x_align (), sprite.get_y_align ()); 
}

bool
SpriteEditorObj::is_over(const Vector& pos)
{
  // FIXME: We don't handle animated objects special (do we need to?)
  if (RectEditorObj::is_over (pos))
    {
#if 1 // EDITOR_PIXEL_PERFECT_IS_OVER
      CL_SurfaceProvider* provider = sprite.get_surface ().get_provider ();
      if (provider)
        {
          // Position relative to the surface, not world
          int provider_pos_x = (int)pos.x;
          int provider_pos_y = (int)pos.y;

          provider_pos_x -= int(pos_ref->x);
          provider_pos_y -= int(pos_ref->y);

          provider_pos_x -= sprite.get_x_align();
          provider_pos_y -= sprite.get_y_align();

          provider_pos_x = Math::mid(0, provider_pos_x, int(provider->get_width()-1));
          provider_pos_y = Math::mid(0, provider_pos_y, int(provider->get_height()-1));

          float r, g, b, a;
          provider->get_pixel (int(provider_pos_x), int(provider_pos_y), &r, &g, &b, &a);
          return (a > 0.0f);
        }
      else
        {
          std::cout << "ERROR: SpriteEditorObj: Failed to get provider for surface" << std::endl;
          return true;
        }
#else
      return true;
#endif
    }
  else
    {
      return false;
    }
}

/* EOF */
