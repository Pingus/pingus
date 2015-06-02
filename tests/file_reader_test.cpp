// Pingus - A free Lemmings clone
// Copyright (C) 1998-2015 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <gtest/gtest.h>

#include "util/file_reader.hpp"

TEST(FileReaderTest, reading)
{
  std::istringstream stream("(test-document\n"
                            "  (intvalue 5)\n"
                            "  (submap (int 7) (float 9.9))\n"
                            "  (floatvalue 5.5))");

  ReaderObject obj = FileReader::parse(stream);
  EXPECT_EQ("test-document", obj.get_name());
  ReaderMapping mapping = obj.get_mapping();
  {
    int intvalue;
    ASSERT_TRUE(mapping.read_int("intvalue", intvalue));
    EXPECT_EQ(5, intvalue);
  }
  {
    float floatvalue;
    ASSERT_TRUE(mapping.read_float("floatvalue", floatvalue));
    EXPECT_EQ(5.5f, floatvalue);
  }
  {
    ReaderMapping submap;
    ASSERT_TRUE(mapping.read_mapping("submap", submap));
    {
      int intvalue;
      ASSERT_TRUE(submap.read_int("int", intvalue));
      EXPECT_EQ(7, intvalue);
    }
    {
      float floatvalue;
      ASSERT_TRUE(submap.read_float("float", floatvalue));
      EXPECT_EQ(9.9f, floatvalue);
    }
  }
}

/* EOF */
