//  Pingus - A free Lemmings clone
//  Copyright (C) 1998-2011 Ingo Ruhnke <grumbel@gmx.de>
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

#include "util/overrride_file_reader.hpp"

#include "util/file_reader_impl.hpp"
#include "util/log.hpp"

class OverrideFileReaderImpl : public FileReaderImpl
{
private:
  FileReader m_reader;
  FileReader m_overrides;

public:
  OverrideFileReaderImpl(const FileReader& reader,
                         const FileReader& overrides) :
    m_reader(reader),
    m_overrides(overrides)
  {
  }

  std::string get_name() const 
  {
    return m_reader.get_name();
  }

  bool read_int(const char* name, int& v) const
  {
    std::vector<std::string> lst = get_section_names();
    if (m_overrides.read_int(name, v))
    {
      return true;
    }
    else
    {
      if (m_reader.read_int(name, v))
      {
        return true;
      }
      return false;
    }
  }

  bool read_float(const char* name, float& v) const
  {
    if (m_overrides.read_float(name, v))
    {
      return true;
    }
    else
    {
      return m_reader.read_float(name, v);
    }
  }

  bool read_bool(const char* name, bool& v) const
  {
    if (m_overrides.read_bool(name, v))
    {
      return true;
    }
    else
    {
      return m_reader.read_bool(name, v);
    }
  }

  bool read_string(const char* name, std::string& str) const
  {
    if (m_overrides.read_string(name, str))
    {
      return true;
    }
    else
    {
      return m_reader.read_string(name, str);
    }
  }

  bool read_vector(const char* name, Vector3f& vec) const
  {
    if (m_overrides.read_vector(name, vec))
    {
      return true;
    }
    else
    {
      return m_reader.read_vector(name, vec);
    }
  }

  bool read_size(const char* name, Size& size) const
  {
    if (m_overrides.read_size(name, size))
    {
      return true;
    }
    else
    {
      return m_reader.read_size(name, size);
    }
  }

  bool read_vector2i(const char* name, Vector2i& vec) const
  {
    if (m_overrides.read_vector2i(name, vec))
    {
      return true;
    }
    else
    {
      return m_reader.read_vector2i(name, vec);
    }
  }

  bool read_rect(const char* name, Rect& rect) const
  {
    if (m_overrides.read_rect(name, rect))
    {
      return true;
    }
    else
    {
      return m_reader.read_rect(name, rect);
    }
  }

  bool read_colorf(const char* name, Color& color) const
  {
    if (m_overrides.read_colorf(name, color))
    {
      return true;
    }
    else
    {
      return m_reader.read_colorf(name, color);
    }
  }

  bool read_colori(const char* name, Color& color) const
  {
    if (m_overrides.read_colori(name, color))
    {
      return true;
    }
    else
    {
      return m_reader.read_colori(name, color);
    }
  }

  bool read_desc(const char* name, ResDescriptor& desc) const
  {
    if (m_overrides.read_desc(name, desc))
    {
      return true;
    }
    else
    {
      return m_reader.read_desc(name, desc);
    }
  }

  bool read_section(const char* name, FileReader& reader) const
  {
    if (m_overrides.read_section(name, reader))
    {
      if (m_reader.read_section(name, reader))
      {
        reader = OverrideFileReader(reader, m_overrides);
      }
      return true;
    }
    else
    {
      return m_reader.read_section(name, reader);
    }
  }

  std::vector<FileReader> get_sections() const
  {
    std::vector<FileReader> lst = m_reader.get_sections();
    std::vector<FileReader> result;

    for(auto it = lst.begin(); it != lst.end(); ++it)
    {
      result.push_back(OverrideFileReader(*it, m_overrides));
    }

    return result;
  }

  std::vector<std::string> get_section_names() const
  {
    return m_reader.get_section_names();
  }
};


OverrideFileReader::OverrideFileReader(const FileReader& reader,
                                       const FileReader& overrides) :
  FileReader(std::shared_ptr<FileReaderImpl>(new OverrideFileReaderImpl(reader, overrides)))
{
}

/* EOF */
