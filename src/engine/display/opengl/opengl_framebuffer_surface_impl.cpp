//  Pingus - A free Lemmings clone
//  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmail.com>

#include "engine/display/opengl/opengl_framebuffer_surface_impl.hpp"

#include <assert.h>

namespace pingus {

namespace {

inline int next_power_of_two(int val)
{
  int result = 1;
  while (result < val)
    result *= 2;
  return result;
}

} // namespace

OpenGLFramebufferSurfaceImpl::OpenGLFramebufferSurfaceImpl(SDL_Surface* src) :
  m_handle(0),
  m_size(src->w, src->h),
  m_texture_size()
{
  glGenTextures(1, &m_handle);

  // Power-of-two pad keeps sampling simple on ES2 without NPOT restrictions.
  m_texture_size = geom::isize(next_power_of_two(src->w), next_power_of_two(src->h));

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  SDL_Surface* convert = SDL_CreateRGBSurface(0,
                                              m_texture_size.width(), m_texture_size.height(), 32,
                                              0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
#else
  SDL_Surface* convert = SDL_CreateRGBSurface(0,
                                              m_texture_size.width(), m_texture_size.height(), 32,
                                              0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
#endif
  SDL_BlitSurface(src, nullptr, convert, nullptr);

  GLenum sdl_format = GL_RGBA;
  if (convert->format->BytesPerPixel == 3)
    sdl_format = GL_RGB;
  else if (convert->format->BytesPerPixel == 4)
    sdl_format = GL_RGBA;
  else
    assert(false && "OpenGLFramebufferSurfaceImpl: Unsupported surface format");

  glBindTexture(GL_TEXTURE_2D, m_handle);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  // Avoid GL_UNPACK_ROW_LENGTH (not in GLES2); convert surface is tightly packed for 32bpp.

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  SDL_LockSurface(convert);
  GLint internal = GL_RGBA;
#if PINGUS_GL_ES
  internal = GL_RGBA; // ES2: format must match
#endif
  glTexImage2D(GL_TEXTURE_2D, 0, internal, m_texture_size.width(), m_texture_size.height(), 0,
               sdl_format, GL_UNSIGNED_BYTE, convert->pixels);
  SDL_UnlockSurface(convert);
  SDL_FreeSurface(convert);

  glBindTexture(GL_TEXTURE_2D, 0);
}

OpenGLFramebufferSurfaceImpl::~OpenGLFramebufferSurfaceImpl()
{
  glDeleteTextures(1, &m_handle);
}

} // namespace pingus

/* EOF */
