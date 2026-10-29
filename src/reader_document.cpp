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

#include <exception>
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
#include <cctype>
#include <cstring>
#include <format>
#include "format_util.hpp"

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
    throw ReaderError(std::format("{}: failed to open: {}", stream_str(filename), strerror(errno)));
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
        throw ReaderError(std::format("json parse error: {}", errs));
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
        std::throw_with_nested(ReaderError(std::format("{}: ReaderDocument::from_stream() failed", filename ?  *filename : "<unknown>")));
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

namespace {

#ifdef PRIO_USE_JSONCPP
/** Return a pointer just past one complete JSON value starting at @a p.
    Supports objects, arrays, strings, numbers and keywords. Returns nullptr
    if the value is incomplete or malformed. */
char const*
json_value_end(char const* p, char const* end)
{
  auto skip_string = [&](char const* s) -> char const* {
    // s points at the opening quote
    ++s;
    while (s < end) {
      if (*s == '\\') {
        ++s;
        if (s >= end) return nullptr;
        ++s;
        continue;
      }
      if (*s == '"') return s + 1;
      ++s;
    }
    return nullptr;
  };

  if (p >= end) return p;

  if (*p == '"') {
    return skip_string(p);
  }

  if (*p == '{' || *p == '[') {
    char const open = *p;
    char const close = (open == '{') ? '}' : ']';
    int depth = 0;
    char const* s = p;
    while (s < end) {
      if (*s == '"') {
        s = skip_string(s);
        if (!s) return nullptr;
        continue;
      }
      if (*s == open) {
        ++depth;
      } else if (*s == close) {
        --depth;
        if (depth == 0) return s + 1;
      }
      ++s;
    }
    return nullptr;
  }

  // number
  if (*p == '-' || (*p >= '0' && *p <= '9')) {
    char const* s = p + 1;
    while (s < end
           && !std::isspace(static_cast<unsigned char>(*s))
           && *s != ',' && *s != '}' && *s != ']' && *s != ':') {
      ++s;
    }
    return s;
  }

  // keywords
  if (p + 4 <= end && std::strncmp(p, "true", 4) == 0) return p + 4;
  if (p + 5 <= end && std::strncmp(p, "false", 5) == 0) return p + 5;
  if (p + 4 <= end && std::strncmp(p, "null", 4) == 0) return p + 4;

  return nullptr;
}

/** Parse successive top-level JSON values from @a content.
    Values may be separated by any whitespace (JSON Lines is the
    newline-separated special case; compact concatenation without
    newlines is also accepted). */
std::vector<Json::Value>
parse_json_values_many(std::string const& content)
{
  std::vector<Json::Value> values;
  char const* p = content.data();
  char const* const end = p + content.size();

  Json::CharReaderBuilder builder;
  std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

  while (true) {
    while (p < end && std::isspace(static_cast<unsigned char>(*p))) {
      ++p;
    }
    if (p >= end) {
      break;
    }

    char const* const value_end = json_value_end(p, end);
    if (!value_end) {
      throw ReaderError("json parse error: incomplete or invalid JSON value");
    }

    Json::Value root;
    std::string errs;
    if (!reader->parse(p, value_end, &root, &errs)) {
      throw ReaderError(std::format("json parse error: {}", errs));
    }
    values.push_back(std::move(root));
    p = value_end;
  }

  return values;
}
#endif

} // namespace

std::vector<ReaderDocument>
ReaderDocument::parse_many(const std::string& pathname)
{
  std::ifstream fin(pathname);
  if (!fin) {
    throw ReaderError(std::format("{}: failed to open: {}", pathname, strerror(errno)));
  }

  // Peek at the first non-whitespace character to choose a backend when both
  // are available. '{' / '[' => JSON; otherwise treat as sexpr.
  int c = fin.get();
  while (c != EOF && std::isspace(static_cast<unsigned char>(c))) {
    c = fin.get();
  }
  if (c == EOF) {
    return {};
  }
  fin.unget();

  bool const looks_json = (c == '{' || c == '[');

#ifdef PRIO_USE_JSONCPP
  if (looks_json) {
    try {
      std::string content(
          (std::istreambuf_iterator<char>(fin)),
          std::istreambuf_iterator<char>());
      auto values = parse_json_values_many(content);
      std::vector<ReaderDocument> docs;
      docs.reserve(values.size());
      for (auto& value : values) {
        docs.emplace_back(std::make_unique<JsonReaderDocumentImpl>(
            std::move(value), ErrorHandler::THROW, pathname));
      }
      return docs;
    } catch (std::exception const&) {
      std::throw_with_nested(ReaderError(std::format(
          "{}: ReaderDocument::parse_many() failed", pathname)));
    }
  }
#endif

#ifdef PRIO_USE_SEXPCPP
  if (!looks_json) {
    try {
      auto values = sexp::Parser::from_stream_many(fin, sexp::Parser::USE_ARRAYS);
      std::vector<ReaderDocument> docs;
      docs.reserve(values.size());
      for (auto& sx : values) {
        docs.emplace_back(std::make_unique<SExprReaderDocumentImpl>(
            std::move(sx), ErrorHandler::THROW, pathname));
      }
      return docs;
    } catch (std::exception const&) {
      std::throw_with_nested(ReaderError(std::format(
          "{}: ReaderDocument::parse_many() failed", pathname)));
    }
  }
#endif

  throw ReaderError(std::format(
      "{}: parse_many() has no suitable backend for this content", pathname));
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
    return "/";
  }

  // Pure string dirname — do not use std::filesystem::path::parent_path().
  // On R36S (GCC 15 headers + _GLIBCXX_USE_CXX11_ABI=0 + ArkOS libstdc++),
  // path::parent_path can fail to strip the filename, producing broken joins
  // like "images/engine/menu/mousecursor.sprite/mousecursor.png".
  // get_filename() returns optional by value — copy, do not bind a reference
  // to the temporary (dangling → segfault in tests / R36S).
  std::string const filename = *m_impl->get_filename();
  auto p = filename.find_last_of("/\\");
  if (p == std::string::npos) {
    return ".";
  }
  return filename.substr(0, p);
}

} // namespace prio

/* EOF */
