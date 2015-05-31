// Pingus - A free Lemmings clone
// Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "engine/display/scene_context.hpp"

#include "engine/display/framebuffer.hpp"

#define SCALE_FACTOR 8.0f

class SceneContextImpl
{
public:
  DrawingContext color;
  DrawingContext light;
  DrawingContext highlight;

  Rect cliprect;
  bool use_cliprect;

  SceneContextImpl() :
    color(),
    light(),
    highlight(),
    cliprect(),
    use_cliprect(false)
  {
  }

  SceneContextImpl(const Rect& rect) :
    color(rect),
    light(rect),
    highlight(rect),
    cliprect(),
    use_cliprect(false)
  {
  }
};

SceneContext::SceneContext() :
  impl(new SceneContextImpl())
{
}

SceneContext::SceneContext(const Rect& rect) :
  impl(new SceneContextImpl(rect))
{
}

SceneContext::~SceneContext()
{
}

DrawingContext&
SceneContext::color()
{
  return impl->color;
}

DrawingContext&
SceneContext::light()
{
  return impl->light;
}

DrawingContext&
SceneContext::highlight()
{
  return impl->highlight;
}

/** Translate the drawing context */
void
SceneContext::translate(int x, int y)
{
  impl->color.translate(x, y);
  impl->light.translate(x, y);
  impl->highlight.translate(x, y);
}

void
SceneContext::push_modelview()
{
  impl->color.push_modelview();
  impl->light.push_modelview();
  impl->highlight.push_modelview();
}

void
SceneContext::pop_modelview()
{
  impl->color.pop_modelview();
  impl->light.pop_modelview();
  impl->highlight.pop_modelview();
}

void
SceneContext::reset_modelview()
{
  impl->color.reset_modelview();
  impl->light.reset_modelview();
  impl->highlight.reset_modelview();
}

void
SceneContext::set_rect(const Rect& rect)
{
  impl->color.set_rect(rect);
  impl->light.set_rect(rect);
  impl->highlight.set_rect(rect);
}

void
SceneContext::set_cliprect(const Rect& rect)
{
  impl->cliprect = rect;
  impl->use_cliprect = true;
}

void
SceneContext::reset_cliprect()
{
  impl->use_cliprect = false;
}

void
SceneContext::render(Framebuffer& fb, const Rect& rect)
{
  // Render all buffers
  // FIXME: Render all to pbuffer for later combining of them
  if (impl->use_cliprect)
  {
    fb.push_cliprect(impl->cliprect);
    impl->color.render(fb, rect);
    fb.pop_cliprect();
  }
  else
  {
    impl->color.render(fb, rect);
  }

#if 0
  { // lightmap support
    impl->light.render(impl->canvas.get_gc());
    impl->canvas.sync_surface();

    //impl->lightmap.set_blend_func(blend_src_alpha, blend_one);
    impl->lightmap.set_blend_func(blend_dest_color, blend_zero);
    //GL_DST_COLOR, GL_ZERO
    impl->lightmap.set_scale(SCALE_FACTOR, SCALE_FACTOR);
    impl->lightmap.draw();
    impl->canvas.get_gc()->clear();
  }
#endif

  impl->highlight.render(fb, rect);
}

void
SceneContext::clear()
{
  impl->color.clear();
  impl->light.clear();
  impl->highlight.clear();
}

SceneContextDrawingRequest::SceneContextDrawingRequest(SceneContext* sc_, const Vector2i& pos_, float z_)
  : DrawingRequest(pos_, z_),
    sc(sc_)
{
}

SceneContextDrawingRequest::~SceneContextDrawingRequest()
{
  //delete sc;
}

void
SceneContextDrawingRequest::render(Framebuffer& fb, const Rect& rect)
{
  sc->render(fb, rect);
}

/* EOF */
