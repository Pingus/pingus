// Pingus - A free Lemmings clone
// Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
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

#include "util/reader.hpp"

#include <fstream>
#include <json/reader.h>
#include <sexp/parser.hpp>

#include "pingus/res_descriptor.hpp"
#include "util/reader_impl.hpp"
#include "util/pathname.hpp"
#include "util/sexpr_reader_impl.hpp"
#include "util/json_reader_impl.hpp"
#include "util/log.hpp"

ReaderMapping::ReaderMapping(std::shared_ptr<ReaderMappingImpl> impl_) :
  m_impl(impl_)
{
}

ReaderMapping::ReaderMapping() :
  m_impl()
{
}

ReaderCollection::ReaderCollection(std::shared_ptr<ReaderCollectionImpl> impl) :
  m_impl(impl)
{
}

ReaderCollection::ReaderCollection() :
  m_impl()
{
}

std::vector<ReaderObject>
ReaderCollection::get_objects() const
{
  if (m_impl)
    return m_impl->get_objects();
  else
    return {};
}

ReaderObject::ReaderObject(std::shared_ptr<ReaderObjectImpl> impl) :
  m_impl(impl)
{
}

ReaderObject::ReaderObject() :
  m_impl()
{
}

std::string
ReaderObject::get_name() const
{
  if (m_impl)
    return m_impl->get_name();
  else
    return {};
}

ReaderMapping
ReaderObject::get_mapping() const
{
  if (m_impl)
    return m_impl->get_mapping();
  else
    return {};
}

bool
ReaderMapping::read_int(const char* key, int& value) const
{
  if (m_impl)
    return m_impl->read_int(key, value);
  else
    return false;
}

bool
ReaderMapping::read_float (const char* key, float& value) const
{
  if (m_impl)
    return m_impl->read_float(key, value);
  else
    return false;
}

bool
ReaderMapping::read_bool  (const char* key, bool& value) const
{
  if (m_impl)
    return m_impl->read_bool(key, value);
  else
    return false;
}

bool
ReaderMapping::read_string(const char* key, std::string& value) const
{
  if (m_impl)
    return m_impl->read_string(key, value);
  else
    return false;
}

bool
ReaderMapping::read_path(const char* key, Pathname& value) const
{
  if (m_impl)
  {
    std::string filename;
    if (m_impl->read_string(key, filename))
    {
      value = Pathname(filename, Pathname::DATA_PATH);
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
ReaderMapping::read_vector(const char* key, Vector3f& value) const
{
  if (m_impl)
    return m_impl->read_vector(key, value);
  else
    return false;
}

bool
ReaderMapping::read_vectors(const char* key, std::vector<Vector3f>& value) const
{
  if (m_impl)
    return m_impl->read_vectors(key, value);
  else
    return false;
}

bool
ReaderMapping::read_colorf(const char* key, Color& value) const
{
  if (m_impl)
    return m_impl->read_colorf(key, value);
  else
    return false;
}

bool
ReaderMapping::read_colori(const char* key, Color& value) const
{
  if (m_impl)
    return m_impl->read_colori(key, value);
  else
    return false;
}

bool
ReaderMapping::read_desc(const char* key, ResDescriptor& value) const
{
  ReaderMapping reader;
  if (read_mapping(key, reader))
  {
    reader.read_string("image", value.res_name);
    reader.read_enum("modifier", value.modifier, &ResourceModifier::from_string);
    return true;
  }
  else
  {
    return false;
  }
}

bool
ReaderMapping::read_size  (const char* key, Size& value) const
{
  if (m_impl)
    return m_impl->read_size(key, value);
  else
    return false;
}

bool
ReaderMapping::read_vector2i(const char* key, Vector2i& value) const
{
  if (m_impl)
    return m_impl->read_vector2i(key, value);
  else
    return false;
}

bool
ReaderMapping::read_rect(const char* key, Rect& value)    const
{
  if (m_impl)
    return m_impl->read_rect(key, value);
  else
    return false;
}

bool
ReaderMapping::read_object(const char* key, ReaderObject& object) const
{
  if (m_impl)
    return m_impl->read_object(key, object);
  else
    return false;
}

bool
ReaderMapping::read_mapping(const char* key, ReaderMapping& mapping) const
{
  if (m_impl)
    return m_impl->read_mapping(key, mapping);
  else
    return false;
}

bool
ReaderMapping::read_collection(const char* key, ReaderCollection& collection) const
{
  if (m_impl)
    return m_impl->read_collection(key, collection);
  else
    return false;
}


ReaderMapping
ReaderMapping::read_mapping(const char* key) const
{
  if (!m_impl)
  {
    return {};
  }
  {
    ReaderMapping result;
    read_mapping(key, result);
    return result;
  }
}

ReaderCollection
ReaderMapping::read_collection(const char* key) const
{
  if (!m_impl)
  {
    return {};
  }
  {
    ReaderCollection result;
    read_collection(key, result);
    return result;
  }
}

ReaderObject
ReaderMapping::read_object(const char* key) const
{
  if (!m_impl)
  {
    return {};
  }
  {
    ReaderObject result;
    read_object(key, result);
    return result;
  }
}

std::vector<std::string>
ReaderMapping::get_keys() const
{
  if (m_impl)
    return m_impl->get_keys();
  else
    return {};
}

ReaderObject
Reader::parse(std::istream& stream)
{
  int c = stream.get();
  stream.unget();
  if (c == '{')
  {
    Json::Reader reader;
    Json::Value root;
    if (reader.parse(stream, root))
    {
      return ReaderObject(std::make_shared<JsonReaderObjectImpl>(root));
    }
    else
    {
      log_error("json parse error: %1%", reader.getFormattedErrorMessages());
      return ReaderObject();
    }
  }
  else
  {
    try
    {
      auto sx = sexp::Parser::from_stream(stream);
      return ReaderObject(std::make_shared<SExprReaderObjectImpl>(std::move(sx)));
    }
    catch(std::exception const& err)
    {
      log_error("sexp parse error: %1%", err.what());
      return ReaderObject();
    }
  }
}

ReaderObject
Reader::parse(const std::string& filename)
{
  std::ifstream fin(filename);
  if (!fin)
  {
    return ReaderObject();
  }
  else
  {
    return parse(fin);
  }
}

ReaderObject
Reader::parse(const Pathname& pathname)
{
  return parse(pathname.get_sys_path());
}

std::vector<ReaderObject>
Reader::parse_many(const Pathname& pathname)
{
  return {};
#if 0
  std::shared_ptr<lisp::Lisp> sexpr = lisp::Parser::parse(pathname.get_sys_path());
  if (sexpr)
  {
    std::vector<Reader> sections;
    for(size_t i = 0; i < sexpr->get_list_size(); ++i)
    {
      sections.push_back(Reader(std::make_shared<SExprReaderImpl>(sexpr->get_list_elem(i))));
    }

    return sections;
  }
  else
  {
    return std::vector<Reader>();
  }
#endif
}

/* EOF */
