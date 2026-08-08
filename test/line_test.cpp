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

#include <geom/line.hpp>

using namespace geom;

TEST(LineTest, length)
{
  fline line({0.0f, 0.0f}, {30.0f, 40.0f});
  EXPECT_FLOAT_EQ(line.length(), 50.0f);
}

TEST(LineTest, intersect)
{
  fline line1({0.0f, 0.0f}, {100.0f, 50.0f});
  fline line2({0.0f, 50.0f}, {100.0f, 0.0f});

  fpoint p;
  ASSERT_TRUE(line1.intersect(line2, p));
  EXPECT_FLOAT_EQ(p.x(), 50.0f);
  EXPECT_FLOAT_EQ(p.y(), 25.0f);
}

TEST(LineTest, distance)
{
  fline line({0.0f, 0.0f}, {100.0f, 100.0f});
  fpoint point1(50.0f, 50.0f);
  fpoint point2(60.0f, 40.0f);

  EXPECT_FLOAT_EQ(line.distance(point1), 0.0f);
  EXPECT_FLOAT_EQ(line.distance(point2), 14.142136f);
}

/* EOF */
