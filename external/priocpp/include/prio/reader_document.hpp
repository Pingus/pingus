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

#ifndef HEADER_PRIO_READER_DOCUMENT_HPP
#define HEADER_PRIO_READER_DOCUMENT_HPP

#include <assert.h>
#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "error_handler.hpp"
#include "format.hpp"
#include "reader_object.hpp"

namespace prio {

class ReaderObject;
class ReaderMapping;
class ReaderCollection;

class ReaderObjectImpl;
class ReaderMappingImpl;
class ReaderCollectionImpl;
class ReaderDocumentImpl;

class ReaderDocument final
{
public:
  static ReaderDocument from_file(Format format,
                                  std::filesystem::path const& filename,
                                  ErrorHandler error_handler = ErrorHandler::THROW);
  static ReaderDocument from_string(Format format,
                                    std::string_view text,
                                    ErrorHandler error_handler = ErrorHandler::THROW,
                                    std::optional<std::string> const& filename = {});
  static ReaderDocument from_stream(Format format,
                                    std::istream& stream,
                                    ErrorHandler error_handler = ErrorHandler::THROW,
                                    std::optional<std::string> const& filename = {});

  static ReaderDocument from_file(std::filesystem::path const& filename,
                                  ErrorHandler error_handler = ErrorHandler::THROW);
  static ReaderDocument from_string(std::string_view text,
                                    ErrorHandler error_handler = ErrorHandler::THROW,
                                    std::optional<std::string> const& filename = {});
  static ReaderDocument from_stream(std::istream& stream,
                                    ErrorHandler error_handler = ErrorHandler::THROW,
                                    std::optional<std::string> const& filename = {});

  /** Reads multiple trees from a file, for use with files that don't
      contain a root element */
  static std::vector<ReaderDocument> parse_many(const std::string& pathname);

public:
  ReaderDocument();
  ReaderDocument(ReaderDocument&&) noexcept;
  ReaderDocument(std::unique_ptr<ReaderDocumentImpl> impl);
  ~ReaderDocument();

  ReaderDocument& operator=(ReaderDocument&&) noexcept;

  std::string get_name() const;
  ReaderMapping get_mapping() const;

  /** Returns the root object */
  ReaderObject get_root() const;

  /** Returns the filename of the document */
  std::string get_filename() const;

  /** Returns the directory of the document */
  std::string get_directory() const;

  explicit operator bool() const { return static_cast<bool>(m_impl); }

  ReaderDocumentImpl const& get_impl() const { assert(m_impl != nullptr); return *m_impl; }

private:
  std::unique_ptr<ReaderDocumentImpl> m_impl;
};

} // namespace prio

#endif

/* EOF */
