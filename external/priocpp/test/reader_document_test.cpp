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

#include <prio/reader_document.hpp>
#include <prio/reader_error.hpp>
#include <prio/reader_mapping.hpp>
#include <prio/reader_object.hpp>

using namespace prio;

class ReaderDocumentTest : public ::testing::TestWithParam<std::string> {};

TEST_P(ReaderDocumentTest, from_file)
{
  EXPECT_NO_THROW(ReaderDocument::from_file("test/data/data" + GetParam()));
  EXPECT_NO_THROW(ReaderDocument::from_file("test/data/data" + GetParam(), ErrorHandler::IGNORE));
}

TEST_P(ReaderDocumentTest, from_file__fail)
{
  EXPECT_THROW(ReaderDocument::from_file("does-not-exist"), ReaderError);
  EXPECT_THROW(ReaderDocument::from_file("does-not-exist", ErrorHandler::IGNORE), ReaderError);

  EXPECT_THROW(ReaderDocument::from_file("test/data/data-corrupt" + GetParam()), ReaderError);
  EXPECT_THROW(ReaderDocument::from_file("test/data/data-corrupt" + GetParam(), ErrorHandler::IGNORE), ReaderError);
}

TEST(ReaderDocumentTest, from_file__format)
{
#ifdef PRIO_USE_JSONCPP
  EXPECT_THROW(ReaderDocument::from_file(Format::JSON, "test/data/data.sexp"), ReaderError);
  EXPECT_NO_THROW(ReaderDocument::from_file(Format::JSON, "test/data/data.json"));
#endif
#ifdef PRIO_USE_SEXPCPP
  EXPECT_THROW(ReaderDocument::from_file(Format::SEXPR, "test/data/data.json"), ReaderError);
  EXPECT_NO_THROW(ReaderDocument::from_file(Format::SEXPR, "test/data/data.sexp"));
#endif
}

TEST_P(ReaderDocumentTest, get_filename)
{
  ReaderDocument doc = ReaderDocument::from_file("test/data/data" + GetParam());
  EXPECT_EQ(doc.get_filename(), "test/data/data" + GetParam());
}

TEST_P(ReaderDocumentTest, get_directory)
{
  ReaderDocument doc = ReaderDocument::from_file("test/data/data" + GetParam());
  EXPECT_EQ(doc.get_directory(), "test/data");
}

TEST_P(ReaderDocumentTest, get_root)
{
  ReaderDocument doc = ReaderDocument::from_file("test/data/data" + GetParam());
  EXPECT_EQ(doc.get_root().get_name(), "test-document");
}

#if defined(PRIO_USE_SEXPCPP) && defined(PRIO_USE_JSONCPP)
INSTANTIATE_TEST_CASE_P(ParamReaderDocumentTest, ReaderDocumentTest,
                        ::testing::Values(".sexp", ".json"));
#elif defined(PRIO_USE_SEXPCPP)
INSTANTIATE_TEST_CASE_P(ParamReaderDocumentTest, ReaderDocumentTest,
                        ::testing::Values(".sexp"));
#elif defined(PRIO_USE_JSONCPP)
INSTANTIATE_TEST_CASE_P(ParamReaderDocumentTest, ReaderDocumentTest,
                        ::testing::Values(".json"));
#endif


#ifdef PRIO_USE_JSONCPP
TEST(ReaderDocumentTest, parse_many_json_lines)
{
  auto docs = ReaderDocument::parse_many("test/data/many.jsonl");
  ASSERT_EQ(docs.size(), 3u);
  EXPECT_EQ(docs[0].get_root().get_name(), "doc-a");
  EXPECT_EQ(docs[1].get_root().get_name(), "doc-b");
  EXPECT_EQ(docs[2].get_root().get_name(), "doc-c");

  int id = 0;
  ASSERT_TRUE(docs[0].get_root().get_mapping().read("id", id));
  EXPECT_EQ(id, 1);
}

TEST(ReaderDocumentTest, parse_many_json_compact)
{
  // Concatenated JSON values without newlines between them
  auto docs = ReaderDocument::parse_many("test/data/many-compact.json");
  ASSERT_EQ(docs.size(), 2u);
  EXPECT_EQ(docs[0].get_root().get_name(), "doc-a");
  EXPECT_EQ(docs[1].get_root().get_name(), "doc-b");
}
#endif

#ifdef PRIO_USE_SEXPCPP
TEST(ReaderDocumentTest, parse_many_sexp)
{
  auto docs = ReaderDocument::parse_many("test/data/many.sexp");
  ASSERT_EQ(docs.size(), 3u);
  EXPECT_EQ(docs[0].get_root().get_name(), "doc-a");
  EXPECT_EQ(docs[1].get_root().get_name(), "doc-b");
  EXPECT_EQ(docs[2].get_root().get_name(), "doc-c");
}
#endif

TEST(ReaderDocumentTest, parse_many_missing_file)
{
  EXPECT_THROW(ReaderDocument::parse_many("does-not-exist"), ReaderError);
}

/* EOF */
