// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later
//
// GCC 15+ headers emit references to libstdc++ / libgcc symbols that are
// absent from the ArkOS sysroot (glibc ~2.30, libstdc++ from GCC ~9).
// Minimal shims so the R36S binary links and runs without shipping a modern
// libstdc++ (which would also demand a newer glibc).

// GCC 16's <format> declares basic_format_arg::_M_handle_unrecognized()
// as an extern template, defined only in a modern libstdc++.so
// (format-inst.cc). Turn the extern template declarations off for this
// file, as libstdc++'s own *-inst.cc files do, so the explicit
// instantiation below emits the definition from the header.
#include <bits/c++config.h>
#if defined(_GLIBCXX_RELEASE) && _GLIBCXX_RELEASE >= 16
#  undef _GLIBCXX_EXTERN_TEMPLATE
#  define _GLIBCXX_EXTERN_TEMPLATE 0
#  define PINGUS_SHIM_FORMAT_ARG 1
#endif

#include <link.h>

#include <algorithm>
#include <charconv>
#include <cstdio>
#include <exception>
#include <new>
#include <system_error>

#include <SDL.h>

#ifdef PINGUS_SHIM_FORMAT_ARG
#  include <format>

namespace std {
template basic_format_arg<format_context>::handle
basic_format_arg<format_context>::_M_handle_unrecognized() const;
} // namespace std
#endif

// ---------------------------------------------------------------------------
// libgcc_eh (static, from the modern GCC) looks up unwind info only through
// glibc 2.35's _dl_find_object(); when that fails it has no fallback, and
// _Unwind_Resume aborts. A stub returning -1 therefore killed every
// exception that passed through a cleanup (destructor) frame. Implement it
// on top of dl_iterate_phdr(), which glibc 2.30 has: find the object whose
// PT_LOAD segments contain the address and return its PT_GNU_EH_FRAME.
// Layout as in modern glibc's <dlfcn.h> for aarch64 (no eh_dbase/eh_count).
// ---------------------------------------------------------------------------
struct dl_find_object
{
  unsigned long long dlfo_flags;
  void* dlfo_map_start;
  void* dlfo_map_end;
  struct link_map* dlfo_link_map;
  void* dlfo_eh_frame;
  void* dlfo_sframe;
  unsigned long long dlfo_reserved[6];
};

namespace {

struct FindObjectQuery
{
  ElfW(Addr) address;
  dl_find_object* result;
  bool found;
};

int find_object_callback(dl_phdr_info* info, size_t /*size*/, void* data)
{
  auto* const query = static_cast<FindObjectQuery*>(data);

  bool contains = false;
  ElfW(Addr) map_start = ~ElfW(Addr){0};
  ElfW(Addr) map_end = 0;
  ElfW(Addr) eh_frame = 0;
  for (ElfW(Half) i = 0; i < info->dlpi_phnum; ++i)
  {
    ElfW(Phdr) const& phdr = info->dlpi_phdr[i];
    if (phdr.p_type == PT_LOAD)
    {
      ElfW(Addr) const start = info->dlpi_addr + phdr.p_vaddr;
      ElfW(Addr) const end = start + phdr.p_memsz;
      map_start = std::min(map_start, start);
      map_end = std::max(map_end, end);
      if (start <= query->address && query->address < end) {
        contains = true;
      }
    }
    else if (phdr.p_type == PT_GNU_EH_FRAME)
    {
      eh_frame = info->dlpi_addr + phdr.p_vaddr;
    }
  }

  if (!contains) {
    return 0;
  }

  *query->result = dl_find_object{};
  query->result->dlfo_map_start = reinterpret_cast<void*>(map_start);
  query->result->dlfo_map_end = reinterpret_cast<void*>(map_end);
  query->result->dlfo_eh_frame = reinterpret_cast<void*>(eh_frame);
  query->found = true;
  return 1;
}

} // namespace

