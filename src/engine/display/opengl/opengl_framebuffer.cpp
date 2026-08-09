//  Pingus - A free Lemmings clone
//  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmail.com>
//
//  Shader-based path shared by OpenGL 3.3 Core and OpenGL ES 2.0.

#include "engine/display/opengl/opengl_framebuffer.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <memory>
#include <sstream>
#include <iostream>

#include <logmich/log.hpp>

#include "engine/display/opengl/opengl_framebuffer_surface_impl.hpp"
#include "math/color.hpp"
#include "util/pathname.hpp"
#include "util/raise_exception.hpp"

namespace pingus {

namespace {

void mat4_ortho(float* out, float left, float right, float bottom, float top, float znear, float zfar)
{
  std::memset(out, 0, sizeof(float) * 16);
  out[0] = 2.0f / (right - left);
  out[5] = 2.0f / (top - bottom);
  out[10] = -2.0f / (zfar - znear);
  out[12] = -(right + left) / (right - left);
  out[13] = -(top + bottom) / (top - bottom);
  out[14] = -(zfar + znear) / (zfar - znear);
  out[15] = 1.0f;
}

} // namespace

OpenGLFramebuffer::OpenGLFramebuffer() :
  m_window(nullptr),
  m_glcontext(nullptr),
  cliprect_stack(),
  m_programs(),
  m_mvp{},
  m_width(0),
  m_height(0),
#if !PINGUS_GL_ES
  m_vao(0),
#endif
  m_vbo(0)
{
}

OpenGLFramebuffer::~OpenGLFramebuffer()
{
  if (m_vbo)
    glDeleteBuffers(1, &m_vbo);
#if !PINGUS_GL_ES
  if (m_vao)
    glDeleteVertexArrays(1, &m_vao);
#endif
  if (m_glcontext)
    SDL_GL_DeleteContext(m_glcontext);
  if (m_window)
    SDL_DestroyWindow(m_window);
}

void
OpenGLFramebuffer::set_ortho(int width, int height)
{
  m_width = width;
  m_height = height;
  // Top-left origin, y grows down (same as classic glOrtho(0,w,h,0,-1,1)).
  mat4_ortho(m_mvp, 0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);
  glViewport(0, 0, width, height);
}

void
OpenGLFramebuffer::draw_arrays(GLenum mode, float const* interleaved, int vertex_count, bool textured)
{
  // interleaved: textured = x,y,u,v per vertex; solid = x,y per vertex
  int stride = textured ? 4 : 2;
  GLsizeiptr nbytes = static_cast<GLsizeiptr>(vertex_count * stride * static_cast<int>(sizeof(float)));

#if !PINGUS_GL_ES
  glBindVertexArray(m_vao);
#endif
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, nbytes, interleaved, GL_STREAM_DRAW);

  if (textured)
  {
    m_programs.textured.use();
    glUniformMatrix4fv(m_programs.textured_u_mvp, 1, GL_FALSE, m_mvp);
    glUniform1i(m_programs.textured_u_tex, 0);
    float white[4] = {1.f, 1.f, 1.f, 1.f};
    glUniform4fv(m_programs.textured_u_color, 1, white);

    glEnableVertexAttribArray(static_cast<GLuint>(m_programs.textured_a_pos));
    glVertexAttribPointer(static_cast<GLuint>(m_programs.textured_a_pos), 2, GL_FLOAT, GL_FALSE,
                          stride * sizeof(float), nullptr);
    glEnableVertexAttribArray(static_cast<GLuint>(m_programs.textured_a_uv));
    glVertexAttribPointer(static_cast<GLuint>(m_programs.textured_a_uv), 2, GL_FLOAT, GL_FALSE,
                          stride * sizeof(float), reinterpret_cast<void*>(sizeof(float) * 2));
  }
  else
  {
    m_programs.solid.use();
    glUniformMatrix4fv(m_programs.solid_u_mvp, 1, GL_FALSE, m_mvp);

    glEnableVertexAttribArray(static_cast<GLuint>(m_programs.solid_a_pos));
    glVertexAttribPointer(static_cast<GLuint>(m_programs.solid_a_pos), 2, GL_FLOAT, GL_FALSE,
                          stride * sizeof(float), nullptr);
  }

  glDrawArrays(mode, 0, vertex_count);

