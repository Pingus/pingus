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

#include <gtest/gtest.h>

#include <fstream>

#include <prio/reader_collection.hpp>
#include <prio/reader_document.hpp>
#include <prio/reader_error.hpp>
#include <prio/reader_mapping.hpp>

using namespace prio;

TEST(ReaderTest, construction)
{
  ReaderDocument my_doc;
  ReaderMapping my_mapping;
  ReaderObject my_object;
  ReaderCollection my_collection;

  my_doc = ReaderDocument();
  my_mapping = ReaderMapping();
  my_object = ReaderObject();
  my_collection = ReaderCollection();
}

/* EOF */
