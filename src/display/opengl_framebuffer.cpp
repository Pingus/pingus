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

#include <sstream>
#include <stdexcept>
#include "surface.hpp"
#include "opengl_framebuffer_surface_impl.hpp"
#include "opengl_framebuffer.hpp"

OpenGLFramebuffer::OpenGLFramebuffer()
{
}

FramebufferSurface
OpenGLFramebuffer::create_surface(const Surface& surface)
{
  return FramebufferSurface(new OpenGLFramebufferSurfaceImpl(surface.get_surface()));
}

void
OpenGLFramebuffer::set_video_mode(const Size& size, bool fullscreen)
{
  int flags = SDL_OPENGL;
  
  if (fullscreen)
    {
      flags |= SDL_FULLSCREEN;
    }
  else
    {
      flags |= SDL_RESIZABLE;
    }

  int bpp = 0; // auto-detect 
  screen = SDL_SetVideoMode(size.width, size.height, bpp, flags);

  if(screen == 0) 
    {
      std::ostringstream msg;
      msg << "Couldn't set video mode (" << size.width << "x" << size.height
          << "-" << bpp << "bpp): " << SDL_GetError();
      throw std::runtime_error(msg.str());
    }

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

void
OpenGLFramebuffer::flip()
{
  SDL_GL_SwapBuffers();
}
  
void
OpenGLFramebuffer::push_cliprect(const Rect& rect)
{
  cliprect_stack.push_back(rect);
}

void
OpenGLFramebuffer::pop_cliprect()
{
  cliprect_stack.pop_back();
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
  
  float vertices[] = {
    pos.x,                     pos.y,
    pos.x+srcrect.get_width(), pos.y,
    pos.x+srcrect.get_width(), pos.y+srcrect.get_height(),
    pos.x,                     pos.y+srcrect.get_height(),
  };
  glVertexPointer(2, GL_FLOAT, 0, vertices);
  
  float uvs[] = {
    float(srcrect.left)/texture->get_texture_size().width,  float(srcrect.top)/texture->get_texture_size().height,
    float(srcrect.right)/texture->get_texture_size().width, float(srcrect.top)/texture->get_texture_size().height,
    float(srcrect.right)/texture->get_texture_size().width, float(srcrect.bottom)/texture->get_texture_size().height,
    float(srcrect.left)/texture->get_texture_size().width,  float(srcrect.bottom)/texture->get_texture_size().height
  };
  glTexCoordPointer(2, GL_FLOAT, 0, uvs);

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void
OpenGLFramebuffer::draw_line(const Vector2i& pos1, const Vector2i& pos2, const Color& color)
{
}

void
OpenGLFramebuffer::draw_rect(const Rect& rect, const Color& color)
{
}

void
OpenGLFramebuffer::fill_rect(const Rect& rect, const Color& color)
{
  glDisable(GL_TEXTURE_2D);
  glColor4i(color.r, color.g, color.b, color.a);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  float vertices[] = {
    rect.left,                  rect.top,
    rect.left+rect.get_width(), rect.top,
    rect.left+rect.get_width(), rect.top+rect.get_height(),
    rect.left,                  rect.top+rect.get_height()
  };
  glVertexPointer(2, GL_FLOAT, 0, vertices);

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnable(GL_TEXTURE_2D);
  glColor4f(1, 1, 1, 1);
}

Size
OpenGLFramebuffer::get_size() const
{
  return Size(screen->w, screen->h);
}

/* EOF */
