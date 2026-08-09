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
// SDL_opengl.h (MinGW) already declares some 1.2+ entry points as functions,
// but opengl32.dll does not provide them. Use uniquely named function pointers
// loaded via SDL_GL_GetProcAddress, then macro-redirect the usual gl* names.
#    include <SDL_opengl_glext.h>

extern PFNGLACTIVETEXTUREPROC              pingus_glActiveTexture;
extern PFNGLATTACHSHADERPROC               pingus_glAttachShader;
extern PFNGLBINDBUFFERPROC                 pingus_glBindBuffer;
extern PFNGLBINDVERTEXARRAYPROC            pingus_glBindVertexArray;
extern PFNGLBUFFERDATAPROC                 pingus_glBufferData;
extern PFNGLCOMPILESHADERPROC              pingus_glCompileShader;
extern PFNGLCREATEPROGRAMPROC              pingus_glCreateProgram;
extern PFNGLCREATESHADERPROC               pingus_glCreateShader;
extern PFNGLDELETEBUFFERSPROC              pingus_glDeleteBuffers;
extern PFNGLDELETEPROGRAMPROC              pingus_glDeleteProgram;
extern PFNGLDELETESHADERPROC               pingus_glDeleteShader;
extern PFNGLDELETEVERTEXARRAYSPROC         pingus_glDeleteVertexArrays;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC   pingus_glDisableVertexAttribArray;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC    pingus_glEnableVertexAttribArray;
extern PFNGLGENBUFFERSPROC                 pingus_glGenBuffers;
extern PFNGLGENVERTEXARRAYSPROC            pingus_glGenVertexArrays;
extern PFNGLGETATTRIBLOCATIONPROC          pingus_glGetAttribLocation;
extern PFNGLGETPROGRAMINFOLOGPROC          pingus_glGetProgramInfoLog;
extern PFNGLGETPROGRAMIVPROC               pingus_glGetProgramiv;
extern PFNGLGETSHADERINFOLOGPROC           pingus_glGetShaderInfoLog;
extern PFNGLGETSHADERIVPROC                pingus_glGetShaderiv;
extern PFNGLGETUNIFORMLOCATIONPROC         pingus_glGetUniformLocation;
extern PFNGLLINKPROGRAMPROC                pingus_glLinkProgram;
extern PFNGLSHADERSOURCEPROC               pingus_glShaderSource;
extern PFNGLUNIFORM1IPROC                  pingus_glUniform1i;
extern PFNGLUNIFORM4FVPROC                 pingus_glUniform4fv;
extern PFNGLUNIFORMMATRIX4FVPROC           pingus_glUniformMatrix4fv;
extern PFNGLUSEPROGRAMPROC                 pingus_glUseProgram;
extern PFNGLVERTEXATTRIBPOINTERPROC        pingus_glVertexAttribPointer;

#define glActiveTexture            pingus_glActiveTexture
#define glAttachShader             pingus_glAttachShader
#define glBindBuffer               pingus_glBindBuffer
#define glBindVertexArray          pingus_glBindVertexArray
#define glBufferData               pingus_glBufferData
#define glCompileShader            pingus_glCompileShader
#define glCreateProgram            pingus_glCreateProgram
#define glCreateShader             pingus_glCreateShader
#define glDeleteBuffers            pingus_glDeleteBuffers
#define glDeleteProgram            pingus_glDeleteProgram
#define glDeleteShader             pingus_glDeleteShader
#define glDeleteVertexArrays       pingus_glDeleteVertexArrays
#define glDisableVertexAttribArray pingus_glDisableVertexAttribArray
#define glEnableVertexAttribArray  pingus_glEnableVertexAttribArray
#define glGenBuffers               pingus_glGenBuffers
#define glGenVertexArrays          pingus_glGenVertexArrays
#define glGetAttribLocation        pingus_glGetAttribLocation
#define glGetProgramInfoLog        pingus_glGetProgramInfoLog
#define glGetProgramiv             pingus_glGetProgramiv
#define glGetShaderInfoLog         pingus_glGetShaderInfoLog
#define glGetShaderiv              pingus_glGetShaderiv
#define glGetUniformLocation       pingus_glGetUniformLocation
#define glLinkProgram              pingus_glLinkProgram
#define glShaderSource             pingus_glShaderSource
#define glUniform1i                pingus_glUniform1i
#define glUniform4fv               pingus_glUniform4fv
#define glUniformMatrix4fv         pingus_glUniformMatrix4fv
#define glUseProgram               pingus_glUseProgram
#define glVertexAttribPointer      pingus_glVertexAttribPointer

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
