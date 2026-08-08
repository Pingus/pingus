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

#include "override_reader_mapping.hpp"

#include <set>

#include "reader_document.hpp"
#include "reader_mapping.hpp"
#include "reader_impl.hpp"

namespace prio {

class OverrideReaderMappingImpl : public ReaderMappingImpl
{
private:
  ReaderMapping const& m_reader;
  ReaderMapping const& m_overrides;

public:
  OverrideReaderMappingImpl(ReaderMapping const& reader,
                            ReaderMapping const& overrides) :
    m_reader(reader),
    m_overrides(overrides)
  {
  }

  ReaderDocumentImpl const& get_document() const override
  {
    return m_reader.get_document().get_impl();
  }

  std::vector<std::string> get_keys() const override
  {
    std::set<std::string> result;

    auto lst = m_reader.get_keys();
    result.insert(lst.begin(), lst.end());
    lst = m_overrides.get_keys();
    result.insert(lst.begin(), lst.end());

    return std::vector<std::string>(result.begin(), result.end());
  }

  bool read(std::string_view key, bool& v) const override
  {
    return m_overrides.read(key, v) || m_reader.read(key, v);
  }

  bool read(std::string_view key, int& v) const override
  {
    return m_overrides.read(key, v) || m_reader.read(key, v);
  }

  bool read(std::string_view key, float& v) const override
  {
    return m_overrides.read(key, v) || m_reader.read(key, v);
  }

  bool read(std::string_view key, std::string& v) const override
  {
    return m_overrides.read(key, v) || m_reader.read(key, v);
  }

  bool read(std::string_view key, std::vector<bool>& v) const override
  {
    return m_overrides.read(key, v) || m_reader.read(key, v);
  }

  bool read(std::string_view key, std::vector<int>& v) const override
  {
    return m_overrides.read(key, v) || m_reader.read(key, v);
  }

  bool read(std::string_view key, std::vector<float>& v) const override
  {
    return m_overrides.read(key, v) || m_reader.read(key, v);
  }

  bool read(std::string_view key, std::vector<std::string>& v) const override
  {
    return m_overrides.read(key, v) || m_reader.read(key, v);
  }

  bool read(std::string_view key, ReaderMapping& result) const override
  {
    ReaderMapping overwrite_result;
    if (m_overrides.read(key, overwrite_result))
    {
      if (m_reader.read(key, result))
      {
        result = make_override_mapping(result, overwrite_result);
        return true;
      }
      else
      {
        result = std::move(overwrite_result);
        return true;
      }
    }
    else
    {
      return m_reader.read(key, result);
    }
  }

  bool read(std::string_view key, ReaderCollection& result) const override
  {
    return m_overrides.read(key, result) || m_reader.read(key, result);
  }

  bool read(std::string_view key, ReaderObject& result) const override
  {
    return m_overrides.read(key, result) || m_reader.read(key, result);
  }

  void error(std::string_view key, std::string_view message) const override
  {
    m_reader.error(key, message);
  }

  void missing_key_error(std::string_view key) const override
  {
    m_reader.missing_key_error(key);
  }
};

ReaderMapping
make_override_mapping(const ReaderMapping& reader, const ReaderMapping& overrides)
{
  return ReaderMapping(std::make_unique<OverrideReaderMappingImpl>(reader, overrides));
}

} // namespace prio

/* EOF */
