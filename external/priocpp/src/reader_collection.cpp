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

#include "reader_collection.hpp"

#include "reader_impl.hpp"
#include "reader_object.hpp"

namespace prio {

ReaderCollection::ReaderCollection(std::unique_ptr<ReaderCollectionImpl> impl) :
  m_impl(std::move(impl))
{
}

ReaderCollection::ReaderCollection(ReaderCollection&&) noexcept = default;

ReaderCollection::ReaderCollection() :
  m_impl()
{
}

ReaderCollection::~ReaderCollection()
{
}

ReaderCollection&
ReaderCollection::operator=(ReaderCollection&&) noexcept = default;

ReaderDocument const&
ReaderCollection::get_document() const
{
  return m_impl->get_document().get_parent();
}

std::vector<ReaderObject>
ReaderCollection::get_objects() const
{
  if (!m_impl) { return {}; }

  return m_impl->get_objects();
}

} // namespace prio

/* EOF */
