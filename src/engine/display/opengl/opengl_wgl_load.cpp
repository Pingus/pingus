// Pingus - A free Lemmings clone
// Copyright (C) 2026 Ingo Ruhnke <grumbel@gmail.com>

#include "engine/display/opengl/opengl_compat.hpp"

#if defined(_WIN32) && !PINGUS_GL_ES

#include <stdexcept>

#include <SDL.h>

#include "util/raise_exception.hpp"

// Header macros would rename these identifiers; strip them for definitions.
#undef glActiveTexture
#undef glAttachShader
#undef glBindBuffer
#undef glBindVertexArray
#undef glBufferData
#undef glCompileShader
#undef glCreateProgram
#undef glCreateShader
#undef glDeleteBuffers
#undef glDeleteProgram
#undef glDeleteShader
#undef glDeleteVertexArrays
#undef glDisableVertexAttribArray
#undef glEnableVertexAttribArray
#undef glGenBuffers
#undef glGenVertexArrays
#undef glGetAttribLocation
#undef glGetProgramInfoLog
#undef glGetProgramiv
#undef glGetShaderInfoLog
#undef glGetShaderiv
#undef glGetUniformLocation
#undef glLinkProgram
#undef glShaderSource
#undef glUniform1i
#undef glUniform4fv
#undef glUniformMatrix4fv
#undef glUseProgram
#undef glVertexAttribPointer

PFNGLACTIVETEXTUREPROC              pingus_glActiveTexture = nullptr;
PFNGLATTACHSHADERPROC               pingus_glAttachShader = nullptr;
PFNGLBINDBUFFERPROC                 pingus_glBindBuffer = nullptr;
PFNGLBINDVERTEXARRAYPROC            pingus_glBindVertexArray = nullptr;
PFNGLBUFFERDATAPROC                 pingus_glBufferData = nullptr;
PFNGLCOMPILESHADERPROC              pingus_glCompileShader = nullptr;
PFNGLCREATEPROGRAMPROC              pingus_glCreateProgram = nullptr;
PFNGLCREATESHADERPROC               pingus_glCreateShader = nullptr;
PFNGLDELETEBUFFERSPROC              pingus_glDeleteBuffers = nullptr;
PFNGLDELETEPROGRAMPROC              pingus_glDeleteProgram = nullptr;
PFNGLDELETESHADERPROC               pingus_glDeleteShader = nullptr;
PFNGLDELETEVERTEXARRAYSPROC         pingus_glDeleteVertexArrays = nullptr;
PFNGLDISABLEVERTEXATTRIBARRAYPROC   pingus_glDisableVertexAttribArray = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC    pingus_glEnableVertexAttribArray = nullptr;
PFNGLGENBUFFERSPROC                 pingus_glGenBuffers = nullptr;
PFNGLGENVERTEXARRAYSPROC            pingus_glGenVertexArrays = nullptr;
PFNGLGETATTRIBLOCATIONPROC          pingus_glGetAttribLocation = nullptr;
PFNGLGETPROGRAMINFOLOGPROC          pingus_glGetProgramInfoLog = nullptr;
PFNGLGETPROGRAMIVPROC               pingus_glGetProgramiv = nullptr;
PFNGLGETSHADERINFOLOGPROC           pingus_glGetShaderInfoLog = nullptr;
PFNGLGETSHADERIVPROC                pingus_glGetShaderiv = nullptr;
PFNGLGETUNIFORMLOCATIONPROC         pingus_glGetUniformLocation = nullptr;
PFNGLLINKPROGRAMPROC                pingus_glLinkProgram = nullptr;
PFNGLSHADERSOURCEPROC               pingus_glShaderSource = nullptr;
PFNGLUNIFORM1IPROC                  pingus_glUniform1i = nullptr;
PFNGLUNIFORM4FVPROC                 pingus_glUniform4fv = nullptr;
PFNGLUNIFORMMATRIX4FVPROC           pingus_glUniformMatrix4fv = nullptr;
PFNGLUSEPROGRAMPROC                 pingus_glUseProgram = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC        pingus_glVertexAttribPointer = nullptr;

