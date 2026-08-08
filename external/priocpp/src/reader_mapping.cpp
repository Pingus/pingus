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

#include "reader_mapping.hpp"

#include "reader_collection.hpp"
#include "reader_object.hpp"
#include "reader_impl.hpp"

namespace prio {

ReaderMapping::ReaderMapping(std::unique_ptr<ReaderMappingImpl> impl) :
  m_impl(std::move(impl))
{
}

ReaderMapping::ReaderMapping(ReaderMapping&&) noexcept = default;

ReaderMapping::ReaderMapping() :
  m_impl()
{
}

ReaderMapping::~ReaderMapping()
{
}

ReaderMapping&
ReaderMapping::operator=(ReaderMapping&&) noexcept = default;

ReaderDocument const&
ReaderMapping::get_document() const
{
  return m_impl->get_document().get_parent();
}

bool
ReaderMapping::read(std::string_view key, bool& value) const
{
  if (!m_impl) { return false; }

  return m_impl->read(key, value);
}

bool
ReaderMapping::read(std::string_view key, int& value) const
{
  if (!m_impl) { return false; }

  return m_impl->read(key, value);
}

bool
ReaderMapping::read (std::string_view key, float& value) const
{
  if (!m_impl) { return false; }

  return m_impl->read(key, value);
}

bool
ReaderMapping::read(std::string_view key, std::string& value) const
{
  if (!m_impl) { return false; }

  return m_impl->read(key, value);
}

bool
ReaderMapping::read(std::string_view key, std::vector<bool>& values) const
{
 if (!m_impl) { return false; }

 return m_impl->read(key, values);
}

bool
ReaderMapping::read(std::string_view key, std::vector<int>& values) const
{
  if (!m_impl) { return false; }

  return m_impl->read(key, values);
}

bool
ReaderMapping::read(std::string_view key, std::vector<float>& values) const
{
  if (!m_impl) { return false; }

  return m_impl->read(key, values);
}

bool
ReaderMapping::read(std::string_view key, std::vector<std::string>& values) const
{
  if (!m_impl) { return false; }

  return m_impl->read(key, values);
}

bool
ReaderMapping::read(std::string_view key, ReaderObject& object) const
{
  if (!m_impl) { return false; }

  return m_impl->read(key, object);
}

bool
ReaderMapping::read(std::string_view key, ReaderMapping& mapping) const
{
  if (!m_impl) { return false; }

  return m_impl->read(key, mapping);
}

bool
ReaderMapping::read(std::string_view key, ReaderCollection& collection) const
{
  if (!m_impl) { return false; }

  return m_impl->read(key, collection);
}

std::vector<std::string>
ReaderMapping::get_keys() const
{
  if (!m_impl) { return {}; }

  return m_impl->get_keys();
}

void
ReaderMapping::error(std::string_view key, std::string_view message) const
{
  if (!m_impl) { return; }

  m_impl->error(key, message);
}

void
ReaderMapping::missing_key_error(std::string_view key) const
{
  if (!m_impl) { return; }

  m_impl->missing_key_error(key);
}

} // namespace prio

/* EOF */