extern "C" int _dl_find_object(void* address, struct dl_find_object* result)
{
  FindObjectQuery query{reinterpret_cast<ElfW(Addr)>(address), result, false};
  dl_iterate_phdr(find_object_callback, &query);
  return query.found ? 0 : -1;
}

extern "C" void __cxa_call_terminate(void*) noexcept
{
  std::terminate();
}

// Some SDL2_image builds expose IMG_GetError only as a macro → SDL_GetError.
// Older headers declare a real function; the .so then has no such symbol.
extern "C" const char* IMG_GetError(void)
{
  return SDL_GetError();
}

namespace std {

__attribute__((noreturn)) void
__throw_bad_array_new_length()
{
  throw bad_array_new_length();
}

namespace {

to_chars_result
snprintf_to_chars(char* first, char* last, char const* fmt, long double value) noexcept
{
  if (first == nullptr || last == nullptr || first > last)
  {
    return {last, errc::value_too_large};
  }
  auto const n = std::snprintf(first, static_cast<size_t>(last - first), fmt,
                               static_cast<double>(value));
  if (n < 0 || n >= static_cast<int>(last - first))
  {
    return {last, errc::value_too_large};
  }
  return {first + n, errc{}};
}

char const*
format_to_printf(chars_format fmt) noexcept
{
  switch (fmt)
  {
    case chars_format::scientific:
      return "%e";
    case chars_format::fixed:
      return "%f";
    case chars_format::hex:
      return "%a";
    case chars_format::general:
    default:
      return "%g";
  }
}

} // namespace

// Floating to_chars used by libstdc++ std::format; not in GCC ~9 libstdc++.
// Signatures must match <charconv> (noexcept).

to_chars_result
to_chars(char* first, char* last, float value) noexcept
{
  return snprintf_to_chars(first, last, "%g", static_cast<long double>(value));
}

to_chars_result
to_chars(char* first, char* last, double value) noexcept
{
  return snprintf_to_chars(first, last, "%g", static_cast<long double>(value));
}

to_chars_result
to_chars(char* first, char* last, long double value) noexcept
{
  return snprintf_to_chars(first, last, "%g", value);
}

to_chars_result
to_chars(char* first, char* last, float value, chars_format fmt) noexcept
{
  return snprintf_to_chars(first, last, format_to_printf(fmt),
                           static_cast<long double>(value));
}

to_chars_result
to_chars(char* first, char* last, double value, chars_format fmt) noexcept
{
  return snprintf_to_chars(first, last, format_to_printf(fmt),
                           static_cast<long double>(value));
}

to_chars_result
to_chars(char* first, char* last, long double value, chars_format fmt) noexcept
{
  return snprintf_to_chars(first, last, format_to_printf(fmt), value);
}

to_chars_result
to_chars(char* first, char* last, float value, chars_format fmt, int precision) noexcept
{
  char buf[16];
  std::snprintf(buf, sizeof(buf), "%%.%d%c", precision,
                (fmt == chars_format::scientific) ? 'e' :
                (fmt == chars_format::fixed) ? 'f' :
                (fmt == chars_format::hex) ? 'a' : 'g');
  return snprintf_to_chars(first, last, buf, static_cast<long double>(value));
}

to_chars_result
to_chars(char* first, char* last, double value, chars_format fmt, int precision) noexcept
{
  char buf[16];
  std::snprintf(buf, sizeof(buf), "%%.%d%c", precision,
                (fmt == chars_format::scientific) ? 'e' :
                (fmt == chars_format::fixed) ? 'f' :
                (fmt == chars_format::hex) ? 'a' : 'g');
  return snprintf_to_chars(first, last, buf, static_cast<long double>(value));
}

to_chars_result
to_chars(char* first, char* last, long double value, chars_format fmt, int precision) noexcept
{
  char buf[16];
  std::snprintf(buf, sizeof(buf), "%%.%d%c", precision,
                (fmt == chars_format::scientific) ? 'e' :
                (fmt == chars_format::fixed) ? 'f' :
                (fmt == chars_format::hex) ? 'a' : 'g');
  return snprintf_to_chars(first, last, buf, value);
}

} // namespace std
