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

#ifndef HEADER_PRIO_JSON_FILE_WRITER_HPP
#define HEADER_PRIO_JSON_FILE_WRITER_HPP

#include <functional>
#include <iosfwd>
#include <json/json.h>
#include <stack>
#include <vector>

#include "writer_impl.hpp"

namespace prio {

class JsonWriterImpl final : public WriterImpl
{
private:
  std::ostream& m_out;

  std::stack<Json::Value> m_stack;

public:
  JsonWriterImpl(std::ostream& out);
  ~JsonWriterImpl() override;

  void begin_collection(std::string_view key) override;
  void end_collection() override;

  void begin_object(std::string_view type) override;
  void end_object() override;

  void begin_mapping(std::string_view key) override;
  void end_mapping() override;

  void begin_keyvalue(std::string_view key) override;
  void end_keyvalue() override;

  void write(std::string_view key, bool value) override;
  void write(std::string_view key, int value) override;
  void write(std::string_view key, float value) override;
  void write(std::string_view key, char const* value) override;
  void write(std::string_view key, std::string_view value) override;

  void write(std::string_view key, std::span<bool const> values) override;
  void write(std::string_view key, std::span<int const> values) override;
  void write(std::string_view key, std::span<float const> values) override;
  void write(std::string_view key, std::span<std::string const> values) override;

  void write(std::string_view key, std::vector<bool> const& values) override;

private:
  void flush();

private:
  JsonWriterImpl(const JsonWriterImpl&) = delete;
  JsonWriterImpl& operator=(const JsonWriterImpl&) = delete;
};

} // namespace prio

#endif

/* EOF */
