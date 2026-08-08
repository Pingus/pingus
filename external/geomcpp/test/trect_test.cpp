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
class trect_test : public ::testing::Test {};

using MyTypes = ::testing::Types<int, long, float, double>;
TYPED_TEST_SUITE(trect_test, MyTypes);

TYPED_TEST(trect_test, construction1)
{
  using rect = trect<TypeParam>;
  using point = tpoint<TypeParam>;
  using size = tsize<TypeParam>;

  rect value(point(12, 34), size(56, 78));
  EXPECT_EQ(value, rect(point(12, 34), size(56, 78)));
  EXPECT_EQ(value.x(), 12);
  EXPECT_EQ(value.y(), 34);
  EXPECT_EQ(value.width(), 56);
  EXPECT_EQ(value.height(), 78);
}

TYPED_TEST(trect_test, construction2)
{
  using rect = trect<TypeParam>;
  using point = tpoint<TypeParam>;
  using size = tsize<TypeParam>;

  rect value(12, 34, 56, 78);
  EXPECT_EQ(value, rect(point(12, 34), size(44, 44)));
  EXPECT_EQ(value.x(), 12);
  EXPECT_EQ(value.y(), 34);
  EXPECT_EQ(value.width(), 44);
  EXPECT_EQ(value.height(), 44);
}

TYPED_TEST(trect_test, construction3)
{
  using rect = trect<TypeParam>;

  rect value;
  EXPECT_EQ(value, rect(0, 0, 0, 0));
  EXPECT_EQ(value.x(), 0);
  EXPECT_EQ(value.y(), 0);
  EXPECT_EQ(value.width(), 0);
  EXPECT_EQ(value.height(), 0);
}

TYPED_TEST(trect_test, conversion)
{
  using rect = trect<TypeParam>;

  EXPECT_EQ(rect(irect(123, 567, 800, 900)), rect(123, 567, 800, 900));
}

TYPED_TEST(trect_test, equality)
{
  using rect = trect<TypeParam>;

  EXPECT_TRUE(rect(1, 2, 3, 4) == rect(1, 2, 3, 4));
  EXPECT_FALSE(rect(1, 2, 3, 4) == rect(4, 3, 2, 1));
  EXPECT_TRUE(rect(1, 2, 3, 4) != rect(4, 3, 2, 1));
  EXPECT_FALSE(rect(1, 2, 3, 4) != rect(1, 2, 3, 4));
}

TYPED_TEST(trect_test, addition)
{
  using rect = trect<TypeParam>;
  using offset = toffset<TypeParam>;

  EXPECT_EQ(rect(0, 0, 44, 33) + offset(6, 7), rect(6, 7, 50, 40));
  EXPECT_EQ(rect(0, 0, 44, 33) - offset(6, 7), rect(-6, -7, 38, 26));

  rect r(0, 0, 44, 33);
  EXPECT_EQ(r += offset(6, 7), rect(6, 7, 50, 40));
  EXPECT_EQ(r -= offset(6, 7), rect(-0, 0, 44, 33));
}

TYPED_TEST(trect_test, grow)
{
  using rect = trect<TypeParam>;
  using point = tpoint<TypeParam>;
  using size = tsize<TypeParam>;

  EXPECT_EQ(grow(rect(point(0, 0), size(123, 567)), TypeParam(10)),
            rect(point(-10, -10), size(143, 587)));
  EXPECT_EQ(grow(rect(point(0, 0), size(123, 567)), TypeParam(10), TypeParam(20)),
            rect(point(-10, -20), size(143, 607)));
}

TYPED_TEST(trect_test, intersection)
{
  using rect = trect<TypeParam>;
  using point = tpoint<TypeParam>;
  using size = tsize<TypeParam>;

  EXPECT_EQ(intersection(rect(point(0, 0), size(123, 567)),
                         rect(point(10, 20), size(50, 60))),
            rect(point(10, 20), size(50, 60)));
}

TYPED_TEST(trect_test, overlapping)
{
  using rect = trect<TypeParam>;

  EXPECT_TRUE(intersects(rect(0, 0, 123, 567),
                         rect(10, 20, 50, 60)));
  EXPECT_TRUE(intersects(rect(0, 0, 123, 567),
                         rect(0, 0, 50, 60)));
  EXPECT_FALSE(intersects(rect(0, 0, 123, 567),
                          rect(-500, -500, -50, -60)));
}

