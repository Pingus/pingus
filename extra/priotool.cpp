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

#include <cctype>
#include <cstdlib>
#include <cstring>
#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <prio/prio.hpp>

using namespace prio;

namespace {

// ---------------------------------------------------------------------------
// Writer-based conversion (existing behaviour)
// ---------------------------------------------------------------------------

void write(Writer& writer, ReaderMapping const& body, std::string_view key);

void write(Writer& writer, ReaderMapping const& body)
{
  for (auto const& key : body.get_keys()) {
    try {
      write(writer, body, key);
    } catch (ReaderError const& err) {
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
  } else if (body.read(key, int_value)) {
    writer.write(key, int_value);
  } else if (body.read(key, float_value)) {
    writer.write(key, float_value);
  } else if (body.read(key, string_value)) {
    writer.write(key, string_value);
  }

  else if (body.read(key, bool_values)) {
    writer.write(key, bool_values);
  } else if (body.read(key, int_values)) {
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

// ---------------------------------------------------------------------------
// Linearize: one path = value line per leaf (grep-friendly)
//
// Path syntax:
//   - mapping keys joined with '.'
//   - collection / homogeneous-array indices as [N]
//   - named objects (in collections or as nested objects) add their name
//     as a path segment after the parent key / index
//
// Examples:
//   server.host = example.com
//   server.tls.enabled = true
//   items[0].wall.texture = brick.png
//   flags[0] = true
//   tags[1] = "hello world"
//   empty-object =
// ---------------------------------------------------------------------------

bool path_segment_needs_brackets(std::string_view key)
{
  if (key.empty()) {
    return true;
  }
  // Safe unquoted segment: identifier-like, no '.' or '[' that would break paths
  for (char c : key) {
    if (!(std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '-')) {
      return true;
    }
  }
  return false;
}

std::string append_key(std::string const& path, std::string_view key)
{
  std::string segment;
  if (path_segment_needs_brackets(key)) {
    // Bracket-quote awkward keys so '.' inside a key cannot be mistaken for
    // a path separator: parent["key.with.dots"]
    segment = "[\"";
    for (char c : key) {
      if (c == '"' || c == '\\') {
        segment.push_back('\\');
      }
      segment.push_back(c);
    }
    segment += "\"]";
    if (path.empty()) {
      return segment;
    }
    return path + segment;
  }

  if (path.empty()) {
    return std::string(key);
  }
  return path + "." + std::string(key);
}

std::string append_index(std::string const& path, std::size_t index)
{
  return path + "[" + std::to_string(index) + "]";
}

bool string_value_needs_quotes(std::string_view value)
{
  if (value.empty()) {
    return true;
  }
  for (char c : value) {
    if (!(std::isalnum(static_cast<unsigned char>(c))
          || c == '_' || c == '-' || c == '.' || c == '/' || c == ':'
          || c == '@' || c == '+' || c == '%')) {
      return true;
    }
  }
  return false;
}

void write_string_value(std::ostream& out, std::string_view value)
{
  if (!string_value_needs_quotes(value)) {
    out << value;
    return;
  }
  out << '"';
  for (char c : value) {
    if (c == '"' || c == '\\') {
      out << '\\';
    }
    out << c;
  }
  out << '"';
}

void linearize_object(std::ostream& out, std::string const& path, ReaderObject const& object);
void linearize_mapping(std::ostream& out, std::string const& path, ReaderMapping const& body);
void linearize_key(std::ostream& out, std::string const& path, ReaderMapping const& body,
                   std::string_view key);

void emit_line(std::ostream& out, std::string const& path, std::string_view value_text)
{
  out << path << " = " << value_text << '\n';
}

void linearize_object(std::ostream& out, std::string const& path_prefix, ReaderObject const& object)
{
  // path_prefix is the path up to (but not including) this object's name.
  // Root: path_prefix empty -> path is just the object name.
  // Collection element: path_prefix is "...collection[0]" -> "...collection[0].name"
  std::string const path = path_prefix.empty()
    ? object.get_name()
    : append_key(path_prefix, object.get_name());

  ReaderMapping const mapping = object.get_mapping();
  std::vector<std::string> const keys = mapping.get_keys();
  if (keys.empty()) {
    // Empty object: still emit a line so the name is visible to grep.
    emit_line(out, path, "");
    return;
  }
  for (std::string const& key : keys) {
    try {
      linearize_key(out, path, mapping, key);
    } catch (ReaderError const& err) {
      std::cerr << "error while processing '" << path << "." << key << "': "
                << err.what() << std::endl;
    }
  }
}

void linearize_mapping(std::ostream& out, std::string const& path, ReaderMapping const& body)
{
  for (std::string const& key : body.get_keys()) {
    try {
      linearize_key(out, path, body, key);
    } catch (ReaderError const& err) {
      std::cerr << "error while processing '" << path << "." << key << "': "
                << err.what() << std::endl;
    }
  }
}

void linearize_key(std::ostream& out, std::string const& path, ReaderMapping const& body,
                   std::string_view key)
{
  std::string const child = append_key(path, key);

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
    emit_line(out, child, bool_value ? "true" : "false");
  } else if (body.read(key, int_value)) {
    emit_line(out, child, std::to_string(int_value));
  } else if (body.read(key, float_value)) {
    emit_line(out, child, std::format("{}", float_value));
  } else if (body.read(key, string_value)) {
    out << child << " = ";
    write_string_value(out, string_value);
    out << '\n';
  }

  else if (body.read(key, bool_values)) {
    for (std::size_t i = 0; i < bool_values.size(); ++i) {
      emit_line(out, append_index(child, i), bool_values[i] ? "true" : "false");
    }
  } else if (body.read(key, int_values)) {
    for (std::size_t i = 0; i < int_values.size(); ++i) {
      emit_line(out, append_index(child, i), std::to_string(int_values[i]));
    }
  } else if (body.read(key, float_values)) {
    for (std::size_t i = 0; i < float_values.size(); ++i) {
      emit_line(out, append_index(child, i), std::format("{}", float_values[i]));
    }
  } else if (body.read(key, string_values)) {
    for (std::size_t i = 0; i < string_values.size(); ++i) {
      out << append_index(child, i) << " = ";
      write_string_value(out, string_values[i]);
      out << '\n';
    }
  }

  else if (body.read(key, mapping)) {
    if (mapping.get_keys().empty()) {
      emit_line(out, child, "");
    } else {
      linearize_mapping(out, child, mapping);
    }
  } else if (body.read(key, collection)) {
    std::vector<ReaderObject> const objects = collection.get_objects();
    if (objects.empty()) {
      emit_line(out, child, "[]");
    } else {
      for (std::size_t i = 0; i < objects.size(); ++i) {
        // collection[i].ObjectName.prop = ...
        linearize_object(out, append_index(child, i), objects[i]);
      }
    }
  } else if (body.read(key, object)) {
    // key holds a single named object: path.key.ObjectName.prop = ...
    linearize_object(out, child, object);
  } else {
    std::cerr << "unknown thing at key: " << child << std::endl;
  }
}

// ---------------------------------------------------------------------------
// CLI
// ---------------------------------------------------------------------------

struct Options
{
  Format format = Format::AUTO;
  bool linearize = false;
  std::vector<std::string> files = {};
};

void print_usage(char const* arg0)
{
  std::cout << "Usage: " << arg0 << " [OPTION]... [FILE]...\n"
            << "Little toy format converter / inspector\n"
            << "\n"
            << "  --help         Display this help text\n"
            << "  --version      Display version information\n"
            << "  --json         Output pretty json\n"
            << "  --fastjson     Output fastjson\n"
            << "  --sexp         Output s-expressions\n"
            << "  --linearize    Flatten to path = value lines (grep-friendly)\n"
            << "  -l             Same as --linearize\n"
            << "\n"
            << "Linearize path syntax:\n"
            << "  mapping keys are joined with '.'\n"
            << "  collection / array indices use [N]\n"
            << "  awkward keys use [\"...\"]\n"
            << "  bools are true/false; strings are quoted when needed\n"
            << "  empty objects emit 'path =' with an empty value\n"
            << "\n"
            << "  server.host = example.com\n"
            << "  server.tls.enabled = true\n"
            << "  items[0].wall.texture = brick.png\n"
            << "  flags[1] = false\n";
}

Options parse_args(int argc, char** argv)
{
  Options opts;

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-' && strlen(argv[i]) > 1) {
      if (strcmp(argv[i], "--help") == 0) {
        print_usage(argv[0]);
        exit(EXIT_SUCCESS);
      } else if (strcmp(argv[i], "--version") == 0) {
        std::cout << "priotool " << PRIO_VERSION << std::endl;
        exit(EXIT_SUCCESS);
      } else if (strcmp(argv[i], "--json") == 0) {
        opts.format = Format::JSON;
      } else if (strcmp(argv[i], "--fastjson") == 0) {
        opts.format = Format::FASTJSON;
      } else if (strcmp(argv[i], "--sexp") == 0) {
        opts.format = Format::SEXPR;
      } else if (strcmp(argv[i], "--linearize") == 0 || strcmp(argv[i], "-l") == 0) {
        opts.linearize = true;
      } else {
        throw std::runtime_error(std::format("invalid argument {}", argv[i]));
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

        if (opts.linearize) {
          linearize_object(std::cout, "", root);
        } else {
          Writer writer = Writer::from_stream(opts.format, std::cout);
          writer.begin_object(root.get_name());
          write(writer, root.get_mapping());
          writer.end_object();
        }
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
