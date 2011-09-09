//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_UTIL_FILE_READER_HPP
#define HEADER_PINGUS_UTIL_FILE_READER_HPP

#include <memory>
#include <vector>

class Size;
class Color;
class Vector3f;
class Vector2i;
class Rect;

class ResDescriptor;
class FileReaderImpl;
class Pathname;

/** Interface to read name/value pairs out of some kind of file or
    structure */
class FileReader
{
public:
  FileReader(std::shared_ptr<FileReaderImpl> impl_);
  FileReader();
  virtual ~FileReader() {}

  /** Name of the current section, ie. in the case of
      <groundpiece><pos>...</groundpiece> it would be 'groundpiece' */
  std::string get_name() const;

  bool read_int   (const char* name, int&)           const;
  bool read_float (const char* name, float&)         const;
  bool read_bool  (const char* name, bool&)          const;
  bool read_string(const char* name, std::string&)   const;
  bool read_path  (const char* name, Pathname&)      const;
  bool read_vector(const char* name, Vector3f&)      const;
  bool read_vector2i(const char* name, Vector2i&)    const;
  bool read_rect(const char* name, Rect&)    const;
  bool read_size  (const char* name, Size&)          const;
  bool read_colorf(const char* name, Color&)         const;
  bool read_colori(const char* name, Color&)         const;
  bool read_desc  (const char* name, ResDescriptor&) const;
  bool read_section(const char* name, FileReader&)   const;
  FileReader read_section(const char* name)   const;

  template<class E, class T>
  bool read_enum  (const char* name, E& value, T enum2string) const
  {
    std::string str;
    if (read_string(name, str))
    {
      value = enum2string(str);
      return true;
    }

    return false;
  }

  std::vector<std::string> get_section_names() const;
  std::vector<FileReader>  get_sections() const;
  int  get_num_sections() const;

  static FileReader parse(const std::string& filename);
  static FileReader parse(const Pathname& pathname);

  /** Reads multiple trees from a file, for use with files that don't
      contain a root element */
  static std::vector<FileReader> parse_many(const Pathname& pathname);

private:
  std::shared_ptr<FileReaderImpl> impl;
};

#endif

/* EOF */
