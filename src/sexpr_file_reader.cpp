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
#include "math/vector3f.hpp"
#include "math/color.hpp"
#include "math/size.hpp"
#include "file_reader_impl.hxx"
#include "sexpr_file_reader.hpp"

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
    // FIXME: Do we have to free the lisp pointer here or outside of the code?
  }

  std::string get_name() const 
  {
    return sexpr->get_list_elem(0)->get_symbol();
  }

  bool read_int   (const char* name, int& v) const 
  {
    lisp::Lisp* item = get_subsection_item(name);
    if (item && item->get_type() == lisp::Lisp::TYPE_INT)
      {
        v = item->get_int();
        return true;
      }
    return false;
  }

  bool read_float (const char* name, float& v) const 
  {
    lisp::Lisp* item = get_subsection_item(name);
    if (item && item->get_type() == lisp::Lisp::TYPE_FLOAT)
      {
        v = item->get_float();
        return true;
      }
    return false;
  }

  bool read_bool  (const char* name, bool& v) const 
  {
    lisp::Lisp* item = get_subsection_item(name);
    if (item && item->get_type() == lisp::Lisp::TYPE_BOOL)
      {
        v = item->get_bool();
        return true;
      }
    return false;
  }

  bool read_string(const char* name, std::string& v) const 
  {
    lisp::Lisp* item = get_subsection_item(name);
    if (item)
      {
        if (item->get_type() == lisp::Lisp::TYPE_STRING)
          {
            v = item->get_string();
            return true;
          }
        else if (item->get_type() == lisp::Lisp::TYPE_SYMBOL)
          {
            v = item->get_symbol();
            return true;
          }
      }
    return false;
  }

  bool read_vector(const char* name, Vector3f& v) const
  {
    lisp::Lisp* sub = get_subsection_item(name);
    if (sub && sub->get_list_size() == 4)
      {
        v = Vector3f(sub->get_list_elem(1)->get_float(),
                     sub->get_list_elem(2)->get_float(),
                     sub->get_list_elem(3)->get_float());
        return true;
      }    
    return false;
  }

  bool read_color (const char* name, Color& v) const
  {
    lisp::Lisp* sub = get_subsection_item(name);
    if (sub && sub->get_list_size() == 5)
      {
        v = Color(int(sub->get_list_elem(1)->get_float() * 255),
                  int(sub->get_list_elem(2)->get_float() * 255),
                  int(sub->get_list_elem(3)->get_float() * 255),
                  int(sub->get_list_elem(4)->get_float() * 255));
        return true;
      }
    return false;
  }

  bool read_desc  (const char* name, ResDescriptor& v) const 
  {
    return false;
  }

  bool read_size  (const char* name, Size& v) const 
  {
    lisp::Lisp* sub = get_subsection_item(name);
    if (sub && sub->get_list_size() == 3)
      {
        v = Size(int(sub->get_list_elem(1)->get_int()),
                 int(sub->get_list_elem(2)->get_int()));
        return true;
      }
    return false;
  }

  bool read_section(const char* name, FileReader& v) const 
  {
    lisp::Lisp* cur = get_subsection(name);
    if (cur)
      {
        v = SExprFileReader(cur);
        return true;
      }
    return false;
  }

  std::vector<FileReader> get_sections() const 
  {
    std::vector<FileReader> lst;
    for(size_t i = 1; i < sexpr->get_list_size(); ++i)
      { // iterate over subsections
        lst.push_back(SExprFileReader(sexpr->get_list_elem(i)));
      }
    return lst;
  }

  std::vector<std::string> get_section_names() const 
  {
    std::vector<std::string> lst;

    for(size_t i = 1; i < sexpr->get_list_size(); ++i)
      { // iterate over subsections
        lisp::Lisp* sub = sexpr->get_list_elem(i);
        lst.push_back(sub->get_list_elem(0)->get_symbol());
      }

    return lst;
  }

private:
  lisp::Lisp* get_subsection_item(const char* name) const
  {
    lisp::Lisp* sub = get_subsection(name);
    if (sub && sub->get_list_size() == 2)
      {
        return sub->get_list_elem(1);
      }
    return 0;
  }

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

/* EOF */
