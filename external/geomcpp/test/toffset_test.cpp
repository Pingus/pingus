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

#include <glm/glm.hpp>
#include <geom/geom.hpp>

using namespace geom;

#ifndef __clang__
#  pragma GCC diagnostic ignored "-Wsuggest-override"
#else
#  pragma GCC diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif

template<typename T>
class toffset_test : public ::testing::Test {};

using MyTypes = ::testing::Types<int, long, float, double>;
TYPED_TEST_SUITE(toffset_test, MyTypes);

TYPED_TEST(toffset_test, construction)
{
  using offset = toffset<TypeParam>;

  offset value(123, 567);
  EXPECT_EQ(value, offset(123, 567));
  EXPECT_EQ(value.x(), 123);
  EXPECT_EQ(value.y(), 567);

  EXPECT_EQ(offset(), offset(0, 0));
  EXPECT_EQ(offset(glm::vec2(111, 222)), offset(111, 222));

  EXPECT_EQ(offset(offset(333, 444)), offset(333, 444));
}

TYPED_TEST(toffset_test, conversion)
{
  using offset = toffset<TypeParam>;

  EXPECT_EQ(offset(toffset<int>(123, 456)), offset(123, 456));
  EXPECT_EQ(offset(toffset<float>(123, 456)), offset(123, 456));
}

TYPED_TEST(toffset_test, equality)
{
  using offset = toffset<TypeParam>;

  offset lhs(123, 456);
  offset rhs1(123, 456);
  offset rhs2(456, 123);

  EXPECT_TRUE(lhs == rhs1);
  EXPECT_FALSE(lhs == rhs2);

  EXPECT_FALSE(lhs != rhs1);
  EXPECT_TRUE(lhs != rhs2);
}

TYPED_TEST(toffset_test, addition)
{
  using offset = toffset<TypeParam>;

  offset lhs(123, 456);
  offset rhs(23, 56);

  EXPECT_EQ(lhs + rhs, offset(146, 512));
  EXPECT_EQ(lhs - rhs, offset(100, 400));

  lhs += rhs;
  EXPECT_EQ(lhs, offset(146, 512));

  lhs -= rhs;
  EXPECT_EQ(lhs, offset(123, 456));
}

TYPED_TEST(toffset_test, multiplication)
{
  using offset = toffset<TypeParam>;

  offset lhs(123, 456);
  EXPECT_EQ(lhs * static_cast<TypeParam>(3), offset(369, 1368));
  EXPECT_EQ(static_cast<TypeParam>(3) * lhs, offset(369, 1368));
  EXPECT_EQ(offset(369, 1368) / static_cast<TypeParam>(3), lhs);

  lhs *= static_cast<TypeParam>(3);
  EXPECT_EQ(lhs, offset(369, 1368));

  lhs /= static_cast<TypeParam>(3);
  EXPECT_EQ(lhs, offset(123, 456));
}

TYPED_TEST(toffset_test, transpose)
{
  using offset = toffset<TypeParam>;

  EXPECT_EQ(transpose(offset(123, 567)), offset(567, 123));
}

/* EOF */
