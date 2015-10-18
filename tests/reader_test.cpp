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

#include <fstream>

#include "math/vector3f.hpp"
#include "util/reader.hpp"

class ReaderTest : public ::testing::TestWithParam<std::string>
{
public:
  ReaderTest() :
    doc(),
    body()
  {}

  void SetUp() override
  {
    std::ifstream stream(GetParam());
    doc = Reader::parse(stream);
    body = doc.get_mapping();
  }

public:
  ReaderObject doc;
  ReaderMapping body;
};

TEST_P(ReaderTest, parse)
{
  EXPECT_EQ("test-document", doc.get_name());
}

TEST_P(ReaderTest, read_int)
{
  int intvalue;
  ASSERT_TRUE(body.read_int("intvalue", intvalue));
  EXPECT_EQ(5, intvalue);
}

TEST_P(ReaderTest, read_float)
{
  float floatvalue;
  ASSERT_TRUE(body.read_float("floatvalue", floatvalue));
  EXPECT_EQ(5.5f, floatvalue);
}

TEST_P(ReaderTest, read_vector)
{
  Vector3f v;
  ASSERT_TRUE(body.read_vector("vector", v));
  EXPECT_EQ(1.0f, v.x);
  EXPECT_EQ(2.0f, v.y);
  EXPECT_EQ(3.0f, v.z);
}

TEST_P(ReaderTest, read_vectors)
{
  std::vector<Vector3f> vs;
  ASSERT_TRUE(body.read_vectors("vectors", vs));
  ASSERT_EQ(3, vs.size());
  EXPECT_EQ(Vector3f(1, 2, 3), vs[0]);
  EXPECT_EQ(Vector3f(4, 5, 6), vs[1]);
  EXPECT_EQ(Vector3f(7, 8, 9), vs[2]);
}

TEST_P(ReaderTest, read_mapping)
{
  ReaderMapping submap;
  ASSERT_TRUE(body.read_mapping("submap", submap));
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

TEST_P(ReaderTest, read_collection)
{
  ReaderCollection collection;
  ASSERT_TRUE(body.read_collection("collection", collection));
  EXPECT_EQ(3, collection.get_objects().size());
  auto objs = collection.get_objects();
  std::vector<std::string> result;
  for(auto e : objs)
  {
    result.push_back(e.get_name());
  }
  EXPECT_EQ(std::vector<std::string>({"obj1", "obj2", "obj3"}), result);
}

TEST_P(ReaderTest, read_object)
{
  ReaderObject object;
  ASSERT_TRUE(body.read_object("object", object));
  EXPECT_EQ("realthing", object.get_name());
  ReaderMapping object_mapping = object.get_mapping();
  int prop1 = 0;
  int prop2 = 0;
  ASSERT_TRUE(object_mapping.read_int("prop1", prop1));
  ASSERT_TRUE(object_mapping.read_int("prop2", prop2));
  EXPECT_EQ(5, prop1);
  EXPECT_EQ(7, prop2);
}

INSTANTIATE_TEST_CASE_P(ParamReaderTest, ReaderTest,
                        ::testing::Values("tests/sexpr.scm", "tests/json.json"));

/* EOF */
