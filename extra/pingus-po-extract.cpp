#include <iostream>

#include "pingus/pingus_level.hpp"
#include "util/pathname.hpp"
#include "util/raise_exception.hpp"

void
emit_msgid(const std::string& str)
{
  std::cout
    << "#: " << "filename" << ":" << "lineno\n"
    << "msgid \"" << str << "\"\n"
    << "msgstr \"\"\n" << std::endl;
}

int main(int argc, char** argv)
{
  std::vector<Pathname> files;

  for(int i = 1; i < argc; ++i)
  {
    Pathname filename(argv[i], Pathname::SYSTEM_PATH);
    if (filename.has_extension(".pingus"))
    {
      PingusLevel plf(filename);

      emit_msgid(plf.get_levelname());
      emit_msgid(plf.get_description());
    }
    else if (filename.has_extension(".worldmap"))
    {
      // worldmaps don't contain translatable strings at the moment
    }
    else if (filename.has_extension(".story"))
    {
      FileReader reader = FileReader::parse(filename);
      std::string tmp;
      if (reader.read_string("title", tmp))
      {
        emit_msgid(tmp);
      }

      FileReader all_pages = reader.read_section("pages");
      const auto& childs = all_pages.get_sections();
      for(auto it = childs.begin(); it != childs.end(); ++it)
      {
        if (it->read_string("text", tmp))
        {
          emit_msgid(tmp);
        }
      }
    }
    else
    {
      raise_exception(std::runtime_error, "unknown file type: " << filename);
    }
  }

  return 0;
}

/* EOF */

