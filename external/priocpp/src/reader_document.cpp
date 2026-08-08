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

#include "reader_document.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <utility>

#include <logmich/log.hpp>

#ifdef PRIO_USE_JSONCPP
#  include <json/reader.h>
#  include "json_reader_impl.hpp"
#endif

#ifdef PRIO_USE_SEXPCPP
#  include <sexp/parser.hpp>
#  include "sexpr_reader_impl.hpp"
#endif

#include "reader_collection.hpp"
#include "reader_error.hpp"
#include "reader_impl.hpp"
#include "reader_mapping.hpp"
#include "reader_object.hpp"

namespace prio {

ReaderDocument
ReaderDocument::from_string(Format format,
                            std::string_view text, ErrorHandler error_handler,
                            std::optional<std::string> const& filename)
{
  std::istringstream in{std::string(text)};
  return ReaderDocument::from_stream(format, in, error_handler, filename);
}

ReaderDocument
ReaderDocument::from_file(Format format,
                          std::filesystem::path const& filename, ErrorHandler error_handler)
{
  std::ifstream fin(filename);
  if (!fin) {
    throw ReaderError(fmt::format("{}: failed to open: {}", fmt::streamed(filename), strerror(errno)));
  } else {
    return from_stream(format, fin, error_handler, filename.string());
  }
}

ReaderDocument
ReaderDocument::from_stream(Format format,
                            std::istream& stream, ErrorHandler error_handler,
                            std::optional<std::string> const& filename)
{
  switch (format)
  {
    case Format::AUTO: {
      int c = stream.get();
      stream.unget();
      if (c == '{') {
        return from_stream(Format::JSON, stream, error_handler, filename);
      } else {
        return from_stream(Format::SEXPR, stream, error_handler, filename);
      }
    }

#ifdef PRIO_USE_JSONCPP
    case Format::FASTJSON:
    case Format::JSON: {
      Json::CharReaderBuilder builder;
      std::string errs;
      Json::Value root;
      if (!Json::parseFromStream(builder, stream, &root, &errs)) {
        throw ReaderError(fmt::format("json parse error: {}", errs));
      }
      return ReaderDocument(std::make_unique<JsonReaderDocumentImpl>(std::move(root), error_handler, filename));
    }
#endif

#ifdef PRIO_USE_SEXPCPP
    case Format::SEXPR: {
      try {
        auto sx = sexp::Parser::from_stream(stream, sexp::Parser::USE_ARRAYS);
        return ReaderDocument(std::make_unique<SExprReaderDocumentImpl>(std::move(sx), error_handler, filename));
      } catch(std::exception const& err) {
        std::throw_with_nested(ReaderError(fmt::format("{}: ReaderDocument::from_stream() failed", filename ?  *filename : "<unknown>")));
      }
    }
#endif

    default:
      throw std::invalid_argument("unknown format");
  }
}

ReaderDocument
ReaderDocument::from_string(std::string_view text, ErrorHandler error_handler,
                            std::optional<std::string> const& filename)
{
  return from_string(Format::AUTO, text, error_handler, filename);
}

ReaderDocument
ReaderDocument::from_stream(std::istream& stream, ErrorHandler error_handler,
                            std::optional<std::string> const& filename)
{
  return from_stream(Format::AUTO, stream, error_handler, filename);
}

ReaderDocument
ReaderDocument::from_file(std::filesystem::path const& filename, ErrorHandler error_handler)
{
  return from_file(Format::AUTO, filename, error_handler);
}

std::vector<ReaderDocument>
ReaderDocument::parse_many(const std::string& pathname)
{
#if 0
  std::shared_ptr<lisp::Lisp> sexpr = lisp::Parser::parse(pathname.get_sys_path());
  if (sexpr)
  {
    std::vector<Reader> sections;
    for(size_t i = 0; i < sexpr->get_list_size(); ++i)
    {
      sections.push_back(Reader(std::make_unique<SExprReaderImpl>(sexpr->get_list_elem(i))));
    }

    return sections;
  }
  else
#endif
  {
    return std::vector<ReaderDocument>();
  }
}

ReaderDocument:: ReaderDocument() :
  m_impl()
{
}

ReaderDocument::ReaderDocument(std::unique_ptr<ReaderDocumentImpl> impl) :
  m_impl(std::move(impl))
{
  m_impl->set_parent(this);
}

ReaderDocument::ReaderDocument(ReaderDocument&& other) noexcept :
  m_impl(std::move(other.m_impl))
{
  m_impl->set_parent(this);
}

ReaderDocument::~ReaderDocument()
{
}

ReaderDocument&
ReaderDocument::operator=(ReaderDocument&&) noexcept = default;

ReaderObject
ReaderDocument::get_root() const
{
  if (!m_impl) { return {}; }

  return m_impl->get_root();
}

std::string
ReaderDocument::get_name() const
{
  if (!m_impl) { return {}; }

  return m_impl->get_root().get_name();
}

ReaderMapping
ReaderDocument::get_mapping() const
{
  if (!m_impl) { return {}; }

  return m_impl->get_root().get_mapping();
}

std::string
ReaderDocument::get_filename() const
{
  if (!m_impl) { return {}; }

  if (!m_impl->get_filename()) {
    return "<unknown>";
  } else {
    return *m_impl->get_filename();
  }
}

std::string
ReaderDocument::get_directory() const
{
  if (!m_impl) { return {}; }

  if (!m_impl->get_filename()) {
    return std::filesystem::path("/").string();
  } else {
    return std::filesystem::path(*m_impl->get_filename()).parent_path().string();
  }
}

} // namespace prio

/* EOF */
