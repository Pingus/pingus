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

#include "engine/display/font_description.hpp"

#include <stdexcept>

#include "util/reader.hpp"
#include "util/raise_exception.hpp"

GlyphDescription::GlyphDescription() :
  image(0),
  unicode(0),
  offset(),
  advance(0),
  rect()
{
}

GlyphDescription::GlyphDescription(const ReaderMapping& reader) :
  image(0),
  unicode(0),
  offset(),
  advance(0),
  rect()
{
  int lazy = 0; // FIXME: implement read_uint32
  reader.read_int("unicode", lazy);
  unicode = static_cast<uint32_t>(lazy);
  reader.read_vector2i("offset", offset);
  reader.read_int("advance", advance);
  reader.read_rect("rect", rect);
}

FontDescription::FontDescription(const Pathname& pathname_) :
  pathname(pathname_),
  char_spacing(),
  vertical_spacing(),
  size(),
  images()
{
  char_spacing     = 1.0f;
  vertical_spacing = 1.0f;

  ReaderObject reader_object = Reader::parse(pathname);

  if (reader_object.get_name() != "pingus-font")
  {
    raise_exception(std::runtime_error, pathname << ": not a pingus-font file");
  }
  else
  {
    ReaderMapping reader = reader_object.get_mapping();

    reader.read_float("char-spacing", char_spacing);
    reader.read_float("vertical-spacing", vertical_spacing);
    reader.read_int("size", size);

    ReaderCollection images_reader;
    if (reader.read_collection("images", images_reader))
    {
      auto images_lst = images_reader.get_objects();

      for(auto i = images_lst.begin(); i != images_lst.end(); ++i)
      {
        ReaderMapping mapping = i->get_mapping();

        GlyphImageDescription image_desc;
        mapping.read_path("filename", image_desc.pathname);

        ReaderCollection glyph_collection;
        if (mapping.read_collection("glyphs", glyph_collection))
        {
          std::vector<ReaderObject> glyph_reader = glyph_collection.get_objects();
          for(auto j = glyph_reader.begin(); j != glyph_reader.end(); ++j)
          {
            if (j->get_name() == "glyph")
            {
              image_desc.glyphs.push_back(GlyphDescription(j->get_mapping()));
            }
            else
            {
              log_warn("unknown tag: %1%", j->get_name());
            }
          }
        }
        images.push_back(image_desc);
      }
    }
  }
}

/* EOF */
