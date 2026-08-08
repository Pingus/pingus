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

#include <filesystem>
#include <fstream>

#include <prio/writer.hpp>

using namespace prio;

TEST(WriterTest, from_stream)
{
  enum class CustomEnum { NONE, ONE };

  std::ostringstream out;
  Writer writer = Writer::from_stream(out);
  writer.begin_object("testfile")
    .write("foo", 10)
    .write("bar", 5)
    .write("customenum", CustomEnum::ONE,
           [](CustomEnum v) {
             switch (v) {
               case CustomEnum::NONE: return "none";
               case CustomEnum::ONE: return "one";
               default: return "fail";
             }})
    .end_object();

  ASSERT_EQ(out.str(),
            "(testfile\n"
            "  (foo 10)\n"
            "  (bar 5)\n"
            "  (customenum \"one\"))\n"
            "\n"
            ";; EOF ;;\n");
}

TEST(WriterTest, from_file)
{
  std::filesystem::path const tmpdir(testing::TempDir());
  std::filesystem::path const outfile = tmpdir / "prio_test_output.txt";

  {
    Writer writer = Writer::from_file(outfile);
    writer.begin_object("testfile")
      .write("foo", 10)
      .write("bar", 5)
      .end_object();
  }

  std::ifstream fin(outfile);
  std::string result((std::istreambuf_iterator<char>(fin)),
                     std::istreambuf_iterator<char>());

  ASSERT_EQ(result,
            "(testfile\n"
            "  (foo 10)\n"
            "  (bar 5))\n"
            "\n"
            ";; EOF ;;\n");
}

namespace {

struct CustomType {};

} // namespace

namespace prio {

template<>
void write_custom(Writer& writer, std::string_view key, CustomType const& value)
{
  writer.write(key, "foobar");
}

} // namespace prio

TEST(WriterTest, write_custom)
{
  std::ostringstream out;
  Writer writer = Writer::from_stream(out);
  writer.begin_object("test");
  writer.write("custom", CustomType{});
  writer.end_object();
}

/* EOF */
