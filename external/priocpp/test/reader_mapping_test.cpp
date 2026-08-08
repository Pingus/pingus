// prio - Property I/O for C++
// Copyright (C) 2005-2020 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <gtest/gtest.h>

#include <fstream>

#include <prio/reader_collection.hpp>
#include <prio/reader_document.hpp>
#include <prio/reader_error.hpp>
#include <prio/reader_mapping.hpp>

using namespace prio;

class ReaderMappingTest : public ::testing::TestWithParam<std::string>
{
public:
  ReaderMappingTest() :
    filename("test/data/data" + GetParam()),
    doc(ReaderDocument::from_file(filename, ErrorHandler::IGNORE)),
    map(doc.get_root().get_mapping()),
    doc_pedantic(ReaderDocument::from_file("test/data/data" + GetParam(), ErrorHandler::THROW)),
    map_pedantic(doc_pedantic.get_root().get_mapping())
  {}

protected:
  std::string filename;
  ReaderDocument const doc;
  ReaderMapping const map;

  ReaderDocument const doc_pedantic;
  ReaderMapping const map_pedantic;
};

TEST_P(ReaderMappingTest, operator_bool)
{
  ASSERT_TRUE(map);
  ASSERT_TRUE(map_pedantic);
  ASSERT_FALSE(ReaderMapping());
}

TEST_P(ReaderMappingTest, document)
{
  EXPECT_EQ(map.get_document().get_filename(), filename);
}

TEST_P(ReaderMappingTest, read_bool)
{
  bool boolvalue;
  ASSERT_TRUE(map.read("boolvalue", boolvalue));
  EXPECT_EQ(true, boolvalue);
}

TEST_P(ReaderMappingTest, get_bool)
{
  ASSERT_EQ(map.get<bool>("boolvalue"), true);
}

TEST_P(ReaderMappingTest, read_int)
{
  int intvalue;
  ASSERT_TRUE(map.read("intvalue", intvalue));
  EXPECT_EQ(5, intvalue);
}

TEST_P(ReaderMappingTest, get_int)
{
  ASSERT_EQ(map.get<int>("intvalue"), 5);
  ASSERT_EQ(map.get<int>("intvalue-doesnotexist", 99), 99);
}

TEST_P(ReaderMappingTest, read_int_from_float)
{
  int intvalue;
  ASSERT_FALSE(map.read("floatvalue", intvalue));
  ASSERT_THROW(map_pedantic.read("floatvalue", intvalue), ReaderError);
}

TEST_P(ReaderMappingTest, read_float)
{
  float floatvalue;
  ASSERT_TRUE(map.read("floatvalue", floatvalue));
  EXPECT_EQ(5.5f, floatvalue);
}

TEST_P(ReaderMappingTest, read_float_from_int)
{
  float floatvalue;
  ASSERT_TRUE(map.read("intvalue", floatvalue));
  EXPECT_EQ(5.0f, floatvalue);
  ASSERT_NO_THROW(map_pedantic.read("intvalue", floatvalue));
}

TEST_P(ReaderMappingTest, get_float)
{
  ASSERT_EQ(map.get<float>("floatvalue"), 5.5f);
  ASSERT_EQ(map.get<float>("floatvalue-doesnotexist", 1.0f), 1.0f);
}

TEST_P(ReaderMappingTest, read_string)
{
  std::string stringvalue;
  ASSERT_TRUE(map.read("stringvalue", stringvalue));
  EXPECT_EQ("Hello World", stringvalue);
}

TEST_P(ReaderMappingTest, get_string)
{
  ASSERT_EQ(map.get<std::string>("stringvalue"), "Hello World");
  ASSERT_EQ(map.get<std::string>("stringvalue-doesnotexist", "Fallback"), "Fallback");
}

TEST_P(ReaderMappingTest, read_bools)
{
  std::vector<bool> boolvalues;
  ASSERT_TRUE(map.read("boolvalues", boolvalues));
  EXPECT_EQ(std::vector<bool>({true, false, true}), boolvalues);
}

TEST_P(ReaderMappingTest, read_bools__fail)
{
  std::vector<bool> original({ true, false, false, false});
  std::vector<bool> boolvalues2 = original;

  ASSERT_THROW(map_pedantic.read("stringvalues", boolvalues2), ReaderError);
  EXPECT_EQ(original, boolvalues2);

  ASSERT_FALSE(map.read("stringvalues", boolvalues2));
  EXPECT_EQ(original, boolvalues2);
}

