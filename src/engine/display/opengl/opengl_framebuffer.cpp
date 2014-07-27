//  Pingus - A free Lemmings clone
//  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmx.de>
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

#include <SDL.h>
#include <SDL_image.h>
#include <sstream>
#include <stdexcept>

#include "engine/display/opengl/opengl_framebuffer_surface_impl.hpp"

OpenGLFramebuffer::OpenGLFramebuffer() :
  m_window(),
  cliprect_stack()  
{
}

OpenGLFramebuffer::~OpenGLFramebuffer()
{
  SDL_DestroyWindow(m_window);
}

FramebufferSurface
OpenGLFramebuffer::create_surface(const Surface& surface)
{
  return FramebufferSurface(new OpenGLFramebufferSurfaceImpl(surface.get_surface()));
}

void
OpenGLFramebuffer::set_video_mode(const Size& size, bool fullscreen, bool resizable)
{
  int flags = SDL_WINDOW_OPENGL;
  
  if (fullscreen)
  {
    flags |= SDL_WINDOW_FULLSCREEN;
  }
  else if (resizable)
  {
    flags |= SDL_WINDOW_RESIZABLE;
  }

  m_window = SDL_CreateWindow("Pingus " VERSION,
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              size.width, size.height,
                              flags);
  if(m_window == 0)
  {
    std::ostringstream msg;
    msg << "Couldn't set video mode (" << size.width << "x" << size.height << "): " << SDL_GetError();
    throw std::runtime_error(msg.str());
  }
  SDL_SetWindowIcon(m_window, IMG_Load(Pathname("images/icons/pingus.png", Pathname::DATA_PATH).get_sys_path().c_str()));

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  // setup opengl state and transform
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glViewport(0, 0, size.width, size.height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(0, size.width, size.height, 0, -1, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
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
OpenGLFramebuffer::push_cliprect(const Rect& rect)
{
  if (cliprect_stack.empty())
    glEnable(GL_SCISSOR_TEST);

  if (cliprect_stack.empty())
  {
    cliprect_stack.push_back(rect);
  }
  else
  {
    cliprect_stack.push_back(Rect(Math::max(cliprect_stack.back().left,   rect.left), 
                                  Math::max(cliprect_stack.back().top,    rect.top), 
                                  Math::min(cliprect_stack.back().right,  rect.right), 
                                  Math::min(cliprect_stack.back().bottom, rect.bottom)));
  }

  glScissor(cliprect_stack.back().left,
            get_size().height - cliprect_stack.back().bottom, 
            cliprect_stack.back().get_width(), 
            cliprect_stack.back().get_height());
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
    const Rect& rect = cliprect_stack.back();
    glScissor(rect.left,        rect.top, 
              rect.get_width(), rect.get_height());
  }
}

void
OpenGLFramebuffer::draw_surface(const FramebufferSurface& src, const Vector2i& pos)
{
  draw_surface(src, Rect(Vector2i(0, 0), src.get_size()),  pos);
}

void
OpenGLFramebuffer::draw_surface(const FramebufferSurface& src, const Rect& srcrect, const Vector2i& pos)
{
  const OpenGLFramebufferSurfaceImpl* texture = static_cast<OpenGLFramebufferSurfaceImpl*>(src.get_impl());
  
  glBindTexture(GL_TEXTURE_2D, texture->get_handle());
  
  int vertices[] = {
    pos.x,                     pos.y,
    pos.x+srcrect.get_width(), pos.y,
    pos.x+srcrect.get_width(), pos.y+srcrect.get_height(),
    pos.x,                     pos.y+srcrect.get_height(),
  };
  glVertexPointer(2, GL_INT, 0, vertices);
  
  float uvs[] = {
    static_cast<float>(srcrect.left)   / static_cast<float>(texture->get_texture_size().width), 
    static_cast<float>(srcrect.top)    / static_cast<float>(texture->get_texture_size().height),

    static_cast<float>(srcrect.right)  / static_cast<float>(texture->get_texture_size().width), 
    static_cast<float>(srcrect.top)    / static_cast<float>(texture->get_texture_size().height),

    static_cast<float>(srcrect.right)  / static_cast<float>(texture->get_texture_size().width), 
    static_cast<float>(srcrect.bottom) / static_cast<float>(texture->get_texture_size().height),

    static_cast<float>(srcrect.left)   / static_cast<float>(texture->get_texture_size().width), 
    static_cast<float>(srcrect.bottom) / static_cast<float>(texture->get_texture_size().height)
  };

  glTexCoordPointer(2, GL_FLOAT, 0, uvs);

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void
OpenGLFramebuffer::draw_line(const Vector2i& pos1, const Vector2i& pos2, const Color& color)
{
  glDisable(GL_TEXTURE_2D);
  glColor4ub(color.r, color.g, color.b, color.a);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  int vertices[] = {
    pos1.x, pos1.y,
    pos2.x, pos2.y,
  };
  glVertexPointer(2, GL_INT, 0, vertices);

  glDrawArrays(GL_LINES, 0, 2);

  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnable(GL_TEXTURE_2D);
  glColor4f(1, 1, 1, 1);
}

void
OpenGLFramebuffer::draw_rect(const Rect& rect, const Color& color)
{
  glDisable(GL_TEXTURE_2D);
  glColor4ub(color.r, color.g, color.b, color.a);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  int vertices[] = {
    rect.left,  rect.top,
    rect.right, rect.top,
    rect.right, rect.bottom,
    rect.left,  rect.bottom,
  };
  glVertexPointer(2, GL_INT, 0, vertices);

  glDrawArrays(GL_LINE_LOOP, 0, 4);

  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnable(GL_TEXTURE_2D);
  glColor4f(1, 1, 1, 1);
}

void
OpenGLFramebuffer::fill_rect(const Rect& rect, const Color& color)
{
  glDisable(GL_TEXTURE_2D);
  glColor4ub(color.r, color.g, color.b, color.a);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  int vertices[] = {
    rect.left,  rect.top,
    rect.right, rect.top,
    rect.right, rect.bottom,
    rect.left,  rect.bottom,
  };
  glVertexPointer(2, GL_INT, 0, vertices);

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnable(GL_TEXTURE_2D);
  glColor4f(1, 1, 1, 1);
}

Size
OpenGLFramebuffer::get_size() const
{
  Size s;
  SDL_GetWindowSize(m_window, &s.width, &s.height);
  return s;
}

/* EOF */
