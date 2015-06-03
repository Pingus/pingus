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

#include "util/sexpr_file_reader_impl.hpp"

#include "math/color.hpp"
#include "math/rect.hpp"
#include "math/vector3f.hpp"
#include "pingus/res_descriptor.hpp"
#include "util/file_reader_impl.hpp"
#include "util/file_reader.hpp"

SExprReaderObjectImpl::SExprReaderObjectImpl(std::shared_ptr<lisp::Lisp> sexpr) :
  m_sexpr(sexpr)
{
  // Expects data in the format:
  // (objectname
  //   (property1 45)
  //   (property2 45))
}

SExprReaderObjectImpl::~SExprReaderObjectImpl()
{
}

std::string
SExprReaderObjectImpl::get_name() const
{
  return m_sexpr->get_list_elem(0)->get_symbol();
}

ReaderMapping
SExprReaderObjectImpl::get_mapping() const
{
  return ReaderMapping(std::make_shared<SExprReaderMappingImpl>(m_sexpr));
}


SExprReaderCollectionImpl::SExprReaderCollectionImpl(std::shared_ptr<lisp::Lisp> sexpr) :
  m_sexpr(sexpr)
{
}

SExprReaderCollectionImpl::~SExprReaderCollectionImpl()
{
}

std::vector<ReaderObject>
SExprReaderCollectionImpl::get_objects() const
{
  std::vector<ReaderObject> lst;
  for(size_t i = 1; i < m_sexpr->get_list_size(); ++i)
  {
    lst.push_back(ReaderObject(std::make_shared<SExprReaderObjectImpl>(m_sexpr->get_list_elem(i))));
  }
  return lst;
}


SExprReaderMappingImpl::SExprReaderMappingImpl(std::shared_ptr<lisp::Lisp> sexpr) :
  m_sexpr(sexpr)
{
  assert(m_sexpr->get_type() == lisp::Lisp::TYPE_LIST &&
         m_sexpr->get_list_size() >= 1);
  // Expects data in this format:
  // (objectname
  //   (property1 45)
  //   (property2 45))
  // 'objectname' is ignored
}

SExprReaderMappingImpl::~SExprReaderMappingImpl()
{
}

std::vector<std::string>
SExprReaderMappingImpl::get_keys() const
{
  std::vector<std::string> lst;

  for(size_t i = 1; i < m_sexpr->get_list_size(); ++i)
  {
    std::shared_ptr<lisp::Lisp> sub = m_sexpr->get_list_elem(i);
    lst.push_back(sub->get_list_elem(0)->get_symbol());
  }

  return lst;
}

bool
SExprReaderMappingImpl::read_int(const char* name, int& v) const
{
  std::shared_ptr<lisp::Lisp> item = get_subsection_item(name);
  if (item && item->get_type() == lisp::Lisp::TYPE_INT)
  {
    v = item->get_int();
    return true;
  }
  else
  {
    return false;
  }
}

