//  Pingus - A free Lemmings clone
//  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
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

#include "pingus_error.hpp"
#include "file_reader.hpp"
#include "font_description.hpp"


GlyphDescription::GlyphDescription()
  : image(0), unicode(0), advance(0)
{
  
}

GlyphDescription::GlyphDescription(FileReader& reader)
{
  image = 0;
  int lazy = 0; // FIXME: implement read_uint32
  reader.read_int("unicode", lazy);
  unicode = lazy;
  reader.read_vector2i("offset",  offset);
  reader.read_int("advance", advance);
  reader.read_rect("rect",   rect);
}

FontDescription::FontDescription(const Pathname& pathname_)
  : pathname(pathname_)
{
  char_spacing     = 1.0f;
  vertical_spacing = 1.0f;

  FileReader reader = FileReader::parse(pathname);

  if (reader.get_name() != "pingus-font")
    {
      PingusError::raise("FontDescription: not a pingus-font file");
    }
  else
    {
      reader.read_float("char-spacing",     char_spacing);
      reader.read_float("vertical-spacing", vertical_spacing);
      reader.read_int("size",               size);

      FileReader images_reader;
      if (reader.read_section("images", images_reader))
        {
          std::vector<FileReader> images_lst = images_reader.get_sections();

          for(std::vector<FileReader>::iterator i = images_lst.begin(); i != images_lst.end(); ++i)
            {
              GlyphImageDescription image_desc;
              i->read_path("filename",             image_desc.pathname);
      
              FileReader glyph_section;
              if (i->read_section("glyphs", glyph_section))
                {
                  std::vector<FileReader> glyph_reader = glyph_section.get_sections();
                  for(std::vector<FileReader>::iterator i = glyph_reader.begin(); i != glyph_reader.end(); ++i)
                    {
                      image_desc.glyphs.push_back(GlyphDescription(*i));
                    }
                }
              images.push_back(image_desc);
            }
        }
    }
}

/* EOF */
