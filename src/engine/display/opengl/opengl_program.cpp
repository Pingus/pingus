// Pingus - A free Lemmings clone
// Copyright (C) 2026 Ingo Ruhnke <grumbel@gmail.com>

#include "engine/display/opengl/opengl_program.hpp"

#include <stdexcept>
#include <vector>

#include <logmich/log.hpp>

#include "util/raise_exception.hpp"

namespace pingus {

namespace {

GLuint compile_shader(GLenum type, char const* source)
{
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  GLint ok = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
  if (!ok)
  {
    GLint len = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    std::vector<char> log(static_cast<size_t>(len > 0 ? len : 1));
    glGetShaderInfoLog(shader, static_cast<GLsizei>(log.size()), nullptr, log.data());
    glDeleteShader(shader);
    raise_exception(std::runtime_error, "Shader compile failed: " << log.data());
  }
  return shader;
}

#if PINGUS_GL_ES
// OpenGL ES 2.0 / WebGL 1
char const* TEXTURED_VERT = R"(#version 100
attribute vec2 a_pos;
attribute vec2 a_uv;
uniform mat4 u_mvp;
varying vec2 v_uv;
void main() {
  v_uv = a_uv;
  gl_Position = u_mvp * vec4(a_pos, 0.0, 1.0);
}
)";

char const* TEXTURED_FRAG = R"(#version 100
precision mediump float;
uniform sampler2D u_tex;
uniform vec4 u_color;
varying vec2 v_uv;
void main() {
  gl_FragColor = texture2D(u_tex, v_uv) * u_color;
}
)";

char const* SOLID_VERT = R"(#version 100
attribute vec2 a_pos;
uniform mat4 u_mvp;
void main() {
  gl_Position = u_mvp * vec4(a_pos, 0.0, 1.0);
}
)";

char const* SOLID_FRAG = R"(#version 100
precision mediump float;
uniform vec4 u_color;
void main() {
  gl_FragColor = u_color;
}
)";
#else
// OpenGL 3.3 Core Profile
char const* TEXTURED_VERT = R"(#version 330 core
layout(location = 0) in vec2 a_pos;
layout(location = 1) in vec2 a_uv;
uniform mat4 u_mvp;
out vec2 v_uv;
void main() {
  v_uv = a_uv;
  gl_Position = u_mvp * vec4(a_pos, 0.0, 1.0);
}
)";

char const* TEXTURED_FRAG = R"(#version 330 core
uniform sampler2D u_tex;
uniform vec4 u_color;
in vec2 v_uv;
out vec4 frag_color;
void main() {
  frag_color = texture(u_tex, v_uv) * u_color;
}
)";

char const* SOLID_VERT = R"(#version 330 core
layout(location = 0) in vec2 a_pos;
uniform mat4 u_mvp;
void main() {
  gl_Position = u_mvp * vec4(a_pos, 0.0, 1.0);
}
)";

char const* SOLID_FRAG = R"(#version 330 core
uniform vec4 u_color;
out vec4 frag_color;
void main() {
  frag_color = u_color;
}
)";
#endif

} // namespace

OpenGLProgram::OpenGLProgram() :
  m_id(0)
{
}

OpenGLProgram::~OpenGLProgram()
{
  if (m_id)
    glDeleteProgram(m_id);
}

void
OpenGLProgram::build(char const* vertex_src, char const* fragment_src)
{
  GLuint vs = compile_shader(GL_VERTEX_SHADER, vertex_src);
  GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fragment_src);
  m_id = glCreateProgram();
  glAttachShader(m_id, vs);
  glAttachShader(m_id, fs);
  glLinkProgram(m_id);
  glDeleteShader(vs);
  glDeleteShader(fs);

  GLint ok = 0;
  glGetProgramiv(m_id, GL_LINK_STATUS, &ok);
  if (!ok)
  {
    GLint len = 0;
    glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &len);
    std::vector<char> log(static_cast<size_t>(len > 0 ? len : 1));
    glGetProgramInfoLog(m_id, static_cast<GLsizei>(log.size()), nullptr, log.data());
    raise_exception(std::runtime_error, "Program link failed: " << log.data());
  }
}

void
OpenGLProgram::use() const
{
  glUseProgram(m_id);
}

GLint
OpenGLProgram::uniform(char const* name) const
{
  return glGetUniformLocation(m_id, name);
}

GLint
OpenGLProgram::attrib(char const* name) const
{
  return glGetAttribLocation(m_id, name);
}

void
OpenGLPrograms::init()
{
  textured.build(TEXTURED_VERT, TEXTURED_FRAG);
  solid.build(SOLID_VERT, SOLID_FRAG);

  textured_u_mvp = textured.uniform("u_mvp");
  textured_u_tex = textured.uniform("u_tex");
  textured_u_color = textured.uniform("u_color");
  textured_a_pos = textured.attrib("a_pos");
  textured_a_uv = textured.attrib("a_uv");

  solid_u_mvp = solid.uniform("u_mvp");
  solid_u_color = solid.uniform("u_color");
  solid_a_pos = solid.attrib("a_pos");

  log_info("OpenGL programs ready (ES={})", PINGUS_GL_ES);
}

} // namespace pingus

/* EOF */
