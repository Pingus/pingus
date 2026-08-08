// Pingus - A free Lemmings clone
// Copyright (C) 2026 Ingo Ruhnke <grumbel@gmail.com>
//
// Shared OpenGL 3.3 Core / OpenGL ES 2.0 compatibility header.

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_OPENGL_OPENGL_COMPAT_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_OPENGL_OPENGL_COMPAT_HPP

#ifdef _WIN32
#  define NOGDI
#endif

#if defined(PINGUS_USE_GLES) || defined(PINGUS_EMSCRIPTEN)

#  include <SDL_opengles2.h>
#  ifndef PINGUS_GL_ES
#    define PINGUS_GL_ES 1
#  endif

#else

// Desktop: SDL_opengl.h alone only exposes legacy 1.x prototypes. Request
// extension prototypes so GL 2+/3 entry points (shaders, VBOs, VAOs) exist.
#  ifndef GL_GLEXT_PROTOTYPES
#    define GL_GLEXT_PROTOTYPES 1
#  endif
#  include <SDL_opengl.h>
#  include <SDL_opengl_glext.h>

// System headers (fallback when SDL's glext is incomplete).
#  if defined(__has_include)
#    if __has_include(<GL/glext.h>)
#      include <GL/glext.h>
#    endif
#  endif

#  ifndef PINGUS_GL_ES
#    define PINGUS_GL_ES 0
#  endif

#endif

namespace pingus {
namespace glapi {

inline void pixel_store_unpack_row_length(int /*pixels*/)
{
#if !PINGUS_GL_ES
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
}

} // namespace glapi
} // namespace pingus

#endif

/* EOF */