TEST_P(ReaderMappingTest, get_bools)
{
  EXPECT_EQ(map.get<std::vector<bool>>("boolvalues"),
            std::vector<bool>({ true, false, true }));
  EXPECT_EQ(map.get<std::vector<bool>>("boolvalues-doesnotexist"),
            std::vector<bool>());
}

TEST_P(ReaderMappingTest, read_ints)
{
  std::vector<int> intvalues;
  ASSERT_TRUE(map.read("intvalues", intvalues));
  EXPECT_EQ(std::vector<int>({1, 2, 3, 4}), intvalues);
}

TEST_P(ReaderMappingTest, get_ints)
{
  EXPECT_EQ(map.get<std::vector<int>>("intvalues"),
            std::vector<int>({1, 2, 3, 4}));
  EXPECT_EQ(map.get<std::vector<int>>("intvalues-doesnotexist"),
            std::vector<int>());

  EXPECT_EQ(map.get<std::vector<int>>("floatvalues"),
            std::vector<int>({}));
  EXPECT_THROW(static_cast<void>(map_pedantic.get<std::vector<int>>("floatvalues")),
               ReaderError);
}

TEST_P(ReaderMappingTest, read_floats)
{
  std::vector<float> floatvalues;
  ASSERT_TRUE(map.read("floatvalues", floatvalues));
  EXPECT_EQ(std::vector<float>({1.5f, 2.5f, 3.5f, 4.5f}), floatvalues);
}

TEST_P(ReaderMappingTest, get_floats)
{
  EXPECT_EQ(map.get<std::vector<float>>("floatvalues"),
            std::vector<float>({1.5f, 2.5f, 3.5f, 4.5f}));
  EXPECT_EQ(map.get<std::vector<float>>("floatvalues-doesnotexist"),
            std::vector<float>());

  EXPECT_EQ(map.get<std::vector<float>>("intvalues"),
            std::vector<float>({1.0f, 2.0f, 3.0f, 4.0f}));
  EXPECT_EQ(map_pedantic.get<std::vector<float>>("intvalues"),
            std::vector<float>({1.0f, 2.0f, 3.0f, 4.0f}));
}

TEST_P(ReaderMappingTest, read_strings)
{
  std::vector<std::string> stringvalues;
  ASSERT_TRUE(map.read("stringvalues", stringvalues));
  EXPECT_EQ(std::vector<std::string>({"Hello", "World"}), stringvalues);
}

TEST_P(ReaderMappingTest, get_strings)
{
  EXPECT_EQ(map.get<std::vector<std::string>>("stringvalues"),
            std::vector<std::string>({"Hello", "World"}));
  EXPECT_EQ(map.get<std::vector<std::string>>("stringvalues"),
            std::vector<std::string>({"Hello", "World"}));
}

namespace {

enum class MyEnum { A, B, C };
MyEnum string2enum(std::string_view text)
{
  if (text == "A") { return MyEnum::A; }
  else if (text == "B") { return MyEnum::B; }
  else if (text == "C") { return MyEnum::C; }
  else { throw std::invalid_argument("couldn't convert enum"); }
}

} // namespace

TEST_P(ReaderMappingTest, read_enum)
{
  MyEnum enumvalue;
  ASSERT_TRUE(map.read("enumvalue", enumvalue, string2enum));
  EXPECT_EQ(enumvalue, MyEnum::C);
}

TEST_P(ReaderMappingTest, get_enum)
{
  EXPECT_EQ(map.get<MyEnum>("enumvalue", string2enum), MyEnum::C);
  EXPECT_EQ(map.get<MyEnum>("enumvalue-doesnotexist", string2enum, MyEnum::B), MyEnum::B);
}

namespace {

struct CustomType {
  int value = 0;
};

} // namespace

namespace prio {

template<>
bool read_custom(ReaderMapping const& mapping, std::string_view key, CustomType& value)
{
  return mapping.read(key, value.value);
}

} // namespace prio

TEST_P(ReaderMappingTest, read_custom)
{
  CustomType customvalue;
  ASSERT_TRUE(map.read("customvalue", customvalue));
  EXPECT_EQ(5, customvalue.value);
}

TEST_P(ReaderMappingTest, get_custom_mapping)
{
  EXPECT_EQ(map.get("customvalue", CustomType{}).value, 5);
  EXPECT_EQ(map.get("customvalue-doesnotexist", CustomType{.value = 99}).value, 99);
}

TEST_P(ReaderMappingTest, read_mapping)
{
  ReaderMapping submap;
  ASSERT_TRUE(map.read("submap", submap));
  {
    int intvalue;
    ASSERT_TRUE(submap.read("int", intvalue));
    EXPECT_EQ(7, intvalue);
  }
  {
    float floatvalue;
    ASSERT_TRUE(submap.read("float", floatvalue));
    EXPECT_EQ(9.9f, floatvalue);
  }
}

