// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "util/system.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <stdlib.h>
#include <string.h>

#ifdef __APPLE__
#include <CoreFoundation/CFLocale.h>
#include <CoreFoundation/CFString.h>
#endif

#ifndef _WIN32
#  include <dirent.h>
#  include <fcntl.h>
#  include <sys/stat.h>
#  include <sys/types.h>
#  include <unistd.h>
#  include <errno.h>
#  include <xdg.h>
#else /* _WIN32 */
#  define NOGDI
#  include <windows.h>
#  include <direct.h>
#  include <sys/stat.h>
#  include <sys/types.h>
#  include <io.h>
#define chdir  _chdir
#define access _access
#define F_OK   0
#endif

#include <logmich/log.hpp>

#include "pingus/globals.hpp"
#include "util/pathname.hpp"
#include "util/raise_exception.hpp"

namespace pingus {

std::string System::userdir;
std::string System::default_email;
std::string System::default_username;

System::DirectoryEntry::DirectoryEntry(std::string const& n, FileType t)
  : type (t), name (n)
{
}

std::string
System::cut_file_extension(std::string const& filename)
{
  for(int i = static_cast<int>(filename.size()) - 1; i >= 0; --i)
  {
    if (filename[static_cast<size_t>(i)] == '.')
    {
      return filename.substr(0, static_cast<size_t>(i));
    }
    else if (filename[static_cast<size_t>(i)] == '/')
    {
      return filename;
    }
  }

  return filename;
}

std::string
System::get_file_extension(std::string const& filename)
{
  for(int i = static_cast<int>(filename.size()) - 1; i >= 0; --i)
  {
    if (filename[i] == '.')
    {
      return filename.substr(i+1);
    }
    else if (filename[i] == '/')
    {
      return std::string();
    }
  }

  return std::string();
}

System::Directory
System::opendir(std::string const& pathname)
{
  Directory dir_list;

#ifndef WIN32
  DIR* dp;
  dirent* de;

  dp = ::opendir(pathname.c_str());

  if (dp == nullptr)
  {
    raise_exception(std::runtime_error, pathname << ": " << strerror(errno));
  }
  else
  {
    while ((de = ::readdir(dp)) != nullptr)
    {
      struct stat buf;
      stat((Pathname::join(pathname, de->d_name)).c_str(), &buf);

      if (strcmp(de->d_name, "..") != 0 &&
          strcmp(de->d_name, ".") != 0)
      {
        if (S_ISDIR(buf.st_mode))
        {
          dir_list.push_back(DirectoryEntry(de->d_name, DE_DIRECTORY));
        }
        else
        {
          dir_list.push_back(DirectoryEntry(de->d_name, DE_FILE));
        }
      }
    }

    closedir(dp);
  }
#else /* WIN32 */
  WIN32_FIND_DATA coFindData;
  HANDLE hFind = FindFirstFile(TEXT(pathname.c_str()), &coFindData);

  if (hFind == INVALID_HANDLE_VALUE)
  {
    if (GetLastError() != ERROR_FILE_NOT_FOUND)
      log_error("System: Couldn't open: {}", pathname);
  }
  else
  {
    do
    {
      if (strcmp(coFindData.cFileName, "..") != 0 &&
          strcmp(coFindData.cFileName, ".") != 0)
      {
        if (coFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
          dir_list.push_back(DirectoryEntry(coFindData.cFileName, System::DE_DIRECTORY));
        }
        else
        {
          dir_list.push_back(DirectoryEntry(coFindData.cFileName, System::DE_FILE));
        }
      }
    }
    while (FindNextFile(hFind,&coFindData));

    FindClose(hFind);
  }
#endif

  return dir_list;
}

std::vector<std::string>
System::opendir_recursive(std::string const& pathname)
{
  std::vector<std::string> lst;
  try
  {
    Directory dir = opendir(pathname);
    for(auto it = dir.begin(); it != dir.end(); ++it)
    {
      if (it->type == DE_DIRECTORY)
      {
        std::vector<std::string> subdir = opendir_recursive(Pathname::join(pathname, it->name));
        lst.insert(lst.end(), subdir.begin(), subdir.end());
      }
      else if (it->type == DE_FILE)
      {
        lst.push_back(Pathname::join(pathname, it->name));
      }
    }
  }
  catch(std::exception const& err)
  {
    log_warn("{}", err.what());
  }
  return lst;
}

// Returns the basic filename without the path
std::string
System::basename(std::string const& filename)
{
  // Should be replaced with STL
  char const* str = filename.c_str();
  int i;

  for(i = static_cast<int>(filename.size()) - 1; i >= 0; --i)
  {
    if (*(str + i) == '/') {
      break;
    }
  }

  return (str+i + 1);
}

std::string
System::dirname (std::string const& filename)
{
  char const* str = filename.c_str();
  int i;

  for(i = static_cast<int>(filename.size()) - 1; i >= 0; --i)
  {
    if (*(str + i) == '/') {
      break;
    }
  }

  return filename.substr(0, static_cast<size_t>(i));
}

bool
System::exist(std::string const& filename)
{
  return !access(filename.c_str(), F_OK);
}

void
System::create_dir(std::string const& directory_)
{
  std::string directory = directory_;
#ifndef WIN32
  if (!exist(directory))
  {
    auto end = directory.end() - 1;
    if(*end == '/') {
      directory.erase(end);
    }
    log_info("System::create_dir: %1", directory);
    if (!std::filesystem::create_directories(directory.c_str()))
    {
      raise_exception(std::runtime_error, "System::create_dir: " << directory << ": " << strerror(errno));
    }
    else
    {
      log_info("Successfully created: {}", directory);
    }
  }
#else
  if (!CreateDirectory(directory.c_str(), nullptr))
  {
    DWORD dwError = GetLastError();
    if (dwError == ERROR_ALREADY_EXISTS)
    {
    }
    else if (dwError == ERROR_PATH_NOT_FOUND)
    {
      raise_exception(std::runtime_error,
                      "CreateDirectory: " << directory <<
                      ": One or more intermediate directories do not exist; this function will only create the final directory in the path.");
    }
    else
    {
      raise_exception(std::runtime_error,
                      "CreateDirectory: " << directory << ": failed with error " << std::to_string(dwError));
    }
  }
  else
  {
    log_info("Successfully created: {}", directory);
  }
#endif
}

std::string
System::find_userdir()
{
#ifdef WIN32
  std::string tmpstr;
  char* appdata  = getenv("APPDATA");
  if (appdata)
  {
    tmpstr = std::string(appdata) + "/Pingus-0.8/";
    for (size_t pos = tmpstr.find('\\', 0); pos != std::string::npos; pos = tmpstr.find('\\', 0))
      tmpstr[pos] = '/';
  }
  else
  {
    tmpstr = "user/";
  }

  return tmpstr;

#elif defined(__APPLE__)
  char* homedir = getenv("HOME");

  if (homedir)
  {
    return std::string(homedir) + "/Library/Application Support/pingus-0.8/";
  }
  else
  {
    raise_exception(std::runtime_error, "Environment variable $HOME not set, fix that and start again.");
  }

#else /* !WIN32 */
  // If ~/.pingus/ exist, use that for backward compatibility reasons,
  // if it does not, use $XDG_CONFIG_HOME, see:
  // http://standards.freedesktop.org/basedir-spec/basedir-spec-latest.html

#if 0
  // FIXME: insert code here to handle backward compatibilty with 0.7.x releases
  { // check for ~/.pingus/
    char* homedir = getenv("HOME");
    if (homedir && strcmp(homedir, "") != 0)
    {
      std::string old_pingus_config_dir = std::string(homedir) + "/.pingus/";
      if (exist(old_pingus_config_dir))
      {
        return old_pingus_config_dir;
      }
    }
  }
#endif

  return (xdg::config().home() / "pingus-0.8/").string();
#endif
}

void
System::init_directories()
{
  if (userdir.empty())
    userdir = find_userdir();

  std::string statdir  = get_userdir();

  // FIXME: We need a better seperation between user created levels,
  // FIXME: third party levels and levels from the base distri
  create_dir(statdir);
  create_dir(statdir + "levels/");
  create_dir(statdir + "levels/dist");
  create_dir(statdir + "themes/");

  // Savegames (FIXME: rename to savegames/?)
  create_dir(statdir + "savegames/");

  // User created images
  create_dir(statdir + "images/");

  // Thumbnail cache
  create_dir(statdir + "cache/");

  // Recorded demos will per default be writen in this directory
  create_dir(statdir + "demos/");

  // User created images
  create_dir(statdir + "backup/");

  // Screenshots will be dumped to that directory:
  create_dir(statdir + "screenshots/");
}

void
System::set_userdir(std::string const& u)
{
  userdir = u + "/";
}

std::string
System::get_userdir()
{
  return userdir;
}

std::string
System::get_cachedir()
{
  return get_userdir() + "cache/";
}

/** Returns the username of the current user or an empty string */
std::string
System::get_username()
{
  if (default_username.empty())
  {
    char* username = getenv("USERNAME");

    if (username)
      return std::string(username);
    else
      return "";
  }
  else
  {
    return default_username;
  }
}

/** Returns the EMail of the user or an empty string */
std::string
System::get_email()
{
  if (default_email.empty())
  {
    char* email = getenv("EMAIL");

    if (email)
      // FIXME: $EMAIL contains the complete from header, not only
      // the email address
      return std::string(email);
    else
      return "";
  }
  else
  {
    return default_email;
  }
}

std::string
System::get_language()
{
#ifdef WIN32
  char* lang_c = getenv("LC_MESSAGES");
#else
  char* lang_c = setlocale(LC_MESSAGES, nullptr);
#endif
  std::string lang;

  if (lang_c)
  {
    lang = lang_c;
  }

  if (lang.empty() || lang == "C")
  {
    lang_c = getenv("LANG");
    if (lang_c)
    {
      lang = lang_c;
    }
  }

  if (lang.empty() || lang == "C")
  {
#ifndef __APPLE__
    return globals::default_language;
#else /* on Mac OS X we get "C" if launched using Finder, so we ask the OS for the language */
    /* Note: this is used as last resort to allow the use of LANG when starting via Terminal */
    CFArrayRef preferred_languages = CFLocaleCopyPreferredLanguages();
    //CFShow(preferred_languages);
    CFStringRef language = (CFStringRef)CFArrayGetValueAtIndex(preferred_languages, 0 /* most important language */);
    //CFShow(language);
    CFRelease(preferred_languages);
    CFStringRef substring = CFStringCreateWithSubstring(NULL, language, CFRangeMake(0, 2));
    CFRelease(language);
    char buff[3];
    CFStringGetCString(substring, buff, 3, kCFStringEncodingUTF8);
    CFRelease(substring);
    lang = buff;
    return lang;
#endif
  }
  else
  {
    return lang.substr(0, 2);
  }
}

std::string
System::checksum(Pathname const& pathname)
{
  return checksum(pathname.get_sys_path());
}

/** Read file and create a checksum and return it */
std::string
System::checksum(std::string const& filename)
{ // FIXME: Replace sys with SHA1 or MD5 or so
  FILE* in;
  size_t bytes_read;
  char buffer[4096];
  long int checksum = 0;

  in = fopen(filename.c_str(), "r");

  if (!in)
  {
    log_error("System::checksum: Couldn't open file: {}", filename);
    return "";
  }

  do
  {
    bytes_read = fread(buffer, sizeof (char), 4096, in);

    if (bytes_read != 4096 && !feof(in))
    {
      raise_exception(std::runtime_error, "System:checksum: file read error");
    }

    for (size_t i=0; i < bytes_read; ++i)
      checksum = checksum * 17 + buffer[i];
  }
  while (bytes_read != 0);

  fclose (in);

  std::ostringstream str;
  str << checksum;
  return str.str();
}

uint64_t
System::get_mtime(std::string const& filename)
{
#ifndef WIN32

  struct stat stat_buf;
  if (stat(filename.c_str(), &stat_buf) == 0)
    return static_cast<uint64_t>(stat_buf.st_mtime);
  else
    return 0;

#else

  struct _stat stat_buf;
  if (_stat(filename.c_str(), &stat_buf) == 0)
    return stat_buf.st_mtime;
  else
    return 0;

#endif
}

std::string
System::realpath(std::string const& pathname)
{
  std::string fullpath;

#ifdef WIN32
  std::string drive;
  if (pathname.size() > 2 && pathname[1] == ':' && pathname[2] == '/')
  {
    // absolute path on Win32
    drive = pathname.substr(0, 2);
    fullpath = pathname.substr(2);
  }
#else
  if (!pathname.empty() && pathname[0] == '/')
  {
    // absolute path on Linux
    fullpath = pathname;
  }
#endif
  else
  {
    // relative path
    char* cwd = getcwd(nullptr, 0);
    if (!cwd)
    {
      log_error("System::realpath: Error: couldn't getcwd()");
      return pathname;
    }
    else
    {
#ifdef WIN32
      // unify directory separator to '/'
      for (char *p = cwd; *p; ++p)
      {
        if (*p == '\\')
          *p = '/';
      }
      drive.assign(cwd, 2);
      fullpath = Pathname::join(std::string(cwd+2), pathname);
#else
      fullpath = Pathname::join(std::string(cwd), pathname);
#endif
      free(cwd);
    }
  }

#ifdef WIN32
  return drive + normalize_path(fullpath);
#else
  return normalize_path(fullpath);
#endif
}

namespace {

void handle_directory(std::string const& dir, int& skip, std::string& result)
{
  if (dir.empty() || dir == ".")
  {
    // ignore
  }
  else if (dir == "..")
  {
    skip += 1;
  }
  else
  {
    if (skip == 0)
    {
      if (result.empty())
      {
        result.append(dir);
      }
      else
      {
        result.append("/");
        result.append(dir);
      }
    }
    else
    {
      skip -= 1;
    }
  }
}

} // namespaces

std::string
System::normalize_path(std::string const& path)
{
  if (path.empty())
  {
    return std::string();
  }
  else
  {
    bool absolute = false;
    if (path[0] == '/')
    {
      absolute = true;
    }

    std::string result;
    result.reserve(path.size());

    std::string::const_reverse_iterator last_slash = path.rbegin();
    int skip = 0;

    for(std::string::const_reverse_iterator i = path.rbegin(); i != path.rend(); ++i)
    {
      if (*i == '/')
      {
        std::string dir(last_slash, i);

        handle_directory(dir, skip, result);

        last_slash = i+1;
      }
    }

    // relative path name
    if (last_slash != path.rend())
    {
      std::string dir(last_slash, path.rend());
      handle_directory(dir, skip, result);
    }

    if (!absolute)
    {
      for(int i = 0; i < skip; ++i)
      {
        if (i == skip - 1)
        {
          if (result.empty())
          {
            result.append("..");
          }
          else
          {
            result.append("/..");
          }
        }
        else
        {
          result.append("../");
        }
      }
    }
    else // if (absolute)
    {
      result += "/";
    }

    std::reverse(result.begin(), result.end());
    return result;
  }
}

void
System::write_file(std::string const& filename, std::string const& content)
{
  log_debug("writing {}", filename);

#ifdef WIN32
  // FIXME: not save
  std::ofstream out(filename);
  out.write(content.data(), content.size());
#else
  // build the filename: "/home/foo/outfile.pngXXXXXX"
  std::string tmpfile_str = filename + "XXXXXX";
  std::vector<char> tmpfile(tmpfile_str.begin(), tmpfile_str.end());
  tmpfile.push_back('\0');

  // create a temporary file
  mode_t old_mask = umask(S_IRWXO | S_IRWXG);
  int fd = mkstemp(tmpfile.data());
  umask(old_mask);
  if (fd < 0)
  {
    raise_exception(std::runtime_error, tmpfile.data() << ": " << strerror(errno));
  }

  // write the data to the temporary file
  if (write(fd, content.data(), content.size()) < 0)
  {
    raise_exception(std::runtime_error, tmpfile.data() << ": " << strerror(errno));
  }

  if (close(fd) < 0)
  {
    raise_exception(std::runtime_error, tmpfile.data() << ": " << strerror(errno));
  }

  // rename the temporary file to it's final location
  if (rename(tmpfile.data(), filename.c_str()) < 0)
  {
    raise_exception(std::runtime_error, tmpfile.data() << ": " << strerror(errno));
  }

  // adjust permissions to normal default permissions, as mkstemp
  // might not honor umask
  if (chmod(filename.c_str(), ~old_mask & 0666) < 0)
  {
    raise_exception(std::runtime_error, tmpfile.data() << ": " << strerror(errno));
  }
#endif
}

} // namespace pingus

/* EOF */
