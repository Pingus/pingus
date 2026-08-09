// Pingus - A free Lemmings clone
// Copyright (C) 2026 Ingo Ruhnke <grumbel@gmail.com>

#include "engine/display/opengl/opengl_compat.hpp"

#if defined(_WIN32) && !PINGUS_GL_ES

#include <stdexcept>

#include <SDL.h>

#include "util/raise_exception.hpp"

PFNGLACTIVETEXTUREPROC              glActiveTexture = nullptr;
PFNGLATTACHSHADERPROC               glAttachShader = nullptr;
PFNGLBINDBUFFERPROC                 glBindBuffer = nullptr;
PFNGLBINDVERTEXARRAYPROC            glBindVertexArray = nullptr;
PFNGLBUFFERDATAPROC                 glBufferData = nullptr;
PFNGLCOMPILESHADERPROC              glCompileShader = nullptr;
PFNGLCREATEPROGRAMPROC              glCreateProgram = nullptr;
PFNGLCREATESHADERPROC               glCreateShader = nullptr;
PFNGLDELETEBUFFERSPROC              glDeleteBuffers = nullptr;
PFNGLDELETEPROGRAMPROC              glDeleteProgram = nullptr;
PFNGLDELETESHADERPROC               glDeleteShader = nullptr;
PFNGLDELETEVERTEXARRAYSPROC         glDeleteVertexArrays = nullptr;
PFNGLDISABLEVERTEXATTRIBARRAYPROC   glDisableVertexAttribArray = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC    glEnableVertexAttribArray = nullptr;
PFNGLGENBUFFERSPROC                 glGenBuffers = nullptr;
PFNGLGENVERTEXARRAYSPROC            glGenVertexArrays = nullptr;
PFNGLGETATTRIBLOCATIONPROC          glGetAttribLocation = nullptr;
PFNGLGETPROGRAMINFOLOGPROC          glGetProgramInfoLog = nullptr;
PFNGLGETPROGRAMIVPROC               glGetProgramiv = nullptr;
PFNGLGETSHADERINFOLOGPROC           glGetShaderInfoLog = nullptr;
PFNGLGETSHADERIVPROC                glGetShaderiv = nullptr;
PFNGLGETUNIFORMLOCATIONPROC         glGetUniformLocation = nullptr;
PFNGLLINKPROGRAMPROC                glLinkProgram = nullptr;
PFNGLSHADERSOURCEPROC               glShaderSource = nullptr;
PFNGLUNIFORM1IPROC                  glUniform1i = nullptr;
PFNGLUNIFORM4FVPROC                 glUniform4fv = nullptr;
PFNGLUNIFORMMATRIX4FVPROC           glUniformMatrix4fv = nullptr;
PFNGLUSEPROGRAMPROC                 glUseProgram = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC        glVertexAttribPointer = nullptr;

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
  glActiveTexture = load_proc<PFNGLACTIVETEXTUREPROC>("glActiveTexture");
  glAttachShader = load_proc<PFNGLATTACHSHADERPROC>("glAttachShader");
  glBindBuffer = load_proc<PFNGLBINDBUFFERPROC>("glBindBuffer");
  glBindVertexArray = load_proc<PFNGLBINDVERTEXARRAYPROC>("glBindVertexArray");
  glBufferData = load_proc<PFNGLBUFFERDATAPROC>("glBufferData");
  glCompileShader = load_proc<PFNGLCOMPILESHADERPROC>("glCompileShader");
  glCreateProgram = load_proc<PFNGLCREATEPROGRAMPROC>("glCreateProgram");
  glCreateShader = load_proc<PFNGLCREATESHADERPROC>("glCreateShader");
  glDeleteBuffers = load_proc<PFNGLDELETEBUFFERSPROC>("glDeleteBuffers");
  glDeleteProgram = load_proc<PFNGLDELETEPROGRAMPROC>("glDeleteProgram");
  glDeleteShader = load_proc<PFNGLDELETESHADERPROC>("glDeleteShader");
  glDeleteVertexArrays = load_proc<PFNGLDELETEVERTEXARRAYSPROC>("glDeleteVertexArrays");
  glDisableVertexAttribArray = load_proc<PFNGLDISABLEVERTEXATTRIBARRAYPROC>("glDisableVertexAttribArray");
  glEnableVertexAttribArray = load_proc<PFNGLENABLEVERTEXATTRIBARRAYPROC>("glEnableVertexAttribArray");
  glGenBuffers = load_proc<PFNGLGENBUFFERSPROC>("glGenBuffers");
  glGenVertexArrays = load_proc<PFNGLGENVERTEXARRAYSPROC>("glGenVertexArrays");
  glGetAttribLocation = load_proc<PFNGLGETATTRIBLOCATIONPROC>("glGetAttribLocation");
  glGetProgramInfoLog = load_proc<PFNGLGETPROGRAMINFOLOGPROC>("glGetProgramInfoLog");
  glGetProgramiv = load_proc<PFNGLGETPROGRAMIVPROC>("glGetProgramiv");
  glGetShaderInfoLog = load_proc<PFNGLGETSHADERINFOLOGPROC>("glGetShaderInfoLog");
  glGetShaderiv = load_proc<PFNGLGETSHADERIVPROC>("glGetShaderiv");
  glGetUniformLocation = load_proc<PFNGLGETUNIFORMLOCATIONPROC>("glGetUniformLocation");
  glLinkProgram = load_proc<PFNGLLINKPROGRAMPROC>("glLinkProgram");
  glShaderSource = load_proc<PFNGLSHADERSOURCEPROC>("glShaderSource");
  glUniform1i = load_proc<PFNGLUNIFORM1IPROC>("glUniform1i");
  glUniform4fv = load_proc<PFNGLUNIFORM4FVPROC>("glUniform4fv");
  glUniformMatrix4fv = load_proc<PFNGLUNIFORMMATRIX4FVPROC>("glUniformMatrix4fv");
  glUseProgram = load_proc<PFNGLUSEPROGRAMPROC>("glUseProgram");
  glVertexAttribPointer = load_proc<PFNGLVERTEXATTRIBPOINTERPROC>("glVertexAttribPointer");
}

} // namespace pingus

#endif

/* EOF */