TYPED_TEST(trect_test, contains)
{
  using rect = trect<TypeParam>;
  using point = tpoint<TypeParam>;

  EXPECT_TRUE(contains(rect(10, 20, 30, 40), point(15, 25)));
  EXPECT_FALSE(contains(rect(10, 20, 30, 40), point(-15, -19)));
}

TYPED_TEST(trect_test, contains2)
{
  using rect = trect<TypeParam>;

  EXPECT_TRUE(contains(rect(10, 20, 30, 40), rect(15, 25, 20, 30)));
  EXPECT_FALSE(contains(rect(10, 20, 30, 40), rect(-15, -19, 100, 100)));
}

TYPED_TEST(trect_test, normalize)
{
  using rect = trect<TypeParam>;

  EXPECT_EQ(normalize(rect(10, 20, -30, -40)),
            rect(-30, -40, 10, 20));
  EXPECT_EQ(normalize(rect(10, 20, 30, -40)),
            rect(10, -40, 30, 20));
}

TYPED_TEST(trect_test, unite)
{
  using rect = trect<TypeParam>;

  EXPECT_EQ(unite(rect(0, 0, 10, 20),
                  rect(30, 40, 50, 70)),
            rect(0, 0, 50, 70));
  EXPECT_EQ(unite(rect(30, 40, 50, 70),
                  rect(0, 0, 10, 20)),
            rect(0, 0, 50, 70));
}

TYPED_TEST(trect_test, transpose)
{
  using rect = trect<TypeParam>;
  using point = tpoint<TypeParam>;
  using size = tsize<TypeParam>;

  EXPECT_EQ(transpose(rect(point(0, 0), size(10, 20))),
            rect(point(0, 0), size(20, 10)));
  EXPECT_EQ(transpose(rect(point(35, 24), size(10, 20))),
            rect(point(35, 24), size(20, 10)));
}

TYPED_TEST(trect_test, aspect_ratio)
{
  using rect = trect<TypeParam>;
  using point = tpoint<TypeParam>;
  using size = tsize<TypeParam>;

  EXPECT_FLOAT_EQ(aspect_ratio(rect(point(0, 0), size(400, 300))),
                  4.0f / 3.0f);
  EXPECT_FLOAT_EQ(aspect_ratio(rect(point(50, 60), size(1600, 900))),
                  16.0f / 9.0f);
}

TYPED_TEST(trect_test, diagonal)
{
  using rect = trect<TypeParam>;

  EXPECT_FLOAT_EQ(diagonal(rect(0, 0, 40, 30)), 50.0f);
}

TYPED_TEST(trect_test, anchored_rect)
{
  using point = tpoint<TypeParam>;
  using size = tsize<TypeParam>;
  using rect = trect<TypeParam>;

  EXPECT_EQ(anchored_rect(point(100, 50), size(60, 40), origin::TOP_LEFT),
            rect(100, 50, 160, 90));

  EXPECT_EQ(anchored_rect(point(100, 50), size(60, 40), origin::TOP_CENTER),
            rect(70, 50, 130, 90));

  EXPECT_EQ(anchored_rect(point(100, 50), size(60, 40), origin::TOP_RIGHT),
            rect(40, 50, 100, 90));

  EXPECT_EQ(anchored_rect(point(100, 50), size(60, 40), origin::CENTER_LEFT),
            rect(100, 30, 160, 70));

  EXPECT_EQ(anchored_rect(point(100, 50), size(60, 40), origin::CENTER),
            rect(70, 30, 130, 70));

  EXPECT_EQ(anchored_rect(point(100, 50), size(60, 40), origin::CENTER_RIGHT),
            rect(40, 30, 100, 70));

  EXPECT_EQ(anchored_rect(point(100, 50), size(60, 40), origin::BOTTOM_LEFT),
            rect(100, 10, 160, 50));

  EXPECT_EQ(anchored_rect(point(100, 50), size(60, 40), origin::BOTTOM_CENTER),
            rect(70, 10, 130, 50));

  EXPECT_EQ(anchored_rect(point(100, 50), size(60, 40), origin::BOTTOM_RIGHT),
            rect(40, 10, 100, 50));
}

/* EOF */