namespace pingus {

namespace {

template<typename T>
T load_proc(char const* name)
{
  void* p = SDL_GL_GetProcAddress(name);
  if (!p)
    raise_exception(std::runtime_error,
                    "SDL_GL_GetProcAddress failed for " << name);
  return reinterpret_cast<T>(p);
}

} // namespace

void opengl_load_procs()
{
  pingus_glActiveTexture = load_proc<PFNGLACTIVETEXTUREPROC>("glActiveTexture");
  pingus_glAttachShader = load_proc<PFNGLATTACHSHADERPROC>("glAttachShader");
  pingus_glBindBuffer = load_proc<PFNGLBINDBUFFERPROC>("glBindBuffer");
  pingus_glBindVertexArray = load_proc<PFNGLBINDVERTEXARRAYPROC>("glBindVertexArray");
  pingus_glBufferData = load_proc<PFNGLBUFFERDATAPROC>("glBufferData");
  pingus_glCompileShader = load_proc<PFNGLCOMPILESHADERPROC>("glCompileShader");
  pingus_glCreateProgram = load_proc<PFNGLCREATEPROGRAMPROC>("glCreateProgram");
  pingus_glCreateShader = load_proc<PFNGLCREATESHADERPROC>("glCreateShader");
  pingus_glDeleteBuffers = load_proc<PFNGLDELETEBUFFERSPROC>("glDeleteBuffers");
  pingus_glDeleteProgram = load_proc<PFNGLDELETEPROGRAMPROC>("glDeleteProgram");
  pingus_glDeleteShader = load_proc<PFNGLDELETESHADERPROC>("glDeleteShader");
  pingus_glDeleteVertexArrays = load_proc<PFNGLDELETEVERTEXARRAYSPROC>("glDeleteVertexArrays");
  pingus_glDisableVertexAttribArray = load_proc<PFNGLDISABLEVERTEXATTRIBARRAYPROC>("glDisableVertexAttribArray");
  pingus_glEnableVertexAttribArray = load_proc<PFNGLENABLEVERTEXATTRIBARRAYPROC>("glEnableVertexAttribArray");
  pingus_glGenBuffers = load_proc<PFNGLGENBUFFERSPROC>("glGenBuffers");
  pingus_glGenVertexArrays = load_proc<PFNGLGENVERTEXARRAYSPROC>("glGenVertexArrays");
  pingus_glGetAttribLocation = load_proc<PFNGLGETATTRIBLOCATIONPROC>("glGetAttribLocation");
  pingus_glGetProgramInfoLog = load_proc<PFNGLGETPROGRAMINFOLOGPROC>("glGetProgramInfoLog");
  pingus_glGetProgramiv = load_proc<PFNGLGETPROGRAMIVPROC>("glGetProgramiv");
  pingus_glGetShaderInfoLog = load_proc<PFNGLGETSHADERINFOLOGPROC>("glGetShaderInfoLog");
  pingus_glGetShaderiv = load_proc<PFNGLGETSHADERIVPROC>("glGetShaderiv");
  pingus_glGetUniformLocation = load_proc<PFNGLGETUNIFORMLOCATIONPROC>("glGetUniformLocation");
  pingus_glLinkProgram = load_proc<PFNGLLINKPROGRAMPROC>("glLinkProgram");
  pingus_glShaderSource = load_proc<PFNGLSHADERSOURCEPROC>("glShaderSource");
  pingus_glUniform1i = load_proc<PFNGLUNIFORM1IPROC>("glUniform1i");
  pingus_glUniform4fv = load_proc<PFNGLUNIFORM4FVPROC>("glUniform4fv");
  pingus_glUniformMatrix4fv = load_proc<PFNGLUNIFORMMATRIX4FVPROC>("glUniformMatrix4fv");
  pingus_glUseProgram = load_proc<PFNGLUSEPROGRAMPROC>("glUseProgram");
  pingus_glVertexAttribPointer = load_proc<PFNGLVERTEXATTRIBPOINTERPROC>("glVertexAttribPointer");
}

} // namespace pingus

#endif

/* EOF */
