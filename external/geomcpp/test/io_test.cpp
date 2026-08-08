// geomcpp - Basic collection of point, size and rect classes
// Copyright (C) 2020 Ingo Ruhnke <grumbel@gmail.com>
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include <gtest/gtest.h>
#include <sstream>

#include <geom/geom.hpp>

using namespace geom;

TEST(io_test, ipoint)
{
  std::ostringstream out;
  out << ipoint(1, 2);
  EXPECT_EQ(out.str(), "ipoint(1, 2)");
}

TEST(io_test, isize)
{
  std::ostringstream out;
  out << isize(1, 2);
  EXPECT_EQ(out.str(), "isize(1, 2)");
}

TEST(io_test, irect)
{
  std::ostringstream out;
  out << irect(1, 2, 3, 4);
  EXPECT_EQ(out.str(), "irect(1, 2, 3, 4)");
}

TEST(io_test, fpoint)
{
  std::ostringstream out;
  out << fpoint(1, 2);
  EXPECT_EQ(out.str(), "fpoint(1, 2)");
}

TEST(io_test, fsize)
{
  std::ostringstream out;
  out << fsize(1, 2);
  EXPECT_EQ(out.str(), "fsize(1, 2)");
}

TEST(io_test, frect)
{
  std::ostringstream out;
  out << frect(1, 2, 3, 4);
  EXPECT_EQ(out.str(), "frect(1, 2, 3, 4)");
}

TEST(io_test, origin)
{
  std::ostringstream out;
  out << origin::TOP_LEFT;
  EXPECT_EQ(out.str(), "top-left");
}

TEST(io_test, ipoint_from_string)
{
  EXPECT_EQ(ipoint(123, 456), ipoint_from_string("ipoint(123, 456)"));
  EXPECT_EQ(ipoint(123, 456), ipoint_from_string("123, 456"));
  EXPECT_EQ(ipoint(123, 456), ipoint_from_string("123,456"));
  EXPECT_EQ(ipoint(123, 456), ipoint_from_string(" 123,456 "));
  EXPECT_THROW({ipoint_from_string("AxB");}, std::invalid_argument);
  EXPECT_THROW({ipoint_from_string("1,2,3");}, std::invalid_argument);
}

TEST(io_test, fpoint_from_string)
{
  EXPECT_EQ(fpoint(123, 456), fpoint_from_string("fpoint(123, 456)"));
  EXPECT_EQ(fpoint(123, 456), fpoint_from_string("123, 456"));
  EXPECT_EQ(fpoint(123, 456), fpoint_from_string("123,456"));
  EXPECT_EQ(fpoint(123, 456), fpoint_from_string(" 123,456 "));
  EXPECT_THROW({fpoint_from_string("AxB");}, std::invalid_argument);
  EXPECT_THROW({fpoint_from_string("1,2,3");}, std::invalid_argument);
}

TEST(io_test, isize_from_string)
{
  EXPECT_EQ(isize(123, 456), isize_from_string("isize(123, 456)"));
  EXPECT_EQ(isize(123, 456), isize_from_string("123, 456"));
  EXPECT_EQ(isize(123, 456), isize_from_string("123,456"));
  EXPECT_EQ(isize(123, 456), isize_from_string(" 123,456 "));
  EXPECT_EQ(isize(123, 456), isize_from_string("123x456"));
  EXPECT_EQ(isize(123, 456), isize_from_string(" 123x456 "));
  EXPECT_THROW({isize_from_string("AxB");}, std::invalid_argument);
  EXPECT_THROW({isize_from_string("1,2,3");}, std::invalid_argument);
}

TEST(io_test, fsize_from_string)
{
  EXPECT_EQ(fsize(123, 456), fsize_from_string("fsize(123, 456)"));
  EXPECT_EQ(fsize(123, 456), fsize_from_string("123, 456"));
  EXPECT_EQ(fsize(123, 456), fsize_from_string("123,456"));
  EXPECT_EQ(fsize(123, 456), fsize_from_string(" 123,456 "));
  EXPECT_EQ(fsize(123, 456), fsize_from_string("123x456"));
  EXPECT_EQ(fsize(123, 456), fsize_from_string(" 123x456 "));
  EXPECT_THROW({fsize_from_string("AxB");}, std::invalid_argument);
  EXPECT_THROW({fsize_from_string("1,2,3");}, std::invalid_argument);
}

TEST(io_test, irect_from_string)
{
  EXPECT_EQ(irect(4, 5, 123, 456), irect_from_string("irect(4, 5, 123, 456)"));
  EXPECT_EQ(irect(4, 5, 123, 456), irect_from_string("4, 5, 123, 456"));
  EXPECT_EQ(irect(4, 5, 123, 456), irect_from_string("4,5,123,456"));
  EXPECT_EQ(irect(4, 5, 123, 456), irect_from_string(" 4,5,123,456 "));
  EXPECT_EQ(irect(isize(123, 456)), irect_from_string("123x456"));
  EXPECT_EQ(irect(isize(123, 456)), irect_from_string(" 123x456 "));
  EXPECT_EQ(irect(ipoint(4, 5), isize(123, 456)), irect_from_string("123x456+4+5"));
  EXPECT_THROW({irect_from_string("AxB");}, std::invalid_argument);
  EXPECT_THROW({irect_from_string("1,2,3,4,5");}, std::invalid_argument);
}

TEST(io_test, frect_from_string)
{
  EXPECT_EQ(frect(4, 5, 123, 456), frect_from_string("frect(4, 5, 123, 456)"));
  EXPECT_EQ(frect(4, 5, 123, 456), frect_from_string("4, 5, 123, 456"));
  EXPECT_EQ(frect(4, 5, 123, 456), frect_from_string("4,5,123,456"));
  EXPECT_EQ(frect(fsize(123, 456)), frect_from_string("123x456"));
  EXPECT_EQ(frect(fsize(123, 456)), frect_from_string(" 123x456 "));
  EXPECT_EQ(frect(fpoint(4, 5), fsize(123, 456)), frect_from_string("123x456+4+5"));
  EXPECT_THROW({frect_from_string("AxB");}, std::invalid_argument);
  EXPECT_THROW({frect_from_string("1,2,3,4,5");}, std::invalid_argument);
}

/* EOF */
