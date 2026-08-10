logmich - A Trivial Logging Library
===================================

logmich is a trivial logging library for C++. It lacks anything but
the bare essentials and formats messages with **C++20 `std::format`**
(runtime format strings via `std::vformat`).

Requires a C++20 compiler (or newer).

Usage is as follows:

```cpp
#include <logmich/log.hpp>

// Set the log level at which we want to log
logmich::set_log_level(logmich::LogLevel::INFO);

// output some log messages
log_error("error level log message, number {}", 5);
log_warn("warning level log message");
log_info("info level log message: {}", "filename.jpg");
log_debug("debug level log message [invisible]");
log_trace("trace level log message [invisible]");
```

Format strings follow [`std::format`](https://en.cppreference.com/w/cpp/utility/format/format)
rules (`{}` placeholders, not printf `%`).

Types used as format arguments must be formattable with `std::format`
(built-ins, strings, or a `std::formatter` specialization). Unlike the
old `{fmt}` + `fmt/ostream.h` path, a type is **not** automatically
formattable just because it has `operator<<`.