bool
SExprReaderMappingImpl::read_float(const char* name, float& v) const
{
  std::shared_ptr<lisp::Lisp> item = get_subsection_item(name);
  if (item)
  {
    if (item->get_type() == lisp::Lisp::TYPE_FLOAT)
    {
      v = item->get_float();
      return true;
    }
    else if (item->get_type() == lisp::Lisp::TYPE_INT)
    {
      v = static_cast<float>(item->get_int());
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}

bool
SExprReaderMappingImpl::read_bool(const char* name, bool& v) const
{
  std::shared_ptr<lisp::Lisp> item = get_subsection_item(name);
  if (item && item->get_type() == lisp::Lisp::TYPE_BOOL)
  {
    v = item->get_bool();
    return true;
  }
  else if (item && item->get_type() == lisp::Lisp::TYPE_INT)
  {
    v = item->get_int();
    return true;
  }
  else
  {
    return false;
  }
}

bool
SExprReaderMappingImpl::read_string(const char* name, std::string& v) const
{
  std::shared_ptr<lisp::Lisp> sub = get_subsection(name);
  if (sub)
  {
    v = "";
    for(size_t i = 1; i < sub->get_list_size(); ++i)
    {
      std::shared_ptr<lisp::Lisp> item = sub->get_list_elem(i);
      if (item->get_type() == lisp::Lisp::TYPE_STRING)
      {
        v += item->get_string();
      }
      else if (item->get_type() == lisp::Lisp::TYPE_SYMBOL)
      {
        v += item->get_symbol();
      }
    }
    return true;
  }
  else
  {
    return false;
  }
}

bool
SExprReaderMappingImpl::read_vector(const char* name, Vector3f& v) const
{
  std::shared_ptr<lisp::Lisp> sub = get_subsection(name);
  if (sub && sub->get_list_size() == 4)
  {
    v = Vector3f(sub->get_list_elem(1)->get_float(),
                 sub->get_list_elem(2)->get_float(),
                 sub->get_list_elem(3)->get_float());
    return true;
  }
  else
  {
    return false;
  }
}

bool
SExprReaderMappingImpl::read_size(const char* name, Size& v) const
{
  std::shared_ptr<lisp::Lisp> sub = get_subsection(name);
  if (sub && sub->get_list_size() == 3)
  {
    v.width  = sub->get_list_elem(1)->get_int();
    v.height = sub->get_list_elem(2)->get_int();
    return true;
  }
  else
  {
    return false;
  }
}

bool
SExprReaderMappingImpl::read_vector2i(const char* name, Vector2i& v) const
{
  std::shared_ptr<lisp::Lisp> sub = get_subsection(name);
  if (sub && sub->get_list_size() == 3)
  {
    v.x = sub->get_list_elem(1)->get_int();
    v.y = sub->get_list_elem(2)->get_int();
    return true;
  }
  else
  {
    return false;
  }
}

bool
SExprReaderMappingImpl::read_rect(const char* name, Rect& rect) const
{
  std::shared_ptr<lisp::Lisp> sub = get_subsection(name);
  if (sub && sub->get_list_size() == 5)
  {
    rect.left   = sub->get_list_elem(1)->get_int();
    rect.top    = sub->get_list_elem(2)->get_int();
    rect.right  = sub->get_list_elem(3)->get_int();
    rect.bottom = sub->get_list_elem(4)->get_int();
    return true;
  }
  else
  {
    return false;
  }
}

bool
SExprReaderMappingImpl::read_colorf(const char* name, Color& v) const
{
  std::shared_ptr<lisp::Lisp> sub = get_subsection(name);
  if (sub && sub->get_list_size() == 5)
  {
    v = Color(static_cast<char>(sub->get_list_elem(1)->get_float() * 255),
              static_cast<char>(sub->get_list_elem(2)->get_float() * 255),
              static_cast<char>(sub->get_list_elem(3)->get_float() * 255),
              static_cast<char>(sub->get_list_elem(4)->get_float() * 255));
    return true;
  }
  else
  {
    return false;
  }
}

bool
SExprReaderMappingImpl::read_colori(const char* name, Color& v) const
{
  std::shared_ptr<lisp::Lisp> sub = get_subsection(name);
  if (sub && sub->get_list_size() == 5)
  {
    v = Color(static_cast<char>(sub->get_list_elem(1)->get_int()),
              static_cast<char>(sub->get_list_elem(2)->get_int()),
              static_cast<char>(sub->get_list_elem(3)->get_int()),
              static_cast<char>(sub->get_list_elem(4)->get_int()));
    return true;
  }
  else
  {
    return false;
  }
}

bool
SExprReaderMappingImpl::read_desc(const char* name, ResDescriptor& v) const
{
  std::shared_ptr<lisp::Lisp> sub = get_subsection(name);
  if (sub)
  {
    auto reader = ReaderMapping(std::make_shared<SExprReaderMappingImpl>(sub));
    reader.read_string("image", v.res_name);
    reader.read_enum("modifier", v.modifier, &ResourceModifier::from_string);
    return true;
  }
  else
  {
    return false;
  }
}

bool
SExprReaderMappingImpl::read_object(const char* key, ReaderObject& value) const
{
  std::shared_ptr<lisp::Lisp> cur = get_subsection_item(key);
  if (cur)
  {
    value = ReaderObject(std::make_shared<SExprReaderObjectImpl>(cur));
    return true;
  }
  else
  {
    return false;
  }
}

bool
SExprReaderMappingImpl::read_collection(const char* name, ReaderCollection& v) const
{
  std::shared_ptr<lisp::Lisp> cur = get_subsection(name);
  if (cur)
  {
    v = ReaderCollection(std::make_shared<SExprReaderCollectionImpl>(cur));
    return true;
  }
  else
  {
    return false;
  }
}

bool
SExprReaderMappingImpl::read_mapping(const char* name, ReaderMapping& v) const
{
  std::shared_ptr<lisp::Lisp> cur = get_subsection(name);
  if (cur)
  {
    v = ReaderMapping(std::make_shared<SExprReaderMappingImpl>(cur));
    return true;
  }
  else
  {
    return false;
  }
}

std::shared_ptr<lisp::Lisp>
SExprReaderMappingImpl::get_subsection_item(const char* name) const
{
  std::shared_ptr<lisp::Lisp> sub = get_subsection(name);
  if (sub && sub->get_list_size() == 2)
  {
    return sub->get_list_elem(1);
  }
  else
  {
    return {};
  }
}

std::shared_ptr<lisp::Lisp>
SExprReaderMappingImpl::get_subsection(const char* name) const
{
  for(size_t i = 1; i < m_sexpr->get_list_size(); ++i)
  {
    std::shared_ptr<lisp::Lisp> sub = m_sexpr->get_list_elem(i);
    if (strcmp(sub->get_list_elem(0)->get_symbol(), name) == 0)
    {
      return sub;
    }
  }
  return {};
}

/* EOF */
