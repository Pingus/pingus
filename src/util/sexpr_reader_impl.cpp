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

#include "util/sexpr_reader_impl.hpp"

#include <sexp/util.hpp>
#include <sexp/io.hpp>

#include "math/color.hpp"
#include "math/rect.hpp"
#include "math/vector3f.hpp"
#include "pingus/res_descriptor.hpp"
#include "util/reader.hpp"
#include "util/reader_impl.hpp"
#include "util/raise_exception.hpp"
#include "util/log.hpp"

SExprReaderObjectImpl::SExprReaderObjectImpl(sexp::Value const& sx) :
  m_sx(sx) // FIXME: all this copying is unnecessary
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
  if (sexp::list_length(m_sx) < 1)
  {
    raise_exception(std::runtime_error, "invalid syntax");
    return {};
  }
  else
  {
    return m_sx.get_car().as_string();
  }
}

ReaderMapping
SExprReaderObjectImpl::get_mapping() const
{
  return ReaderMapping(std::make_shared<SExprReaderMappingImpl>(m_sx.get_cdr()));
}


SExprReaderCollectionImpl::SExprReaderCollectionImpl(sexp::Value const& sx) :
  m_sx(sx)
{
}

SExprReaderCollectionImpl::~SExprReaderCollectionImpl()
{
}

std::vector<ReaderObject>
SExprReaderCollectionImpl::get_objects() const
{
  std::vector<ReaderObject> lst;
  for(auto const& cur : sexp::ListAdapter(m_sx))
  {
    lst.push_back(ReaderObject(std::make_shared<SExprReaderObjectImpl>(cur)));
  }
  return lst;
}


SExprReaderMappingImpl::SExprReaderMappingImpl(sexp::Value const& sx) :
  m_sx(sx)
{
  if (!sexp::is_list(m_sx))
  {
    std::ostringstream msg;
    msg << "SExprReaderMapping s-expression must be a list: " << sx;
    throw std::runtime_error(msg.str());
  }

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

  for(auto const& cur : sexp::ListAdapter(m_sx))
  {
    // assert if (cur.is_cons())
    lst.push_back(cur.get_car().as_string());
  }

  return lst;
}

bool
SExprReaderMappingImpl::read_int(const char* key, int& value) const
{
  sexp::Value const* item = get_subsection_item(key);
  if (item && item->is_integer())
  {
    value = item->as_int();
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
  sexp::Value const* item = get_subsection_item(key);
  if (item)
  {
    if (item->is_real())
    {
      value = item->as_float();
      return true;
    }
    else if (item->is_integer())
    {
      value = static_cast<float>(item->as_int());
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
  sexp::Value const* item = get_subsection_item(key);
  if (item && item->is_boolean())
  {
    value = item->as_bool();
    return true;
  }
  else if (item && item->is_integer())
  {
    value = item->as_int();
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
  sexp::Value const* sub = get_subsection(key);
  if (sub)
  {
    value = "";
    for(auto const& item : sexp::ListAdapter(*sub))
    {
      if (item.is_string())
      {
        value += item.as_string();
      }
      else if (item.is_symbol())
      {
        value += item.as_string();
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
  sexp::Value const* sub = get_subsection(key);
  if (sub && sexp::list_length(*sub) == 3)
  {
    value = Vector3f(sexp::list_ref(*sub, 0).as_float(),
                     sexp::list_ref(*sub, 1).as_float(),
                     sexp::list_ref(*sub, 2).as_float());
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
  sexp::Value const* sub_lst = get_subsection(key);
  if (sub_lst)
  {
    for(auto const& sub : sexp::ListAdapter(*sub_lst))
    {
      if (sexp::list_length(sub) == 3)
      {
        values.emplace_back(sexp::list_ref(sub, 0).as_float(),
                            sexp::list_ref(sub, 1).as_float(),
                            sexp::list_ref(sub, 2).as_float());
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
  sexp::Value const* sub = get_subsection(key);
  if (sub && sexp::list_length(*sub) == 2)
  {
    value.width  = sexp::list_ref(*sub, 0).as_int();
    value.height = sexp::list_ref(*sub, 1).as_int();
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
  sexp::Value const* sub = get_subsection(key);
  if (sub && sexp::list_length(*sub) == 2)
  {
    value.x = sexp::list_ref(*sub, 0).as_int();
    value.y = sexp::list_ref(*sub, 1).as_int();
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
  sexp::Value const* sub = get_subsection(key);
  if (sub && sexp::list_length(*sub) == 4)
  {
    rect.left   = sexp::list_ref(*sub, 0).as_int();
    rect.top    = sexp::list_ref(*sub, 1).as_int();
    rect.right  = sexp::list_ref(*sub, 2).as_int();
    rect.bottom = sexp::list_ref(*sub, 3).as_int();
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
  sexp::Value const* sub = get_subsection(key);
  if (sub && sexp::list_length(*sub) == 4)
  {
    value = Color(static_cast<uint8_t>(sexp::list_ref(*sub, 0).as_float() * 255),
                  static_cast<uint8_t>(sexp::list_ref(*sub, 1).as_float() * 255),
                  static_cast<uint8_t>(sexp::list_ref(*sub, 2).as_float() * 255),
                  static_cast<uint8_t>(sexp::list_ref(*sub, 3).as_float() * 255));
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
  sexp::Value const* sub = get_subsection(key);
  if (sub && sexp::list_length(*sub) == 4)
  {
    value = Color(static_cast<uint8_t>(sexp::list_ref(*sub, 0).as_int()),
                  static_cast<uint8_t>(sexp::list_ref(*sub, 1).as_int()),
                  static_cast<uint8_t>(sexp::list_ref(*sub, 2).as_int()),
                  static_cast<uint8_t>(sexp::list_ref(*sub, 3).as_int()));
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
  sexp::Value const* cur = get_subsection_item(key);
  if (cur)
  {
    value = ReaderObject(std::make_shared<SExprReaderObjectImpl>(*cur));
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
  sexp::Value const* cur = get_subsection(key);
  if (cur)
  {
    value = ReaderCollection(std::make_shared<SExprReaderCollectionImpl>(*cur));
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
  sexp::Value const* cur = get_subsection(key);
  if (cur)
  {
    value = ReaderMapping(std::make_shared<SExprReaderMappingImpl>(*cur));
    return true;
  }
  else
  {
    return false;
  }
}

sexp::Value const*
SExprReaderMappingImpl::get_subsection_item(const char* key) const
{
  sexp::Value const* sub = get_subsection(key);
  if (sub && sexp::list_length(*sub) >= 1)
  {
    if (sexp::list_length(*sub) > 1)
    {
      log_error("invalid items in section: %1%", key);
    }

    return &sub->get_car();
  }
  else
  {
    return nullptr;
  }
}

sexp::Value const*
SExprReaderMappingImpl::get_subsection(const char* key) const
{
  sexp::Value const* result = nullptr;

  int count = 0;
  for(auto const& cur : sexp::ListAdapter(m_sx))
  {
    if (cur.get_car().as_string() == key)
    {
      count += 1;
      result = &cur.get_cdr();
    }
  }

  if (count > 1)
  {
    log_error("duplicate key value '%1%'", key);
  }

  return result;
}

/* EOF */
