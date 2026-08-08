//  UITest - Simple testing framework for interactive tests
//  Copyright (C) 2015 Ingo Ruhnke <grumbel@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <uitest/uitest.hpp>

namespace {

void print_exception(std::exception const& err, int level = 0)
{
  std::cerr << std::string(static_cast<size_t>(level * 2), ' ') << "exception: " << err.what() << std::endl;
  try
  {
    std::rethrow_if_nested(err);
  }
  catch(std::exception const& nested_err)
  {
    print_exception(nested_err, level + 1);
  }
  catch(...)
  {
    std::cerr << "unknown exception\n";
  }
}

} // namespace

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0] << " TESTCASE [ARG]...\n"
              << "\nTest Cases:\n";
    for(const auto& testinfo : ::uitesting::g_tests())
    {
      std::cout << "  " << testinfo->m_class << "." << testinfo->m_function << " " << testinfo->m_args.str() << std::endl;
    }
    return 0;
  }
  else
  {
    std::string testcase = argv[1];
    ::uitesting::TestInfo* testinfo = ::uitesting::find_testcase(testcase);
    if (!testinfo)
    {
      std::cerr << "Couldn't find test case '" << testcase << "'" << std::endl;
      return 1;
    }
    else
    {
      std::vector<std::string> raw_args(argv+2, argv + argc);
      std::vector<std::string> args;
      std::vector<std::string> rest;

      auto testobj = testinfo->m_factory();
      if (!testinfo->m_args.parse_args(raw_args, args, rest))
      {
        std::cerr << "error: incorrect number of arguments\n\n"
                  << "Usage: " << argv[0] << " " << testinfo->m_name
                  << " " << testinfo->m_args.str() << std::endl;
        if (!testinfo->m_doc.empty())
        {
          std::cerr << "  " << testinfo->m_doc << std::endl;
        }
      }
      else
      {
        try
        {
        testobj->TestBody(args, rest);
        }
        catch(std::exception const& err)
        {
          std::cerr << "uitest " << testcase << " failed with:\n";
          print_exception(err);
        }
        catch(...)
        {
          std::cerr << "uitest " << testcase << " failed with:\n";
          std::cerr << "unknown exception\n";
        }
      }
      return 0;
    }
  }
}

/* EOF */
