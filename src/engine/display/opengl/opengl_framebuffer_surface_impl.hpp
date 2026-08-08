//  Pingus - A free Lemmings clone
//  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmail.com>

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_OPENGL_OPENGL_FRAMEBUFFER_SURFACE_IMPL_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_OPENGL_OPENGL_FRAMEBUFFER_SURFACE_IMPL_HPP

#include "engine/display/opengl/opengl_compat.hpp"
#include "engine/display/framebuffer_surface.hpp"

namespace pingus {

class OpenGLFramebufferSurfaceImpl : public FramebufferSurfaceImpl
{
private:
  GLuint m_handle;
  geom::isize m_size;
  geom::isize m_texture_size;

public:
  OpenGLFramebufferSurfaceImpl(SDL_Surface* src);
  ~OpenGLFramebufferSurfaceImpl() override;

  int get_width()  const override { return m_size.width();  }
  int get_height() const override { return m_size.height(); }

  GLuint get_handle() const { return m_handle; }
  geom::isize get_texture_size() const { return m_texture_size; }
  geom::isize get_size() const { return m_size; }

private:
  OpenGLFramebufferSurfaceImpl(OpenGLFramebufferSurfaceImpl const&);
  OpenGLFramebufferSurfaceImpl& operator=(OpenGLFramebufferSurfaceImpl const&);
};

} // namespace pingus

#endif

/* EOF */
