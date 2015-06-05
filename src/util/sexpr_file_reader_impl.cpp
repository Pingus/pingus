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
#include "util/file_reader.hpp"
#include "util/file_reader_impl.hpp"
#include "util/raise_exception.hpp"
#include "util/log.hpp"

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
  if (m_sexpr->get_list_size() < 1)
  {
    raise_exception(std::runtime_error, "invalid syntax");
    return {};
  }
  else
  {
    return m_sexpr->get_list_elem(0)->get_symbol();
  }
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
SExprReaderMappingImpl::read_int(const char* key, int& value) const
{
  std::shared_ptr<lisp::Lisp> item = get_subsection_item(key);
  if (item && item->get_type() == lisp::Lisp::TYPE_INT)
  {
    value = item->get_int();
    return true;
  }
  else
  {
    return false;
  }
}

bool
SExprReaderMappingImpl::read_float(const char* key, float& value) const
{
  std::shared_ptr<lisp::Lisp> item = get_subsection_item(key);
  if (item)
  {
    if (item->get_type() == lisp::Lisp::TYPE_FLOAT)
    {
      value = item->get_float();
      return true;
    }
    else if (item->get_type() == lisp::Lisp::TYPE_INT)
    {
      value = static_cast<float>(item->get_int());
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
SExprReaderMappingImpl::read_bool(const char* key, bool& value) const
{
  std::shared_ptr<lisp::Lisp> item = get_subsection_item(key);
  if (item && item->get_type() == lisp::Lisp::TYPE_BOOL)
  {
    value = item->get_bool();
    return true;
  }
  else if (item && item->get_type() == lisp::Lisp::TYPE_INT)
  {
    value = item->get_int();
    return true;
  }
  else
  {
    return false;
  }
}

bool
SExprReaderMappingImpl::read_string(const char* key, std::string& value) const
{
  std::shared_ptr<lisp::Lisp> sub = get_subsection(key);
  if (sub)
  {
    value = "";
    for(size_t i = 1; i < sub->get_list_size(); ++i)
    {
      std::shared_ptr<lisp::Lisp> item = sub->get_list_elem(i);
      if (item->get_type() == lisp::Lisp::TYPE_STRING)
      {
        value += item->get_string();
      }
      else if (item->get_type() == lisp::Lisp::TYPE_SYMBOL)
      {
        value += item->get_symbol();
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
SExprReaderMappingImpl::read_vector(const char* key, Vector3f& value) const
{
  std::shared_ptr<lisp::Lisp> sub = get_subsection(key);
  if (sub && sub->get_list_size() == 4)
  {
    value = Vector3f(sub->get_list_elem(1)->get_float(),
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
SExprReaderMappingImpl::read_vectors(const char* key, std::vector<Vector3f>& values) const
{
  std::shared_ptr<lisp::Lisp> sub_lst = get_subsection(key);
  if (sub_lst)
  {
    for(size_t i = 1; i < sub_lst->get_list_size(); ++i)
    {
      auto sub = sub_lst->get_list_elem(i);
      if (sub && sub->get_list_size() == 3)
      {
        values.emplace_back(sub->get_list_elem(0)->get_float(),
                            sub->get_list_elem(1)->get_float(),
                            sub->get_list_elem(2)->get_float());
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
SExprReaderMappingImpl::read_size(const char* key, Size& value) const
{
  std::shared_ptr<lisp::Lisp> sub = get_subsection(key);
  if (sub && sub->get_list_size() == 3)
  {
    value.width  = sub->get_list_elem(1)->get_int();
    value.height = sub->get_list_elem(2)->get_int();
    return true;
  }
  else
  {
    return false;
  }
}

bool
SExprReaderMappingImpl::read_vector2i(const char* key, Vector2i& value) const
{
  std::shared_ptr<lisp::Lisp> sub = get_subsection(key);
  if (sub && sub->get_list_size() == 3)
  {
    value.x = sub->get_list_elem(1)->get_int();
    value.y = sub->get_list_elem(2)->get_int();
    return true;
  }
  else
  {
    return false;
  }
}

bool
SExprReaderMappingImpl::read_rect(const char* key, Rect& rect) const
{
  std::shared_ptr<lisp::Lisp> sub = get_subsection(key);
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
SExprReaderMappingImpl::read_colorf(const char* key, Color& value) const
{
  std::shared_ptr<lisp::Lisp> sub = get_subsection(key);
  if (sub && sub->get_list_size() == 5)
  {
    value = Color(static_cast<uint8_t>(sub->get_list_elem(1)->get_float() * 255),
                  static_cast<uint8_t>(sub->get_list_elem(2)->get_float() * 255),
                  static_cast<uint8_t>(sub->get_list_elem(3)->get_float() * 255),
                  static_cast<uint8_t>(sub->get_list_elem(4)->get_float() * 255));
    return true;
  }
  else
  {
    return false;
  }
}

bool
SExprReaderMappingImpl::read_colori(const char* key, Color& value) const
{
  std::shared_ptr<lisp::Lisp> sub = get_subsection(key);
  if (sub && sub->get_list_size() == 5)
  {
    value = Color(static_cast<uint8_t>(sub->get_list_elem(1)->get_int()),
                  static_cast<uint8_t>(sub->get_list_elem(2)->get_int()),
                  static_cast<uint8_t>(sub->get_list_elem(3)->get_int()),
                  static_cast<uint8_t>(sub->get_list_elem(4)->get_int()));
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
SExprReaderMappingImpl::read_collection(const char* key, ReaderCollection& value) const
{
  std::shared_ptr<lisp::Lisp> cur = get_subsection(key);
  if (cur)
  {
    value = ReaderCollection(std::make_shared<SExprReaderCollectionImpl>(cur));
    return true;
  }
  else
  {
    return false;
  }
}

bool
SExprReaderMappingImpl::read_mapping(const char* key, ReaderMapping& value) const
{
  std::shared_ptr<lisp::Lisp> cur = get_subsection(key);
  if (cur)
  {
    value = ReaderMapping(std::make_shared<SExprReaderMappingImpl>(cur));
    return true;
  }
  else
  {
    return false;
  }
}

std::shared_ptr<lisp::Lisp>
SExprReaderMappingImpl::get_subsection_item(const char* key) const
{
  std::shared_ptr<lisp::Lisp> sub = get_subsection(key);
  if (sub && sub->get_list_size() >= 2)
  {
    if (sub->get_list_size() > 2)
    {
      log_error("invalid items in section: %1%", key);
    }

    return sub->get_list_elem(1);
  }
  else
  {
    return {};
  }
}

std::shared_ptr<lisp::Lisp>
SExprReaderMappingImpl::get_subsection(const char* key) const
{
  std::shared_ptr<lisp::Lisp> result;
  int count = 0;
  for(size_t i = 1; i < m_sexpr->get_list_size(); ++i)
  {
    auto sexpr = m_sexpr->get_list_elem(i);
    if (strcmp(sexpr->get_list_elem(0)->get_symbol(), key) == 0)
    {
      count += 1;
      result = sexpr;
    }
  }

  if (count > 1)
  {
    log_error("duplicate key value '%1%'", key);
  }

  return result;
}

/* EOF */
