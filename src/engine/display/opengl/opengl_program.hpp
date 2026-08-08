// Pingus - A free Lemmings clone
// Copyright (C) 2026 Ingo Ruhnke <grumbel@gmail.com>

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_OPENGL_OPENGL_PROGRAM_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_OPENGL_OPENGL_PROGRAM_HPP

#include "engine/display/opengl/opengl_compat.hpp"

#include <string>

namespace pingus {

/** Minimal GLSL program shared by GL 3.3 core and GLES2 (via #version). */
class OpenGLProgram
{
public:
  OpenGLProgram();
  ~OpenGLProgram();

  OpenGLProgram(OpenGLProgram const&) = delete;
  OpenGLProgram& operator=(OpenGLProgram const&) = delete;

  void build(char const* vertex_src, char const* fragment_src);
  void use() const;
  GLint uniform(char const* name) const;
  GLint attrib(char const* name) const;
  GLuint id() const { return m_id; }

private:
  GLuint m_id;
};

/** Built-in programs used by OpenGLFramebuffer. */
struct OpenGLPrograms
{
  OpenGLProgram textured;
  OpenGLProgram solid;
  GLint textured_u_mvp = -1;
  GLint textured_u_tex = -1;
  GLint textured_u_color = -1;
  GLint textured_a_pos = -1;
  GLint textured_a_uv = -1;
  GLint solid_u_mvp = -1;
  GLint solid_u_color = -1;
  GLint solid_a_pos = -1;

  void init();
};

} // namespace pingus

#endif

/* EOF */
