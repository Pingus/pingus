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
#  include <SDL_opengl.h>
#  ifndef PINGUS_GL_ES
#    define PINGUS_GL_ES 0
#  endif
#endif

namespace pingus {
namespace glapi {

// GLES2 lacks GL_UNPACK_ROW_LENGTH in the core spec.
inline void pixel_store_unpack_row_length(int /*pixels*/)
{
#if !PINGUS_GL_ES
  glPixelStorei(GL_UNPACK_ROW_LENGTH, /*pixels*/ 0);
#endif
}

} // namespace glapi
} // namespace pingus

#endif

/* EOF */
