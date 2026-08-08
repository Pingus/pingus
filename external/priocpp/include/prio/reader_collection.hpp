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

#ifndef HEADER_PRIO_READER_COLLECTION_HPP
#define HEADER_PRIO_READER_COLLECTION_HPP

#include <memory>
#include <vector>

namespace prio {

class ReaderCollectionImpl;
class ReaderDocument;
class ReaderObject;

class ReaderCollection final
{
public:
  ReaderCollection();
  ReaderCollection(ReaderCollection&&) noexcept;
  ReaderCollection(std::unique_ptr<ReaderCollectionImpl> impl);
  ~ReaderCollection();

  explicit operator bool() const { return static_cast<bool>(m_impl); }
  ReaderCollectionImpl const& get_impl() const { return *m_impl; }

  ReaderDocument const& get_document() const;
  ReaderCollection& operator=(ReaderCollection&&) noexcept;

  std::vector<ReaderObject> get_objects() const;

private:
  std::unique_ptr<ReaderCollectionImpl> m_impl;
};

} // namespace prio

#endif

/* EOF */
