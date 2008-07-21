//  Pingus - A free Lemmings clone
//  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_OPENGL_FRAMEBUFFER_SURFACE_IMPL_HPP
#define HEADER_OPENGL_FRAMEBUFFER_SURFACE_IMPL_HPP

#ifndef MACOSX
#include <GL/gl.h>
#include <GL/glext.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#endif

#include "SDL.h"
#include "framebuffer_surface.hpp"

class OpenGLFramebufferSurfaceImpl : public FramebufferSurfaceImpl
{
private:
  GLuint handle;
  Size   size;
  Size   texture_size;

public:
  OpenGLFramebufferSurfaceImpl(SDL_Surface* src);

  int get_width()  const { return size.width;  }
  int get_height() const { return size.height; }

  GLuint get_handle() const { return handle; }
  Size get_texture_size() const { return texture_size; }
  Size get_size() const { return size; }

  Surface to_surface() const;
};

#endif

/* EOF */
