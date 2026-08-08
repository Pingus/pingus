// prio - Property I/O for C++
// Copyright (C) 2005-2020 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "writer.hpp"

#include <assert.h>
#include <errno.h>
#include <string.h>

#include <fstream>
#include <sstream>
#include <stdexcept>

#ifdef PRIO_USE_JSONCPP
#  include "json_writer_impl.hpp"
#  include "jsonpretty_writer_impl.hpp"
#endif

#ifdef PRIO_USE_SEXPCPP
#  include "sexpr_writer_impl.hpp"
#endif

namespace prio {

Writer
Writer::from_file(Format format, std::filesystem::path const& filename)
{
  std::ofstream fout(filename);
  if (!fout) {
    std::ostringstream oss;
    oss << filename << ": failed to open for writing: " << strerror(errno);
    throw std::runtime_error(oss.str());
  }

  return from_stream(format, std::make_unique<std::ofstream>(std::move(fout)));
}

Writer
Writer::from_stream(Format format, std::unique_ptr<std::ostream> out)
{
  Writer writer = from_stream(format, *out);
  writer.m_owned = std::move(out);
  return writer;
}

Writer
Writer::from_stream(Format format, std::ostream& out)
{
  switch (format) {
#ifdef PRIO_USE_JSONCPP
#ifndef PRIO_USE_SEXPCPP
    case Format::AUTO:
#endif
    case Format::FASTJSON:
      return Writer(std::make_unique<JsonWriterImpl>(out));

    case Format::JSON:
      return Writer(std::make_unique<JsonPrettyWriterImpl>(out));
#endif

#ifdef PRIO_USE_SEXPCPP
    case Format::AUTO:
    case Format::SEXPR:
      return Writer(std::make_unique<SExprWriterImpl>(out));
#endif

    default:
      throw std::invalid_argument("invalid format");
  }
}

Writer
Writer::from_file(std::filesystem::path const& filename)
{
  return from_file(Format::AUTO, filename);
}

Writer
Writer::from_stream(std::ostream& out)
{
  return from_stream(Format::AUTO, out);
}

Writer
Writer::from_stream(std::unique_ptr<std::ostream> out)
{
  return from_stream(Format::AUTO, std::move(out));
}

#ifdef PRIO_USE_SEXPCPP
Writer::Writer(std::ostream& out) :
  m_impl(std::make_unique<SExprWriterImpl>(out)),
  m_owned()
{
}
#elif PRIO_USE_JSONCPP
Writer::Writer(std::ostream& out) :
  m_impl(std::make_unique<JsonPrettyWriterImpl>(out)),
  m_owned()
{
}
#else
#  error "No syntax library available"
#endif

Writer::Writer(std::unique_ptr<WriterImpl> impl) :
  m_impl(std::move(impl)),
  m_owned()
{
}

Writer::~Writer()
{
}

Writer&
Writer::write_comment(std::string_view text)
{
  return *this;
}

Writer&
Writer::begin_document(std::string_view type)
{
  assert(m_impl);
  m_impl->begin_object(type);
  return *this;
}

void
Writer::end_document()
{
  assert(m_impl);
  m_impl->end_object();

  // flush here
}

Writer&
Writer::begin_collection(std::string_view key)
{
  assert(m_impl);
  m_impl->begin_collection(key);
  return *this;
}

void
Writer::end_collection()
{
  assert(m_impl);
  m_impl->end_collection();
}

Writer&
Writer::begin_object(std::string_view type)
{
  assert(m_impl);
  m_impl->begin_object(type);
  return *this;
}

void
Writer::end_object()
{
  assert(m_impl);
  m_impl->end_object();
}

Writer&
Writer::begin_mapping(std::string_view key)
{
  assert(m_impl);
  m_impl->begin_mapping(key);
  return *this;
}

void
Writer::end_mapping()
{
  assert(m_impl);
  m_impl->end_mapping();
}

Writer&
Writer::begin_keyvalue(std::string_view key)
{
  assert(m_impl);
  m_impl->begin_keyvalue(key);
  return *this;
}

void
Writer::end_keyvalue()
{
  assert(m_impl);
  m_impl->end_keyvalue();
}

Writer&
Writer::write(std::string_view key, bool value)
{
  assert(m_impl);
  m_impl->write(key, value);
  return *this;
}

Writer&
Writer::write(std::string_view key, int value)
{
  assert(m_impl);
  m_impl->write(key, value);
  return *this;
}

Writer&
Writer::write(std::string_view key, float value)
{
  assert(m_impl);
  m_impl->write(key, value);
  return *this;
}

Writer&
Writer::write(std::string_view key, char const* value)
{
  assert(m_impl);
  m_impl->write(key, value);
  return *this;
}

Writer&
Writer::write(std::string_view key, std::string_view value)
{
  assert(m_impl);
  m_impl->write(key, value);
  return *this;
}

Writer&
Writer::write(std::string_view key, std::string const& value)
{
  assert(m_impl);
  m_impl->write(key, std::string_view(value));
  return *this;
}

Writer&
Writer::write(std::string_view key, std::span<bool const> values)
{
  assert(m_impl);
  m_impl->write(key, values);
  return *this;
}

Writer&
Writer::write(std::string_view key, std::span<int const> values)
{
  assert(m_impl);
  m_impl->write(key, values);
  return *this;
}

Writer&
Writer::write(std::string_view key, std::span<float const> values)
{
  assert(m_impl);
  m_impl->write(key, values);
  return *this;
}

Writer&
Writer::write(std::string_view key, std::span<std::string const> values)
{
  assert(m_impl);
  m_impl->write(key, values);
  return *this;
}

Writer&
Writer::write(std::string_view key, std::vector<bool> const& values)
{
  assert(m_impl);
  m_impl->write(key, values);
  return *this;
}

Writer&
Writer::write(std::string_view key, std::vector<int> const& values)
{
  assert(m_impl);
  m_impl->write(key, std::span(values));
  return *this;
}

Writer&
Writer::write(std::string_view key, std::vector<float> const& values)
{
  assert(m_impl);
  m_impl->write(key, std::span(values));
  return *this;
}

Writer&
Writer::write(std::string_view key, std::vector<std::string> const& values)
{
  assert(m_impl);
  m_impl->write(key, values);
  return *this;
}

} // namespace prio

/* EOF */
