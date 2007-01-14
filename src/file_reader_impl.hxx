//  $Id$
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_FILE_READER_IMPL_HXX
#define HEADER_FILE_READER_IMPL_HXX

#include <vector>
#include <string>

class Size;
class Color;
class Vector3f;


class FileReader;
class ResDescriptor;

/** */
class FileReaderImpl
{
public:
  FileReaderImpl() {}
  virtual ~FileReaderImpl() {}

  virtual std::string get_name()                           const =0;
  virtual bool read_int   (const char* name, int&)         const =0;
  virtual bool read_float (const char* name, float&)       const =0;
  virtual bool read_bool  (const char* name, bool&)        const =0;
  virtual bool read_string(const char* name, std::string&) const =0;
  virtual bool read_vector(const char* name, Vector3f&)    const =0;
  virtual bool read_color (const char* name, Color&)   const =0;
  virtual bool read_desc  (const char* name, ResDescriptor&) const =0;
  ////  virtual bool read_size  (const char* name, CL_Size&)       const =0;
  virtual bool read_section(const char* name, FileReader&)   const =0;
  virtual std::vector<FileReader> get_sections() const =0;
  virtual std::vector<std::string> get_section_names() const =0;
};


#endif

/* EOF */
