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

// Desktop GL
#  include <SDL_opengl.h>

#  if defined(_WIN32)
// opengl32.dll only has the 1.1 ABI. Declare modern entry points as function
// pointers loaded via SDL_GL_GetProcAddress (see opengl_wgl_load.cpp).
#    include <SDL_opengl_glext.h>

extern PFNGLACTIVETEXTUREPROC              glActiveTexture;
extern PFNGLATTACHSHADERPROC               glAttachShader;
extern PFNGLBINDBUFFERPROC                 glBindBuffer;
extern PFNGLBINDVERTEXARRAYPROC            glBindVertexArray;
extern PFNGLBUFFERDATAPROC                 glBufferData;
extern PFNGLCOMPILESHADERPROC              glCompileShader;
extern PFNGLCREATEPROGRAMPROC              glCreateProgram;
extern PFNGLCREATESHADERPROC               glCreateShader;
extern PFNGLDELETEBUFFERSPROC              glDeleteBuffers;
extern PFNGLDELETEPROGRAMPROC              glDeleteProgram;
extern PFNGLDELETESHADERPROC               glDeleteShader;
extern PFNGLDELETEVERTEXARRAYSPROC         glDeleteVertexArrays;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC   glDisableVertexAttribArray;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC    glEnableVertexAttribArray;
extern PFNGLGENBUFFERSPROC                 glGenBuffers;
extern PFNGLGENVERTEXARRAYSPROC            glGenVertexArrays;
extern PFNGLGETATTRIBLOCATIONPROC          glGetAttribLocation;
extern PFNGLGETPROGRAMINFOLOGPROC          glGetProgramInfoLog;
extern PFNGLGETPROGRAMIVPROC               glGetProgramiv;
extern PFNGLGETSHADERINFOLOGPROC           glGetShaderInfoLog;
extern PFNGLGETSHADERIVPROC                glGetShaderiv;
extern PFNGLGETUNIFORMLOCATIONPROC         glGetUniformLocation;
extern PFNGLLINKPROGRAMPROC                glLinkProgram;
extern PFNGLSHADERSOURCEPROC               glShaderSource;
extern PFNGLUNIFORM1IPROC                  glUniform1i;
extern PFNGLUNIFORM4FVPROC                 glUniform4fv;
extern PFNGLUNIFORMMATRIX4FVPROC           glUniformMatrix4fv;
extern PFNGLUSEPROGRAMPROC                 glUseProgram;
extern PFNGLVERTEXATTRIBPOINTERPROC        glVertexAttribPointer;

namespace pingus {
void opengl_load_procs();
}

#  else
// Linux/macOS: request extension prototypes linked from libGL.
#    ifndef GL_GLEXT_PROTOTYPES
#      define GL_GLEXT_PROTOTYPES 1
#    endif
#    include <SDL_opengl_glext.h>
#    if defined(__has_include)
#      if __has_include(<GL/glext.h>)
#        include <GL/glext.h>
#      endif
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