TEST_P(ReaderMappingTest, read_collection)
{
  ReaderCollection collection;
  ASSERT_TRUE(map.read("collection", collection));
  EXPECT_EQ(3, collection.get_objects().size());
  auto objs = collection.get_objects();
  std::vector<std::string> result;
  for(auto const& e : objs)
  {
    result.push_back(e.get_name()); // NOLINT
  }
  EXPECT_EQ(std::vector<std::string>({"obj1", "obj2", "obj3"}), result);
}

TEST_P(ReaderMappingTest, read_object)
{
  ReaderObject object;
  ASSERT_TRUE(map.read("object", object));
  EXPECT_EQ("realthing", object.get_name());
  ReaderMapping object_mapping = object.get_mapping();
  int prop1 = 0;
  int prop2 = 0;
  ASSERT_TRUE(object_mapping.read("prop1", prop1));
  ASSERT_TRUE(object_mapping.read("prop2", prop2));
  EXPECT_EQ(5, prop1);
  EXPECT_EQ(7, prop2);
}

TEST_P(ReaderMappingTest, get_object)
{
  EXPECT_TRUE(map.get<ReaderObject>("object"));
  EXPECT_FALSE(map.get<ReaderObject>("object-doesnotexist"));
  if (ReaderObject object = map.get<ReaderObject>("object-doesnotexist")) {}
}

TEST_P(ReaderMappingTest, get_collection)
{
  EXPECT_TRUE(map.get<ReaderCollection>("collection"));
  EXPECT_FALSE(map.get<ReaderCollection>("collection-doesnotexist"));
  if (ReaderCollection collection = map.get<ReaderCollection>("collection-doesnotexist")) {}
}

TEST_P(ReaderMappingTest, get_mapping)
{
  EXPECT_TRUE(map.get<ReaderMapping>("submap"));
  EXPECT_FALSE(map.get<ReaderMapping>("submap-doesnotexist"));
  if (ReaderMapping mapping = map.get<ReaderMapping>("submap-doesnotexist")) {}
}

TEST_P(ReaderMappingTest, must_read)
{
  int value;
  EXPECT_THROW(map.must_read("doesnotexist", value), ReaderError);
}

TEST_P(ReaderMappingTest, must_get)
{
  EXPECT_THROW(map.must_get<int>("doesnotexist"), ReaderError);
}

TEST_P(ReaderMappingTest, must_read_enum)
{
  MyEnum enumvalue = MyEnum::A;
  EXPECT_THROW(map.must_read("doesnotexist", enumvalue, string2enum), ReaderError);
  map.must_read("enumvalue", enumvalue, string2enum);
  EXPECT_EQ(enumvalue, MyEnum::C);
}

TEST_P(ReaderMappingTest, must_get_enum)
{
  EXPECT_THROW(map.must_get<MyEnum>("doesnotexist", string2enum), ReaderError);
  EXPECT_EQ(map.must_get<MyEnum>("enumvalue", string2enum), MyEnum::C);
}

TEST_P(ReaderMappingTest, get_keys)
{
  auto const& keys = map.get_keys();
  std::set<std::string> result(keys.begin(), keys.end());
  std::set<std::string> expected({
      "boolvalue", "intvalue", "floatvalue", "stringvalue",
      "boolvalues", "intvalues", "floatvalues", "stringvalues",
      "enumvalue", "customvalue", "submap",
      "collection", "object", "vector"});
  ASSERT_EQ(expected, result);
}

TEST_P(ReaderMappingTest, read_wrong)
{
  bool bool_value;
  int int_value;
  float float_value;
  std::string string_value;

  std::vector<bool> bool_values;
  std::vector<int> int_values;
  std::vector<float> float_values;
  std::vector<std::string> string_values;

  ReaderMapping mapping;
  ReaderObject object;
  ReaderCollection collection;

  for(auto const& key : map.get_keys()) {
    map.read(key, bool_value);
    map.read(key, int_value);
    map.read(key, float_value);
    map.read(key, string_value);

    map.read(key, bool_values);
    map.read(key, int_values);
    map.read(key, float_values);
    map.read(key, string_values);

    map.read(key, mapping);
    map.read(key, object);
    map.read(key, collection);
  }

  SUCCEED();
}

INSTANTIATE_TEST_CASE_P(ParamReaderMappingTest, ReaderMappingTest,
                        ::testing::Values(".sexp", ".json"));

/* EOF */
