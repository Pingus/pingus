//  Pingus - A free Lemmings clone
//  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "engine/display/opengl/opengl_framebuffer.hpp"

#ifdef _WIN32
#  define NOGDI
#endif

#include <SDL.h>
#include <SDL_image.h>
#include <sstream>
#include <stdexcept>

#include <logmich/log.hpp>

#include "engine/display/opengl/opengl_framebuffer_surface_impl.hpp"
#include "util/raise_exception.hpp"

namespace pingus {

OpenGLFramebuffer::OpenGLFramebuffer() :
  m_window(),
  m_glcontext(),
  cliprect_stack()
{
}

OpenGLFramebuffer::~OpenGLFramebuffer()
{
  SDL_GL_DeleteContext(m_glcontext);
  SDL_DestroyWindow(m_window);
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
  glReadPixels(0, 0, size.width(), size.height(), GL_RGB, GL_UNSIGNED_BYTE, buffer.get());

  Surface screenshot(size.width(), size.height());
  uint8_t* op = screenshot.get_data();
  size_t pitch = static_cast<size_t>(screenshot.get_pitch());
  for(size_t y = 0; y < static_cast<size_t>(size.height()); ++y)
  {
    for(size_t x = 0; x < static_cast<size_t>(size.height()); ++x)
    {
      op[y * pitch + 4*x + 0] = buffer[4 * static_cast<size_t>(size.width()) + 3*x + 0];
      op[y * pitch + 4*x + 1] = buffer[4 * static_cast<size_t>(size.width()) + 3*x + 1];
      op[y * pitch + 4*x + 2] = buffer[4 * static_cast<size_t>(size.width()) + 3*x + 2];
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

    log_error("video mode switching not implemented: {}x{}", size.width(), size.height());
    glViewport(0, 0, size.width(), size.height());

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, size.width(), size.height(), 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(1.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
  else
  {
    Uint32 flags = SDL_WINDOW_OPENGL;

    if (fullscreen)
    {
      flags |= SDL_WINDOW_FULLSCREEN;
    }
    else if (resizable)
    {
      flags |= SDL_WINDOW_RESIZABLE;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,     8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,   8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,    8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    if ((false)) // anti-aliasing
    {
      SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1); // boolean value, either it's enabled or not
      SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2); // 0, 2, or 4 for number of samples
    }

    m_window = SDL_CreateWindow("Pingus " PROJECT_VERSION,
                                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                size.width(), size.height(),
                                flags);
    if (!m_window)
    {
      raise_error("Couldn't set video mode (" << size.width() << "x" << size.height() << "): " << SDL_GetError());
    }
    SDL_SetWindowIcon(m_window, IMG_Load(Pathname("images/icons/pingus.png", Pathname::DATA_PATH).get_sys_path().c_str()));

    m_glcontext = SDL_GL_CreateContext(m_window);
    if (!m_glcontext)
    {
      raise_error("couldn't create GL context: " << SDL_GetError());
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    // setup opengl state and transform
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, size.width(), size.height());

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, size.width(), size.height(), 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
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
    glScissor(rect.left(), rect.top(),
              rect.width(), rect.height());
  }
}

void
OpenGLFramebuffer::draw_surface(FramebufferSurface const& src, geom::ipoint const& pos)
{
  draw_surface(src, geom::irect(geom::ipoint(0, 0), src.get_size()),  pos);
}

void
OpenGLFramebuffer::draw_surface(FramebufferSurface const& src, geom::irect const& srcrect, geom::ipoint const& pos)
{
  OpenGLFramebufferSurfaceImpl const* texture = static_cast<OpenGLFramebufferSurfaceImpl*>(src.get_impl());

  glBindTexture(GL_TEXTURE_2D, texture->get_handle());

  int vertices[] = {
    pos.x(),                        pos.y(),
    pos.x() + srcrect.width(),      pos.y(),
    pos.x() + srcrect.width(),      pos.y() + srcrect.height(),
    pos.x(),                        pos.y() + srcrect.height(),
  };
  glVertexPointer(2, GL_INT, 0, vertices);

  float uvs[] = {
    static_cast<float>(srcrect.left())   / static_cast<float>(texture->get_texture_size().width()),
    static_cast<float>(srcrect.top())    / static_cast<float>(texture->get_texture_size().height()),

    static_cast<float>(srcrect.right())  / static_cast<float>(texture->get_texture_size().width()),
    static_cast<float>(srcrect.top())    / static_cast<float>(texture->get_texture_size().height()),

    static_cast<float>(srcrect.right())  / static_cast<float>(texture->get_texture_size().width()),
    static_cast<float>(srcrect.bottom()) / static_cast<float>(texture->get_texture_size().height()),

    static_cast<float>(srcrect.left())   / static_cast<float>(texture->get_texture_size().width()),
    static_cast<float>(srcrect.bottom()) / static_cast<float>(texture->get_texture_size().height())
  };

  glTexCoordPointer(2, GL_FLOAT, 0, uvs);

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void
OpenGLFramebuffer::draw_line(geom::ipoint const& pos1, geom::ipoint const& pos2, Color const& color)
{
  glDisable(GL_TEXTURE_2D);
  glColor4ub(color.r, color.g, color.b, color.a);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  int vertices[] = {
    pos1.x(), pos1.y(),
    pos2.x(), pos2.y(),
  };
  glVertexPointer(2, GL_INT, 0, vertices);

  glDrawArrays(GL_LINES, 0, 2);

  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnable(GL_TEXTURE_2D);
  glColor4f(1, 1, 1, 1);
}

void
OpenGLFramebuffer::draw_rect(geom::irect const& rect, Color const& color)
{
  glDisable(GL_TEXTURE_2D);
  glColor4ub(color.r, color.g, color.b, color.a);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  int vertices[] = {
    rect.left(),  rect.top(),
    rect.right(), rect.top(),
    rect.right(), rect.bottom(),
    rect.left(),  rect.bottom(),
  };
  glVertexPointer(2, GL_INT, 0, vertices);

  glDrawArrays(GL_LINE_LOOP, 0, 4);

  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnable(GL_TEXTURE_2D);
  glColor4f(1, 1, 1, 1);
}

void
OpenGLFramebuffer::fill_rect(geom::irect const& rect, Color const& color)
{
  glDisable(GL_TEXTURE_2D);
  glColor4ub(color.r, color.g, color.b, color.a);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  int vertices[] = {
    rect.left(),  rect.top(),
    rect.right(), rect.top(),
    rect.right(), rect.bottom(),
    rect.left(),  rect.bottom(),
  };
  glVertexPointer(2, GL_INT, 0, vertices);

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnable(GL_TEXTURE_2D);
  glColor4f(1, 1, 1, 1);
}

geom::isize
OpenGLFramebuffer::get_size() const
{
  int w = 0;
  int h = 0;
  SDL_GetWindowSize(m_window, &w, &h);
  return geom::isize(w, h);
}

} // namespace pingus

/* EOF */
