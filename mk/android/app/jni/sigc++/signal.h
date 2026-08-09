// Minimal sigc++-2 subset for Android ndk-build (no full libsigc++).
#ifndef PINGUS_ANDROID_SIGC_SIGNAL_H_
#define PINGUS_ANDROID_SIGC_SIGNAL_H_

#include <functional>
#include <memory>
#include <vector>
#include <utility>

#include <sigc++/connection.h>

namespace sigc {

template<typename Signature>
class signal;

template<typename R, typename... Args>
class signal<R(Args...)>
{
public:
  using slot_type = std::function<R(Args...)>;

  signal() : slots_() {}

  connection connect(slot_type slot)
  {
    auto entry = std::make_shared<slot_type>(std::move(slot));
    slots_.push_back(entry);
    std::weak_ptr<slot_type> weak = entry;
    return connection([weak]() {
      if (auto s = weak.lock())
        *s = nullptr;
    });
  }

  void emit(Args... args) const
  {
    for (auto const& s : slots_) {
      if (s && *s)
        (*s)(args...);
    }
  }

  void operator()(Args... args) const { emit(args...); }

private:
  mutable std::vector<std::shared_ptr<slot_type>> slots_;
};

} // namespace sigc

#endif
