#include <iostream>
#include <stdlib.h>

#include "pingus/pingus_level.hpp"
#include "util/pathname.hpp"
#include "util/raise_exception.hpp"

using namespace pingus;

namespace {

void
emit_msgid(std::string const& str)
{
  std::cout
    << "#: " << "filename" << ":" << "lineno\n"
    << "msgid \"" << str << "\"\n"
    << "msgstr \"\"\n" << std::endl;
}

int po_extract_main(int argc, char** argv)
{
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
      auto doc = ReaderDocument::from_file(filename.get_sys_path());
      ReaderMapping reader = doc.get_mapping();

      std::string tmp;
      if (reader.read("title", tmp))
      {
        emit_msgid(tmp);
      }

      ReaderCollection all_pages = reader.get<ReaderCollection>("pages");
      auto const& childs = all_pages.get_objects();
      for(auto it = childs.begin(); it != childs.end(); ++it)
      {
        ReaderMapping r = it->get_mapping();
        if (r.read("text", tmp))
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

} // namespace

int main(int argc, char** argv)
{
  try {
    return po_extract_main(argc, argv);
  } catch(std::exception const& err) {
    std::cerr << "error: " << err.what() << std::endl;
    return EXIT_FAILURE;
  } catch(...) {
    std::cerr << "unknown error" << std::endl;
    return EXIT_FAILURE;
  }
}

/* EOF */

