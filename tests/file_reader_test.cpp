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

const char json_doc[] =
  "(test-document\n"
  "  (intvalue 5)\n"
  "  (submap (int 7) (float 9.9))\n"
  "  (collection (obj1) (obj2) (obj3))\n"
  "  (object (realthing (prop1 5) (prop2 7)))\n"
  "  (floatvalue 5.5))";

const char sexpr_doc[] =
  "{\n"
  "  \"test-document\": {\n"
  "    \"intvalue\": 5,\n"
  "    \"submap\": { \"int\": 7, \"float\": 9.9 },\n"
  "    \"collection\": [ { \"obj1\": {}}, { \"obj2\": {}}, {\"obj3\": {}} ],\n"
  "    \"object\": { \"realthing\": { \"prop1\": 5, \"prop2\": 7 } },\n"
  "    \"floatvalue\": 5.5\n"
  "  }\n"
  "}\n";

class FileReaderTest : public ::testing::TestWithParam<std::string>
{
public:
  FileReaderTest() :
    doc(),
    body()
  {}

  void SetUp() override
  {
    std::istringstream stream(GetParam());
    doc = FileReader::parse(stream);
    body = doc.get_mapping();
  }

public:
  ReaderObject doc;
  ReaderMapping body;
};

TEST_P(FileReaderTest, parse)
{
  EXPECT_EQ("test-document", doc.get_name());
}

TEST_P(FileReaderTest, read_int)
{
  int intvalue;
  ASSERT_TRUE(body.read_int("intvalue", intvalue));
  EXPECT_EQ(5, intvalue);
}

TEST_P(FileReaderTest, read_float)
{
  float floatvalue;
  ASSERT_TRUE(body.read_float("floatvalue", floatvalue));
  EXPECT_EQ(5.5f, floatvalue);
}

TEST_P(FileReaderTest, read_mapping)
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

TEST_P(FileReaderTest, read_collection)
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

TEST_P(FileReaderTest, read_object)
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

INSTANTIATE_TEST_CASE_P(ParamFileReaderTest, FileReaderTest,
                        ::testing::Values(sexpr_doc, json_doc));

/* EOF */
