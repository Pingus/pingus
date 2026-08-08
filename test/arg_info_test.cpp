// UITest - Simple testing framework for interactive tests
// Copyright (C) 2015 Ingo Ruhnke <grumbel@gmail.com>
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

#include <uitest/arg_info.hpp>

TEST(ArgInfoTest, from_string)
{
  {
    auto args = uitesting::ArgInfo::from_string("FILE");
    EXPECT_EQ(1, args.required());
    EXPECT_EQ(0, args.optional());
    EXPECT_EQ(0, args.rest());
  }

  {
    auto args = uitesting::ArgInfo::from_string("URL...");
    EXPECT_EQ(0, args.required());
    EXPECT_EQ(0, args.optional());
    EXPECT_EQ(2, args.rest());
  }

  {
    auto args = uitesting::ArgInfo::from_string("FILE FILE [FILE] [FILE] [FILE]");
    EXPECT_EQ(2, args.required());
    EXPECT_EQ(3, args.optional());
    EXPECT_EQ(0, args.rest());
  }

  {
    auto args = uitesting::ArgInfo::from_string("FILE [FILE] [FILE]...");
    EXPECT_EQ(1, args.required());
    EXPECT_EQ(1, args.optional());
    EXPECT_EQ(1, args.rest());
  }

  {
    auto args = uitesting::ArgInfo::from_string("FILE FILE FILE...");
    EXPECT_EQ(2, args.required());
    EXPECT_EQ(0, args.optional());
    EXPECT_EQ(2, args.rest());
  }
}

/* EOF */
