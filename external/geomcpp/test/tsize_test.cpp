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

#include <iostream>

#include <geom/geom.hpp>
#include <geom/io.hpp>

using namespace geom;

#ifndef __clang__
#  pragma GCC diagnostic ignored "-Wsuggest-override"
#else
#  pragma GCC diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif

template<typename T>
class tsize_test : public ::testing::Test {};

using MyTypes = ::testing::Types<int, long, float, double>;
TYPED_TEST_SUITE(tsize_test, MyTypes);

TYPED_TEST(tsize_test, construction)
{
  using size = tsize<TypeParam>;

  size value(123, 567);
  EXPECT_EQ(value, size(123, 567));
  EXPECT_EQ(value.width(), 123);
  EXPECT_EQ(value.height(), 567);
}

TYPED_TEST(tsize_test, construction2)
{
  using size = tsize<TypeParam>;

  size value;
  EXPECT_EQ(value, size(0, 0));
  EXPECT_EQ(value.width(), 0);
  EXPECT_EQ(value.height(), 0);
}

TYPED_TEST(tsize_test, conversion)
{
  using size = tsize<TypeParam>;

  EXPECT_EQ(size(isize(123, 567)), size(123, 567));
  EXPECT_EQ(size(fsize(123, 567)), size(123, 567));
}

TYPED_TEST(tsize_test, equality)
{
  using size = tsize<TypeParam>;

  EXPECT_TRUE(size(1, 2) == size(1, 2));
  EXPECT_FALSE(size(1, 2) == size(2, 1));
  EXPECT_TRUE(size(1, 2) != size(2, 1));
  EXPECT_FALSE(size(1, 2) != size(1, 2));
}

TYPED_TEST(tsize_test, area)
{
  using size = tsize<TypeParam>;

  EXPECT_EQ(area(size(123, 567)), 69741);
}

TYPED_TEST(tsize_test, multiplication)
{
  using size = tsize<TypeParam>;

  EXPECT_EQ(size(123, 567) * TypeParam(3), size(123*3, 567 * TypeParam(3)));
  EXPECT_EQ(TypeParam(3) * size(123, 567), size(123*3, 567 * TypeParam(3)));
}

TYPED_TEST(tsize_test, transpose)
{
  using size = tsize<TypeParam>;

  EXPECT_EQ(transpose(size(123, 567)), size(567, 123));
}

TYPED_TEST(tsize_test, aspect_ratio)
{
  using size = tsize<TypeParam>;

  EXPECT_FLOAT_EQ(aspect_ratio(size(400, 300)), 4.0f / 3.0f);
}

TYPED_TEST(tsize_test, is_valid)
{
  using size = tsize<TypeParam>;

  EXPECT_TRUE(size(0, 0).is_valid());
  EXPECT_TRUE(size(1, 1).is_valid());
  EXPECT_FALSE(size(-1, 1).is_valid());
  EXPECT_FALSE(size(1, -1).is_valid());
  EXPECT_FALSE(size(-1, -1).is_valid());
}

TYPED_TEST(tsize_test, is_empty)
{
  using size = tsize<TypeParam>;

  EXPECT_TRUE(size(0, 0).is_empty());
  EXPECT_FALSE(size(1, 1).is_empty());
  EXPECT_TRUE(size(-1, 1).is_empty());
  EXPECT_TRUE(size(1, -1).is_empty());
  EXPECT_TRUE(size(-1, -1).is_empty());
}

TYPED_TEST(tsize_test, is_null)
{
  using size = tsize<TypeParam>;

  EXPECT_TRUE(size(0, 0).is_null());
  EXPECT_FALSE(size(1, 1).is_null());
  EXPECT_FALSE(size(-1, 1).is_null());
  EXPECT_FALSE(size(1, -1).is_null());
  EXPECT_FALSE(size(-1, -1).is_null());
}

TYPED_TEST(tsize_test, contains_size)
{
  using size = tsize<TypeParam>;

  EXPECT_FALSE(contains(size(0, 0), size(10, 20)));
  EXPECT_TRUE(contains(size(0, 0), size(0, 0)));
  EXPECT_TRUE(contains(size(10, 20), size(5, 10)));
  EXPECT_FALSE(contains(size(10, 20), size(15, 10)));
  EXPECT_TRUE(contains(size(20, 10), size(15, 10)));
  EXPECT_TRUE(contains(size(20, 10), size(0, 0)));
  EXPECT_FALSE(contains(size(20, 10), size(0, 20)));
}

TYPED_TEST(tsize_test, contains_point)
{
  using size = tsize<TypeParam>;
  using point = tpoint<TypeParam>;

  EXPECT_FALSE(contains(size(0, 0), point(10, 20)));
  EXPECT_FALSE(contains(size(0, 0), point(0, 0)));
  EXPECT_TRUE(contains(size(10, 20), point(5, 10)));
  EXPECT_FALSE(contains(size(10, 20), point(15, 10)));
  EXPECT_FALSE(contains(size(20, 10), point(15, 10)));
  EXPECT_TRUE(contains(size(20, 10), point(0, 0)));
  EXPECT_FALSE(contains(size(20, 10), point(0, 20)));
}

TYPED_TEST(tsize_test, resize_to_fit)
{
  using size = tsize<TypeParam>;

  EXPECT_EQ(resize_to_fit(size(1024, 768), size(1024, 768)), size(1024, 768));
  EXPECT_EQ(resize_to_fit(size(640, 480), size(1024, 768)), size(1024, 768));
  EXPECT_EQ(resize_to_fit(size(640, 400), size(1024, 768)), size(1024, 640));
  EXPECT_EQ(resize_to_fit(size(400, 640), size(768, 1024)), size(640, 1024));
}

TYPED_TEST(tsize_test, grow_to_fit)
{
  using size = tsize<TypeParam>;

  EXPECT_EQ(grow_to_fit(size(1600, 1200), size(1024, 768)), size(1600, 1200));
  EXPECT_EQ(grow_to_fit(size(1024, 768), size(1024, 768)), size(1024, 768));
  EXPECT_EQ(grow_to_fit(size(640, 480), size(1024, 768)), size(1024, 768));
  EXPECT_EQ(grow_to_fit(size(640, 400), size(1024, 768)), size(1024, 640));
  EXPECT_EQ(grow_to_fit(size(400, 640), size(768, 1024)), size(640, 1024));
}

TYPED_TEST(tsize_test, shrink_to_fit)
{
  using size = tsize<TypeParam>;

  EXPECT_EQ(shrink_to_fit(size(1600, 1200), size(1024, 768)), size(1024, 768));
  EXPECT_EQ(shrink_to_fit(size(1024, 768), size(1024, 768)), size(1024, 768));
  EXPECT_EQ(shrink_to_fit(size(6400, 4800), size(1024, 768)), size(1024, 768));
  EXPECT_EQ(shrink_to_fit(size(6400, 4000), size(1024, 768)), size(1024, 640));
  EXPECT_EQ(shrink_to_fit(size(400, 640), size(768, 1024)), size(400, 640));
}

/* EOF */