  if (textured)
  {
    glDisableVertexAttribArray(static_cast<GLuint>(m_programs.textured_a_pos));
    glDisableVertexAttribArray(static_cast<GLuint>(m_programs.textured_a_uv));
  }
  else
  {
    glDisableVertexAttribArray(static_cast<GLuint>(m_programs.solid_a_pos));
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
#if !PINGUS_GL_ES
  glBindVertexArray(0);
#endif
  glUseProgram(0);
}

FramebufferSurface
OpenGLFramebuffer::create_surface(Surface const& surface)
{
  return FramebufferSurface(new OpenGLFramebufferSurfaceImpl(surface.get_surface()));
}

Surface
OpenGLFramebuffer::make_screenshot() const
{
  geom::isize size = get_size();
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  std::unique_ptr<uint8_t[]> buffer(new uint8_t[static_cast<size_t>(size.width() * size.height() * 4)]);
  glReadPixels(0, 0, size.width(), size.height(), GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());

  Surface screenshot(size.width(), size.height());
  uint8_t* op = screenshot.get_data();
  size_t pitch = static_cast<size_t>(screenshot.get_pitch());
  for (size_t y = 0; y < static_cast<size_t>(size.height()); ++y)
  {
    size_t src_y = static_cast<size_t>(size.height()) - 1 - y; // flip
    for (size_t x = 0; x < static_cast<size_t>(size.width()); ++x)
    {
      size_t si = (src_y * static_cast<size_t>(size.width()) + x) * 4;
      op[y * pitch + 4 * x + 0] = buffer[si + 0];
      op[y * pitch + 4 * x + 1] = buffer[si + 1];
      op[y * pitch + 4 * x + 2] = buffer[si + 2];
      op[y * pitch + 4 * x + 3] = buffer[si + 3];
    }
  }
  return screenshot;
}

void
OpenGLFramebuffer::set_video_mode(geom::isize const& size, bool fullscreen, bool resizable)
{
  if (m_window)
  {
    SDL_SetWindowSize(m_window, size.width(), size.height());
    int dw = size.width();
    int dh = size.height();
    SDL_GL_GetDrawableSize(m_window, &dw, &dh);
    if (dw <= 0 || dh <= 0)
      SDL_GetWindowSize(m_window, &dw, &dh);
    set_ortho(dw, dh);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    return;
  }

  Uint32 flags = SDL_WINDOW_OPENGL;
#ifdef ANDROID
  // Single surface; windowed/resizable modes do not match the Java surface
  // size and produce a shifted GLES viewport.
  flags |= SDL_WINDOW_FULLSCREEN;
  (void)fullscreen;
  (void)resizable;
#else
  if (fullscreen)
    flags |= SDL_WINDOW_FULLSCREEN;
  else if (resizable)
    flags |= SDL_WINDOW_RESIZABLE;
#endif

  std::cerr << "OpenGLFramebuffer: SDL_VIDEODRIVER="
            << (std::getenv("SDL_VIDEODRIVER") ? std::getenv("SDL_VIDEODRIVER") : "(unset)")
            << " requested=" << size.width() << "x" << size.height()
            << " flags=0x" << std::hex << flags << std::dec
            << " ES=" << PINGUS_GL_ES << std::endl;

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

#if PINGUS_GL_ES
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
  std::cerr << "OpenGLFramebuffer: requesting GLES 2.0 context" << std::endl;
#else
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  std::cerr << "OpenGLFramebuffer: requesting OpenGL 3.3 core context" << std::endl;
#endif

  std::cerr << "OpenGLFramebuffer: SDL_CreateWindow..." << std::endl;
  m_window = SDL_CreateWindow("Pingus " PROJECT_VERSION,
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              size.width(), size.height(),
                              flags);
  if (!m_window)
  {
    std::cerr << "OpenGLFramebuffer: SDL_CreateWindow FAILED: " << SDL_GetError() << std::endl;
    raise_error("Couldn't set video mode (" << size.width() << "x" << size.height() << "): " << SDL_GetError());
  }
  std::cerr << "OpenGLFramebuffer: window ok id=" << SDL_GetWindowID(m_window) << std::endl;

  {
    SDL_Surface* icon = IMG_Load(Pathname("images/icons/pingus.png", Pathname::DATA_PATH).get_sys_path().c_str());
    if (icon)
    {
      SDL_SetWindowIcon(m_window, icon);
      SDL_FreeSurface(icon);
    }
  }

  std::cerr << "OpenGLFramebuffer: SDL_GL_CreateContext..." << std::endl;
  m_glcontext = SDL_GL_CreateContext(m_window);
  if (!m_glcontext)
  {
    std::cerr << "OpenGLFramebuffer: SDL_GL_CreateContext FAILED: " << SDL_GetError() << std::endl;
    raise_error("couldn't create GL context: " << SDL_GetError());
  }
  std::cerr << "OpenGLFramebuffer: context ok" << std::endl;

#if defined(_WIN32) && !PINGUS_GL_ES
  opengl_load_procs();
#endif

  std::cerr << "OpenGLFramebuffer: buffers/programs init..." << std::endl;
#if !PINGUS_GL_ES
  glGenVertexArrays(1, &m_vao);
#endif
  glGenBuffers(1, &m_vbo);

  m_programs.init();
  std::cerr << "OpenGLFramebuffer: programs ready" << std::endl;

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // On Android (and high-DPI), the drawable size can differ from the
  // requested window size. Ortho/viewport must match the drawable or the
  // UI is vertically shifted/clipped (e.g. 1024x768 request vs 1024x528 surface).
  int dw = size.width();
  int dh = size.height();
  SDL_GL_GetDrawableSize(m_window, &dw, &dh);
  if (dw <= 0 || dh <= 0)
    SDL_GetWindowSize(m_window, &dw, &dh);
  set_ortho(dw, dh);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  {
    char const* vendor = reinterpret_cast<char const*>(glGetString(GL_VENDOR));
    char const* renderer = reinterpret_cast<char const*>(glGetString(GL_RENDERER));
    char const* version = reinterpret_cast<char const*>(glGetString(GL_VERSION));
    log_info("OpenGL framebuffer ready (ES={} requested={}x{} drawable={}x{})",
             PINGUS_GL_ES, size.width(), size.height(), dw, dh);
    log_info("OpenGL: vendor='{}' renderer='{}' version='{}'",
             vendor ? vendor : "?",
             renderer ? renderer : "?",
             version ? version : "?");
    std::cerr << "OpenGLFramebuffer: ready ES=" << PINGUS_GL_ES
              << " requested=" << size.width() << "x" << size.height()
              << " drawable=" << dw << "x" << dh
              << " vendor='" << (vendor ? vendor : "?") << "'"
              << " renderer='" << (renderer ? renderer : "?") << "'"
              << " version='" << (version ? version : "?") << "'"
              << std::endl;
  }
}

bool
OpenGLFramebuffer::is_fullscreen() const
{
  return SDL_GetWindowFlags(m_window) & SDL_WINDOW_FULLSCREEN;
}

bool
OpenGLFramebuffer::is_resizable() const
{
  return SDL_GetWindowFlags(m_window) & SDL_WINDOW_RESIZABLE;
}

void
OpenGLFramebuffer::flip()
{
  SDL_GL_SwapWindow(m_window);
}

void
OpenGLFramebuffer::push_cliprect(geom::irect const& rect)
{
  if (cliprect_stack.empty())
    glEnable(GL_SCISSOR_TEST);

  if (cliprect_stack.empty())
  {
    cliprect_stack.push_back(rect);
  }
  else
  {
    cliprect_stack.push_back(geom::irect(std::max(cliprect_stack.back().left(),   rect.left()),
                                  std::max(cliprect_stack.back().top(),    rect.top()),
                                  std::min(cliprect_stack.back().right(),  rect.right()),
                                  std::min(cliprect_stack.back().bottom(), rect.bottom())));
  }

  glScissor(cliprect_stack.back().left(),
            get_size().height() - cliprect_stack.back().bottom(),
            cliprect_stack.back().width(),
            cliprect_stack.back().height());
}

void
OpenGLFramebuffer::pop_cliprect()
{
  cliprect_stack.pop_back();
  if (cliprect_stack.empty())
  {
    glDisable(GL_SCISSOR_TEST);
  }
  else
  {
    geom::irect const& rect = cliprect_stack.back();
    glScissor(rect.left(),
              get_size().height() - rect.bottom(),
              rect.width(),
              rect.height());
  }
}

void
OpenGLFramebuffer::draw_surface(FramebufferSurface const& src, geom::ipoint const& pos)
{
  draw_surface(src, geom::irect(geom::ipoint(0, 0), src.get_size()), pos);
}

void
OpenGLFramebuffer::draw_surface(FramebufferSurface const& src, geom::irect const& srcrect, geom::ipoint const& pos)
{
  OpenGLFramebufferSurfaceImpl const* texture = static_cast<OpenGLFramebufferSurfaceImpl*>(src.get_impl());
  float tw = static_cast<float>(texture->get_texture_size().width());
  float th = static_cast<float>(texture->get_texture_size().height());
  float u0 = static_cast<float>(srcrect.left()) / tw;
  float v0 = static_cast<float>(srcrect.top()) / th;
  float u1 = static_cast<float>(srcrect.right()) / tw;
  float v1 = static_cast<float>(srcrect.bottom()) / th;
  float x0 = static_cast<float>(pos.x());
  float y0 = static_cast<float>(pos.y());
  float x1 = static_cast<float>(pos.x() + srcrect.width());
  float y1 = static_cast<float>(pos.y() + srcrect.height());

  float verts[] = {
    x0, y0, u0, v0,
    x1, y0, u1, v0,
    x1, y1, u1, v1,
    x0, y1, u0, v1,
  };

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture->get_handle());
  draw_arrays(GL_TRIANGLE_FAN, verts, 4, true);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void
OpenGLFramebuffer::draw_line(geom::ipoint const& pos1, geom::ipoint const& pos2, Color const& color)
{
  float verts[] = {
    static_cast<float>(pos1.x()), static_cast<float>(pos1.y()),
    static_cast<float>(pos2.x()), static_cast<float>(pos2.y()),
  };
  m_programs.solid.use();
  glUniformMatrix4fv(m_programs.solid_u_mvp, 1, GL_FALSE, m_mvp);
  float c[4] = {color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f};
  glUniform4fv(m_programs.solid_u_color, 1, c);

#if !PINGUS_GL_ES
  glBindVertexArray(m_vao);
#endif
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STREAM_DRAW);
  glEnableVertexAttribArray(static_cast<GLuint>(m_programs.solid_a_pos));
  glVertexAttribPointer(static_cast<GLuint>(m_programs.solid_a_pos), 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glDrawArrays(GL_LINES, 0, 2);
  glDisableVertexAttribArray(static_cast<GLuint>(m_programs.solid_a_pos));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
#if !PINGUS_GL_ES
  glBindVertexArray(0);
#endif
  glUseProgram(0);
}

void
OpenGLFramebuffer::draw_rect(geom::irect const& rect, Color const& color)
{
  float verts[] = {
    static_cast<float>(rect.left()),  static_cast<float>(rect.top()),
    static_cast<float>(rect.right()), static_cast<float>(rect.top()),
    static_cast<float>(rect.right()), static_cast<float>(rect.bottom()),
    static_cast<float>(rect.left()),  static_cast<float>(rect.bottom()),
  };
  m_programs.solid.use();
  glUniformMatrix4fv(m_programs.solid_u_mvp, 1, GL_FALSE, m_mvp);
  float c[4] = {color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f};
  glUniform4fv(m_programs.solid_u_color, 1, c);

#if !PINGUS_GL_ES
  glBindVertexArray(m_vao);
#endif
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STREAM_DRAW);
  glEnableVertexAttribArray(static_cast<GLuint>(m_programs.solid_a_pos));
  glVertexAttribPointer(static_cast<GLuint>(m_programs.solid_a_pos), 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glDrawArrays(GL_LINE_LOOP, 0, 4);
  glDisableVertexAttribArray(static_cast<GLuint>(m_programs.solid_a_pos));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
#if !PINGUS_GL_ES
  glBindVertexArray(0);
#endif
  glUseProgram(0);
}

void
OpenGLFramebuffer::fill_rect(geom::irect const& rect, Color const& color)
{
  float x0 = static_cast<float>(rect.left());
  float y0 = static_cast<float>(rect.top());
  float x1 = static_cast<float>(rect.right());
  float y1 = static_cast<float>(rect.bottom());
  float verts[] = { x0, y0, x1, y0, x1, y1, x0, y1 };

  m_programs.solid.use();
  glUniformMatrix4fv(m_programs.solid_u_mvp, 1, GL_FALSE, m_mvp);
  float c[4] = {color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f};
  glUniform4fv(m_programs.solid_u_color, 1, c);

#if !PINGUS_GL_ES
  glBindVertexArray(m_vao);
#endif
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STREAM_DRAW);
  glEnableVertexAttribArray(static_cast<GLuint>(m_programs.solid_a_pos));
  glVertexAttribPointer(static_cast<GLuint>(m_programs.solid_a_pos), 2, GL_FLOAT, GL_FALSE, 0, nullptr);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glDisableVertexAttribArray(static_cast<GLuint>(m_programs.solid_a_pos));
  glBindBuffer(GL_ARRAY_BUFFER, 0);
#if !PINGUS_GL_ES
  glBindVertexArray(0);
#endif
  glUseProgram(0);
}

geom::isize
OpenGLFramebuffer::get_size() const
{
  int w = 0;
  int h = 0;
  // Prefer drawable size so layout matches the GL viewport/ortho.
  SDL_GL_GetDrawableSize(m_window, &w, &h);
  if (w <= 0 || h <= 0)
    SDL_GetWindowSize(m_window, &w, &h);
  return geom::isize(w, h);
}

} // namespace pingus

/* EOF */
