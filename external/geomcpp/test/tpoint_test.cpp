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

#include <geom/geom.hpp>

using namespace geom;

#ifndef __clang__
#  pragma GCC diagnostic ignored "-Wsuggest-override"
#else
#  pragma GCC diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif

template<typename T>
class tpoint_test : public ::testing::Test {};

using MyTypes = ::testing::Types<int, long, float, double>;
TYPED_TEST_SUITE(tpoint_test, MyTypes);

TYPED_TEST(tpoint_test, construction)
{
  using point = tpoint<TypeParam>;

  point value(123, 567);
  EXPECT_EQ(value, point(123, 567));
  EXPECT_EQ(value.x(), 123);
  EXPECT_EQ(value.y(), 567);
}

TYPED_TEST(tpoint_test, construction2)
{
  using point = tpoint<TypeParam>;

  point value;
  EXPECT_EQ(value, point(0, 0));
  EXPECT_EQ(value.x(), 0);
  EXPECT_EQ(value.y(), 0);
}

TYPED_TEST(tpoint_test, conversion)
{
  using point = tpoint<TypeParam>;

  EXPECT_EQ(point(ipoint(123, 567)), point(123, 567));
  EXPECT_EQ(point(fpoint(123, 567)), point(123, 567));
}

TYPED_TEST(tpoint_test, equality)
{
  using point = tpoint<TypeParam>;

  EXPECT_TRUE(point(1, 2) == point(1, 2));
  EXPECT_FALSE(point(1, 2) == point(2, 1));
  EXPECT_TRUE(point(1, 2) != point(2, 1));
  EXPECT_FALSE(point(1, 2) != point(1, 2));
}

TYPED_TEST(tpoint_test, distance)
{
  using point = tpoint<TypeParam>;

  EXPECT_FLOAT_EQ(distance(point(0, 0), point(10, 0)), 10.0f);
  EXPECT_FLOAT_EQ(distance(point(10, 10), point(20, 20)), 14.142135623730951f);
}

TYPED_TEST(tpoint_test, transpose)
{
  using point = tpoint<TypeParam>;

  EXPECT_EQ(transpose(point(4, 3)), point(3, 4));
  EXPECT_EQ(transpose(point(123, 456)), point(456, 123));
}

TYPED_TEST(tpoint_test, addition)
{
  using point = tpoint<TypeParam>;
  using size = tsize<TypeParam>;

  EXPECT_EQ(point(1, 2) + size(5, 6), point(6, 8));
}

TYPED_TEST(tpoint_test, interpolate)
{
  using point = tpoint<TypeParam>;

  EXPECT_EQ(interpolate(point(10, 30), point(30, 0), 0.5f),
            point(20, 15));
}

TYPED_TEST(tpoint_test, clamp)
{
  using point = tpoint<TypeParam>;
  using rect = trect<TypeParam>;

  EXPECT_EQ(geom::clamp(point(0, 0), rect(20, 50, 200, 500)), point(20, 50));
  EXPECT_EQ(geom::clamp(point(1000, 5000), rect(20, 50, 200, 500)), point(200, 500));
  EXPECT_EQ(geom::clamp(point(100, 150), rect(20, 50, 200, 500)), point(100, 150));
}

/* EOF */
