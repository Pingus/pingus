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

#include "engine/display/opengl/opengl_framebuffer_surface_impl.hpp"

#include <assert.h>

namespace {

inline int next_power_of_two(int val)
{
  int result = 1;
  while(result < val)
    result *= 2;
  return result;
}

} // namespace 

OpenGLFramebufferSurfaceImpl::OpenGLFramebufferSurfaceImpl(SDL_Surface* src) :
  m_handle(),
  m_size(src->w, src->h),
  m_texture_size()
{
  glGenTextures(1, &m_handle);
  
  m_texture_size.width  = next_power_of_two(src->w);
  m_texture_size.height = next_power_of_two(src->h);

  //  Convert the src surface to a format usable for upload to OpenGL
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  SDL_Surface* convert = SDL_CreateRGBSurface(0,
                                              m_texture_size.width, m_texture_size.height, 32,
                                              0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
#else
  SDL_Surface* convert = SDL_CreateRGBSurface(0,
                                              m_texture_size.width, m_texture_size.height, 32,
                                              0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
#endif
  SDL_SetSurfaceAlphaMod(src, 0);
  SDL_BlitSurface(src, 0, convert, 0);
  
  GLenum sdl_format;
  if(convert->format->BytesPerPixel == 3)
    sdl_format = GL_RGB;
  else if(convert->format->BytesPerPixel == 4)
    sdl_format = GL_RGBA;
  else
    assert(!"OpenGLFramebufferSurfaceImpl: Unsupported surface format");

  glBindTexture(GL_TEXTURE_2D, m_handle);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glPixelStorei(GL_UNPACK_ROW_LENGTH, convert->pitch/convert->format->BytesPerPixel);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Upload the surface to a texture
  SDL_LockSurface(convert);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_texture_size.width, m_texture_size.height, 0,
               sdl_format, GL_UNSIGNED_BYTE, convert->pixels);
  SDL_UnlockSurface(convert);
   
  SDL_FreeSurface(convert);  

  // Unbind the texture
  glBindTexture(GL_TEXTURE_2D, 0);
}

OpenGLFramebufferSurfaceImpl::~OpenGLFramebufferSurfaceImpl()
{
  glDeleteTextures(1, &m_handle);
}

/* EOF */
