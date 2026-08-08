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

#include "json_writer_impl.hpp"
#include "writer_impl_test.hpp"

using namespace prio;

TEST(JsonWriterImplTest, write)
{
  std::ostringstream os;
  JsonWriterImpl writer(os);
  write_testfile(writer);

  ASSERT_EQ(os.str(),
            "{\n"
            "\t\"testfile\" : \n"
            "\t{\n"
            "\t\t\"background\" : \n"
            "\t\t{\n"
            "\t\t\t\"color\" : \n"
            "\t\t\t{\n"
            "\t\t\t\t\"blue\" : 0.5,\n"
            "\t\t\t\t\"green\" : 0.25,\n"
            "\t\t\t\t\"red\" : 0.125\n"
            "\t\t\t}\n"
            "\t\t},\n"
            "\t\t\"collection\" : \n"
            "\t\t[\n"
            "\t\t\t{\n"
            "\t\t\t\t\"object1\" : \n"
            "\t\t\t\t{\n"
            "\t\t\t\t\t\"x\" : 123.5,\n"
            "\t\t\t\t\t\"y\" : 456.5\n"
            "\t\t\t\t}\n"
            "\t\t\t},\n"
            "\t\t\t{\n"
            "\t\t\t\t\"object1\" : \n"
            "\t\t\t\t{\n"
            "\t\t\t\t\t\"x\" : 78.5,\n"
            "\t\t\t\t\t\"y\" : 90.5\n"
            "\t\t\t\t}\n"
            "\t\t\t}\n"
            "\t\t],\n"
            "\t\t\"escapedstringval\" : \"\\\"Hello\\\\World\\\"\",\n"
            "\t\t\"falseval\" : false,\n"
            "\t\t\"floatval\" : 123.5,\n"
            "\t\t\"floatvals\" : \n"
            "\t\t[\n"
            "\t\t\t1.5,\n"
            "\t\t\t2.5,\n"
            "\t\t\t3.5\n"
            "\t\t],\n"
            "\t\t\"intval\" : 123,\n"
            "\t\t\"intvals\" : \n"
            "\t\t[\n"
            "\t\t\t1,\n"
            "\t\t\t2,\n"
            "\t\t\t3\n"
            "\t\t],\n"
            "\t\t\"mapping\" : \n"
            "\t\t{\n"
            "\t\t\t\"one\" : 1,\n"
            "\t\t\t\"three\" : 3,\n"
            "\t\t\t\"two\" : 2\n"
            "\t\t},\n"
            "\t\t\"stringval\" : \"Hello World\",\n"
            "\t\t\"stringvals\" : \n"
            "\t\t[\n"
            "\t\t\t\"\\\"Hello\",\n"
            "\t\t\t\"World\\\"\"\n"
            "\t\t],\n"
            "\t\t\"trueval\" : true,\n"
            "\t\t\"truevals\" : \n"
            "\t\t[\n"
            "\t\t\ttrue,\n"
            "\t\t\tfalse,\n"
            "\t\t\ttrue\n"
            "\t\t]\n"
            "\t}\n}");
}

/* EOF */
