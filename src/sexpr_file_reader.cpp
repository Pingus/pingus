/*  $Id$
**   __      __ __             ___        __   __ __   __
**  /  \    /  \__| ____    __| _/_______/  |_|__|  | |  |   ____
**  \   \/\/   /  |/    \  / __ |/  ___/\   __\  |  | |  | _/ __ \
**   \        /|  |   |  \/ /_/ |\___ \  |  | |  |  |_|  |_\  ___/
**    \__/\  / |__|___|  /\____ /____  > |__| |__|____/____/\___  >
**         \/          \/      \/    \/                         \/
**  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
** 
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
**  02111-1307, USA.
*/

#include <assert.h>
#include "file_reader_impl.hxx"
#include "sexpr_file_reader.hpp"

namespace Pingus {

class SExprFileReaderImpl: public FileReaderImpl
{
public:
  lisp::Lisp* sexpr;

  SExprFileReaderImpl(lisp::Lisp* sexpr_) 
    : sexpr(sexpr_)
  {
    assert(sexpr->get_type() == lisp::Lisp::TYPE_LIST &&
           sexpr->get_list_size() >= 1);

    for(size_t i = 1; i < sexpr->get_list_size(); ++i)
      { // iterate over subsections
        sexpr->get_list_elem(i);
      }
  }

  ~SExprFileReaderImpl()
  {
    
  }

  std::string get_name() const 
  {
    return sexpr->get_list_elem(0)->get_symbol();
  }

  bool read_int   (const char* name, int& v) const 
  {
    lisp::Lisp* sub = get_subsection(name);
    if (sub && sub->get_list_size() == 2)
      {
        if (sub->get_list_elem(1)->get_type() == lisp::Lisp::TYPE_INT)
          {
            v = sub->get_list_elem(1)->get_int();
            return true;
          }
      }
    return false;
  }

  bool read_float (const char* name, float&) const 
  {
    return false;
  }

  bool read_bool  (const char* name, bool&) const 
  {
    return false;
  }

  bool read_string(const char* name, std::string&) const 
  {
    return false;
  }

  bool read_vector(const char* name, Vector&) const
  {
    return false;
  }

  bool read_color (const char* name, Color&) const
  {
    return false;
  }

  bool read_desc  (const char* name, ResDescriptor&) const 
  {
    return false;
  }

  bool read_size  (const char* name, CL_Size&) const 
  {
    return false;
  }

  bool read_section(const char* name, FileReader&) const 
  {
    return false;
  }

  std::vector<FileReader> get_sections() const 
  {
    return std::vector<FileReader>();
  }

  std::vector<std::string> get_section_names() const 
  {
    return std::vector<std::string>();
  }

private:
  lisp::Lisp* get_subsection(const char* name) const
  {
    for(size_t i = 1; i < sexpr->get_list_size(); ++i)
      { // iterate over subsections
        lisp::Lisp* sub = sexpr->get_list_elem(i);
        if (strcmp(sub->get_list_elem(0)->get_symbol(), name) == 0)
          return sub;
      }
    return 0;
  }
  
};

SExprFileReader::SExprFileReader(lisp::Lisp* lisp)
  : FileReader(SharedPtr<FileReaderImpl>(new SExprFileReaderImpl(lisp)))
{
  
}

} // namespace Pingus

/* EOF */
