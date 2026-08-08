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

#include <iostream>
#include <fmt/format.h>

#include <prio/prio.hpp>

using namespace prio;

namespace {

void write(Writer& writer, ReaderMapping const& body, std::string_view key);

void write(Writer& writer, ReaderMapping const& body)
{
  for(auto const& key : body.get_keys()) {
    try {
      write(writer, body, key);
    } catch(ReaderError const& err) {
      std::cerr << "error while processing '" << key << "': " << err.what() << std::endl;
    }
  }
}

void write(Writer& writer, ReaderMapping const& body, std::string_view key)
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

  if (body.read(key, bool_value)) {
    writer.write(key, bool_value);
  } else  if (body.read(key, int_value)) {
    writer.write(key, int_value);
  } else if (body.read(key, float_value)) {
    writer.write(key, float_value);
  } else if (body.read(key, string_value)) {
    writer.write(key, string_value);
  }

  else if (body.read(key, bool_values)) {
    writer.write(key, bool_values);
  } else  if (body.read(key, int_values)) {
    writer.write(key, int_values);
  } else if (body.read(key, float_values)) {
    writer.write(key, float_values);
  } else if (body.read(key, string_values)) {
    writer.write(key, string_values);
  }

  else if (body.read(key, mapping)) {
    writer.begin_mapping(key);
    write(writer, mapping);
    writer.end_mapping();
  } else if (body.read(key, collection)) {
    writer.begin_collection(key);
    for (auto const& obj : collection.get_objects()) {
      writer.begin_object(obj.get_name());
      write(writer, obj.get_mapping());
      writer.end_object();
    }
    writer.end_collection();
  } else if (body.read(key, object)) {
    writer.begin_keyvalue(key);
    writer.begin_object(object.get_name());
    write(writer, object.get_mapping());
    writer.end_object();
    writer.end_keyvalue();
  } else {
    std::cerr << "unknown thing at key: " << key << std::endl;
  }
}

struct Options
{
  Format format = Format::AUTO;
  std::vector<std::string> files = {};
};

void print_usage(char const* arg0)
{
  std::cout << "Usage: " << arg0 << "[OPTION]... [FILE]...\n"
            << "Little toy format converter\n"
            << "\n"
            << "  --help        Display this help text\n"
            << "  --json        Output pretty json\n"
            << "  --fastjson    Output fastjson\n"
            << "  --sexp        Output s-expressions\n";
}

Options parse_args(int argc, char** argv)
{
  Options opts;

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-' && strlen(argv[i]) > 1) {
      if (strcmp(argv[i], "--help") == 0) {
        print_usage(argv[0]);
      } else if (strcmp(argv[i], "--json") == 0) {
        opts.format = Format::JSON;
      } else if (strcmp(argv[i], "--fastjson") == 0) {
        opts.format = Format::FASTJSON;
      } else if (strcmp(argv[i], "--sexp") == 0) {
        opts.format = Format::SEXPR;
      } else {
        throw std::runtime_error(fmt::format("invalid argument {}", argv[i]));
      }
    } else {
      opts.files.emplace_back(argv[i]);
    }
  }

  return opts;
}

} // namespace

int main(int argc, char** argv)
{
  bool errors = false;
  try {
    Options opts = parse_args(argc, argv);

    for (auto const& filename : opts.files) {
      try {
        ReaderDocument doc = (filename == "-") ?
          ReaderDocument::from_stream(std::cin, ErrorHandler::IGNORE) :
          ReaderDocument::from_file(filename, ErrorHandler::IGNORE);

        ReaderObject const& root = doc.get_root();

        Writer writer = Writer::from_stream(opts.format, std::cout);
        writer.begin_object(root.get_name());
        write(writer, root.get_mapping());
        writer.end_object();
      } catch (std::exception& err) {
        std::cerr << filename << ": " << err.what() << std::endl;
        errors = true;
      }
    }
  } catch (std::exception const& err) {
    std::cerr << err.what() << std::endl;
    errors = true;
  }

  if (errors) {
    return EXIT_FAILURE;
  } else {
    return EXIT_SUCCESS;
  }
}

/* EOF */
