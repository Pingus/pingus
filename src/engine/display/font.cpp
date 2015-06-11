// Pingus - A free Lemmings clone
// Copyright (C) 2005 Ingo Ruhnke <grumbel@gmail.com>
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

#include "engine/display/font.hpp"

#include "engine/display/display.hpp"
#include "engine/display/font_description.hpp"
#include "engine/display/framebuffer.hpp"
#include "util/line_iterator.hpp"
#include "util/log.hpp"
#include "util/mem.hpp"
#include "util/utf8.hpp"

class FontImpl
{
public:
  std::vector<FramebufferSurface> framebuffer_surfaces;
  typedef std::vector<std::unique_ptr<GlyphDescription> > Glyphs;
  Glyphs glyphs;
  int    space_length;
  float  char_spacing;
  float  vertical_spacing;
  int    size;

  FontImpl(const FontDescription& desc) :
    framebuffer_surfaces(),
    glyphs(),
    space_length(),
    char_spacing(desc.char_spacing),
    vertical_spacing(),
    size(desc.size)
  {
    vertical_spacing = static_cast<float>(size) * desc.vertical_spacing;

    glyphs.resize(65536); // 16bit ought to be enough for everybody

    // Copyh Unicode -> Glyph mapping
    for(std::vector<GlyphImageDescription>::size_type j = 0; j < desc.images.size(); ++j)
    {
      Surface surface(desc.images[j].pathname);
      if (!surface)
      {
        log_info("IMG: %1%", desc.images[j].pathname.str());
        assert(false);
      }

      framebuffer_surfaces.push_back(Display::get_framebuffer()->create_surface(surface));

      for(auto i = desc.images[j].glyphs.begin(); i != desc.images[j].glyphs.end(); ++i)
      {
        if (i->unicode < glyphs.size())
        {
          if (glyphs[i->unicode] == 0)
          {
            glyphs[i->unicode] = util::make_unique<GlyphDescription>(*i);
            glyphs[i->unicode]->image = static_cast<int>(framebuffer_surfaces.size()) - 1;
          }
          else
          {
            log_warn("unicode collision on %1%", i->unicode);
          }
        }
        else
        {
          log_warn("unicode out of range: %1%", i->unicode);
        }
      }
    }
  }

  ~FontImpl()
  {
  }

  void render(Origin origin, int x, int y_, const std::string& text, Framebuffer& fb)
  {
    y_ += get_height();

    float y = float(y_);
    // FIXME: only origins top_left, top_right and top_center do work right now
    LineIterator it(text);
    while(it.next()) {
      render_line(origin, x, int(y), it.get(), fb);
      y += vertical_spacing;
    }
  }

  void render_line(Origin origin, int x, int y, const std::string& text, Framebuffer& fb)
  {
    Vector2i offset = calc_origin(origin, get_size(text));

    float dstx = float(x - offset.x);
    float dsty = float(y - offset.y);

    UTF8::iterator i(text);
    while(i.next())
    {
      const uint32_t& unicode = *i;

      if (unicode < glyphs.size() && glyphs[unicode])
      {
        const GlyphDescription& glyph = *glyphs[unicode];
        fb.draw_surface(framebuffer_surfaces[static_cast<size_t>(glyph.image)],
                        glyph.rect, Vector2i(static_cast<int>(dstx), static_cast<int>(dsty)) + glyph.offset);
        dstx += static_cast<float>(glyph.advance) + char_spacing;
      }
      else
      {
        // Draw placeholder char and issue a warning
      }
    }
  }

  int get_height() const
  {
    return size;
  }

  float get_width(uint32_t unicode) const
  {
    if (unicode < glyphs.size() && glyphs[unicode])
      return static_cast<float>(glyphs[unicode]->advance);
    else
      return 0;
  }

  float get_width(const std::string& text) const
  {
    float width = 0.0f;
    float last_width = 0;

    UTF8::iterator i(text);
    while(i.next())
    {
      const uint32_t& unicode = *i;

      if (unicode == '\n')
      {
        last_width = std::max(last_width, width);
        width = 0;
      }
      else
      {
        width += get_width(unicode) + char_spacing;
      }
    }

    return std::max(width, last_width);
  }

  Size get_size(const std::string& text) const
  {
    return Size(static_cast<int>(get_width(text)), get_height());
  }

  Rect bounding_rect(int x, int y, const std::string& str) const
  {
    return Rect(Vector2i(x, y), get_size(str));
  }
};

Font::Font() :
  impl()
{
}

Font::Font(const FontDescription& desc) :
  impl(new FontImpl(desc))
{
}

void
Font::render(int x, int y, const std::string& text, Framebuffer& fb)
{
  if (impl)
    impl->render(origin_top_left, x,y,text, fb);
}

void
Font::render(Origin origin, int x, int y, const std::string& text, Framebuffer& fb)
{
  if (impl)
    impl->render(origin, x,y,text, fb);
}

int
Font::get_height() const
{
  if (impl)
    return impl->get_height();
  else
    return 0;
}

float
Font::get_width(uint32_t unicode) const
{
  if (impl)
    return impl->get_width(unicode);
  else
    return 0;
}

float
Font::get_width(const std::string& text) const
{
  if (impl)
    return impl->get_width(text);
  else
    return 0;
}

Size
Font::get_size(const std::string& str) const
{
  if (impl)
    return impl->get_size(str);
  else
    return Size();
}

Rect
Font::bounding_rect(int x, int y, const std::string& str) const
{
  if (impl)
    return impl->bounding_rect(x, y, str);
  else
    return Rect();
}

/* EOF */
