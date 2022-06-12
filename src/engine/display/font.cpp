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

#include <string_view>

#include <logmich/log.hpp>
#include <strut/utf8.hpp>
#include <strut/split.hpp>

#include "engine/display/display.hpp"
#include "engine/display/font_description.hpp"
#include "engine/display/framebuffer.hpp"

namespace pingus {

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

  FontImpl(FontDescription const& desc) :
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
        log_info("IMG: {}", desc.images[j].pathname.str());
        assert(false);
      }

      framebuffer_surfaces.push_back(Display::get_framebuffer()->create_surface(surface));

      for(auto i = desc.images[j].glyphs.begin(); i != desc.images[j].glyphs.end(); ++i)
      {
        if (i->unicode < glyphs.size())
        {
          if (glyphs[i->unicode] == nullptr)
          {
            glyphs[i->unicode] = std::make_unique<GlyphDescription>(*i);
            glyphs[i->unicode]->image = static_cast<int>(framebuffer_surfaces.size()) - 1;
          }
          else
          {
            log_warn("unicode collision on {}", i->unicode);
          }
        }
        else
        {
          log_warn("unicode out of range: {}", i->unicode);
        }
      }
    }
  }

  ~FontImpl()
  {
  }

  void render(geom::origin origin, int x, int y_, std::string const& text, Framebuffer& fb) const
  {
    y_ += get_height();

    float y = float(y_);
    // FIXME: only origins top_left, top_right and top_center do work right now
    for(auto const&& line : strut::splitter(text, '\n')) {
      render_line(origin, x, int(y), line, fb);
      y += vertical_spacing;
    }
  }

  void render_line(geom::origin origin, int x, int y, std::string_view text, Framebuffer& fb) const
  {
    if (text.empty()) { return; }

    geom::ioffset offset = (-geom::anchor_offset(get_size(text), origin));

    float dstx = float(x - offset.x());
    float dsty = float(y - offset.y());

    strut::utf8::iterator i(text);
    while(i.next())
    {
      uint32_t const& unicode = *i;

      if (unicode < glyphs.size() && glyphs[unicode])
      {
        GlyphDescription const& glyph = *glyphs[unicode];
        fb.draw_surface(framebuffer_surfaces[static_cast<size_t>(glyph.image)],
                        glyph.rect, geom::ipoint(static_cast<int>(dstx), static_cast<int>(dsty)) + geom::ioffset(glyph.offset.as_vec()));
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

  float get_width(std::string_view text) const
  {
    float width = 0.0f;
    float last_width = 0;

    strut::utf8::iterator i(text);
    while(i.next())
    {
      uint32_t const& unicode = *i;

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

  geom::isize get_size(std::string_view text) const
  {
    return geom::isize(static_cast<int>(get_width(text)), get_height());
  }

  geom::irect bounding_rect(int x, int y, std::string const& str) const
  {
    return geom::irect(geom::ipoint(x, y), get_size(str));
  }
};

Font::Font() :
  impl()
{
}

Font::Font(FontDescription const& desc) :
  impl(new FontImpl(desc))
{
}

void
Font::render(int x, int y, std::string const& text, Framebuffer& fb)
{
  if (impl)
    impl->render(geom::origin::TOP_LEFT, x,y,text, fb);
}

void
Font::render(geom::origin origin, int x, int y, std::string const& text, Framebuffer& fb)
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
Font::get_width(std::string const& text) const
{
  if (impl)
    return impl->get_width(text);
  else
    return 0;
}

geom::isize
Font::get_size(std::string const& str) const
{
  if (impl)
    return impl->get_size(str);
  else
    return geom::isize();
}

geom::irect
Font::bounding_rect(int x, int y, std::string const& str) const
{
  if (impl)
    return impl->bounding_rect(x, y, str);
  else
    return geom::irect();
}

} // namespace pingus

/* EOF */
