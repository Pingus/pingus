// Pingus - A free Lemmings clone
// Copyright (C) 1998-2011 Ingo Ruhnke <grumbel@gmail.com>
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

#include "util/overrride_reader.hpp"

#include <set>

#include "util/reader.hpp"
#include "util/reader_impl.hpp"
#include "util/log.hpp"

class OverrideReaderMappingImpl : public ReaderMappingImpl
{
private:
  ReaderMapping m_reader;
  ReaderMapping m_overrides;

public:
  OverrideReaderMappingImpl(const ReaderMapping& reader,
                            const ReaderMapping& overrides) :
    m_reader(reader),
    m_overrides(overrides)
  {
  }

  std::vector<std::string> get_keys() const
  {
    std::set<std::string> result;

    auto lst = m_reader.get_keys();
    result.insert(lst.begin(), lst.end());
    lst = m_overrides.get_keys();
    result.insert(lst.begin(), lst.end());

    return std::vector<std::string>(result.begin(), result.end());
  }

  bool read_int(const char* name, int& v) const
  {
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

  bool read_vectors(const char* name, std::vector<Vector3f>& vec) const
  {
    if (m_overrides.read_vectors(name, vec))
    {
      return true;
    }
    else
    {
      return m_reader.read_vectors(name, vec);
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

  bool read_mapping(const char* name, ReaderMapping& result) const
  {
    ReaderMapping overwrite_result;
    if (m_overrides.read_mapping(name, overwrite_result))
    {
      if (m_reader.read_mapping(name, result))
      {
        result = make_override_mapping(result, overwrite_result);
        return true;
      }
      else
      {
        result = overwrite_result;
        return true;
      }
    }
    else
    {
      return m_reader.read_mapping(name, result);
    }
  }

  bool read_collection(const char* key, ReaderCollection& result) const
  {
    if (m_overrides.read_collection(key, result))
    {
      return true;
    }
    else
    {
      return m_reader.read_collection(key, result);
    }
  }

  bool read_object(const char* key, ReaderObject& result) const
  {
    if (m_overrides.read_object(key, result))
    {
      return true;
    }
    else
    {
      return m_reader.read_object(key, result);
    }
  }
};

ReaderMapping
make_override_mapping(const ReaderMapping& reader, const ReaderMapping& overrides)
{
  return ReaderMapping(std::make_shared<OverrideReaderMappingImpl>(reader, overrides));
}

/* EOF */
