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

#ifndef HEADER_PRIO_READER_MAPPING_HPP
#define HEADER_PRIO_READER_MAPPING_HPP

#include <memory>
#include <sstream>
#include <vector>

namespace prio {

class ReaderCollection;
class ReaderDocument;
class ReaderMapping;
class ReaderMappingImpl;
class ReaderObject;

template<typename T>
bool read_custom(ReaderMapping const& map, std::string_view key, T& value)
{
  static_assert(!std::is_same<T, T>::value,
                "no read_custom() specialisation found for the given type");
  return false;
}

class ReaderMapping final
{
public:
  ReaderMapping();
  ReaderMapping(ReaderMapping&&) noexcept;
  ReaderMapping(std::unique_ptr<ReaderMappingImpl> impl);
  ~ReaderMapping();

  explicit operator bool() const { return static_cast<bool>(m_impl); }
  ReaderMappingImpl const& get_impl() const { return *m_impl; }

  ReaderMapping& operator=(ReaderMapping&&) noexcept;

  ReaderDocument const& get_document() const;
  std::vector<std::string> get_keys() const;

  // regular readers
  bool read(std::string_view key, bool& value) const;
  bool read(std::string_view key, int& value) const;
  bool read(std::string_view key, float& value) const;
  bool read(std::string_view key, std::string& value) const;

  bool read(std::string_view key, std::vector<bool>& value) const;
  bool read(std::string_view key, std::vector<int>& value) const;
  bool read(std::string_view key, std::vector<float>& value) const;
  bool read(std::string_view key, std::vector<std::string>& value) const;

  bool read(std::string_view key, ReaderMapping&) const;
  bool read(std::string_view key, ReaderCollection&) const;
  bool read(std::string_view key, ReaderObject&) const;

  template<typename Enum, typename String2Enum,
           typename std::enable_if_t<std::is_enum<Enum>::value, int> = 0>
  bool read(std::string_view key, Enum& value, String2Enum string2enum) const
  {
    std::string str;
    if (!read(key, str)) {
      return false;
    } else {
      // FIXME: add pedantic handling here
      value = string2enum(str);
      return true;
    }
  }

  template<typename T>
  bool read(std::string_view key, T& value) const {
    return read_custom<T>(*this, key, value);
  }

  // regular getters
  template<typename T>
  [[nodiscard]]
  T get(std::string_view key, T fallback = {}) const {
    read(key, fallback);
    return fallback;
  }

  template<typename Enum, typename String2Enum,
           typename std::enable_if_t<std::is_enum<Enum>::value, int> = 0>
  [[nodiscard]]
  Enum get(std::string_view key, String2Enum string2enum, Enum fallback = {}) const {
    read(key, fallback, string2enum);
    return fallback;
  }

  // must readers
  template<typename T>
  void must_read(std::string_view key, T& value) const {
    if (!read(key, value)) {
      missing_key_error(key);
    }
  }

  template<typename Enum, typename String2Enum,
           typename std::enable_if_t<std::is_enum<Enum>::value, int> = 0>
  void must_read(std::string_view key, Enum& value, String2Enum string2enum) const
  {
    if (!read(key, value, string2enum)) {
      missing_key_error(key);
    }
  }

  // must getters
  template<typename T>
  T must_get(std::string_view key) const {
    T value;
    if (!read(key, value)) {
      missing_key_error(key);
    }
    return value;
  }

  template<typename Enum, typename String2Enum,
           typename std::enable_if_t<std::is_enum<Enum>::value, int> = 0>
  Enum must_get(std::string_view key, String2Enum string2enum) const
  {
    Enum value{};
    if (!read(key, value, string2enum)) {
      missing_key_error(key);
    }
    return value;
  }

  /** report error with key that can be ignored */
  void error(std::string_view key, std::string_view message) const;

  /** report error with key that must not be ignored */
  void missing_key_error(std::string_view key) const;

private:
  std::unique_ptr<ReaderMappingImpl> m_impl;
};

} // namespace prio

#endif

/* EOF */
