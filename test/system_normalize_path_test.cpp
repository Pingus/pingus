#include <assert.h>

#include "util/system.hpp"

int main(int argc, char** argv)
{
  assert(System::normalize_path("") == "");
  assert(System::normalize_path("/") == "/");
  assert(System::normalize_path("abc") == "abc");

  assert(System::normalize_path("/abc") == "/abc");
  assert(System::normalize_path("/../..") == "/");
  assert(System::normalize_path("a/../..") == "..");
  assert(System::normalize_path("a////c") == "a/c");
  assert(System::normalize_path("a//..///c") == "c");
  assert(System::normalize_path("/a//..///c") == "/c");
  assert(System::normalize_path("/a/../../b/c") == "/b/c");
  assert(System::normalize_path("../foo/bar/") == "../foo/bar");

  return 0;
}

/* EOF */
