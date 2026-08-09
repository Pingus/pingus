#ifndef PINGUS_ANDROID_SIGC_CONNECTION_H_
#define PINGUS_ANDROID_SIGC_CONNECTION_H_

#include <functional>
#include <utility>

namespace sigc {

class connection
{
public:
  connection() = default;
  explicit connection(std::function<void()> disconnect_fn)
    : disconnect_(std::move(disconnect_fn))
  {}

  void disconnect()
  {
    if (disconnect_) {
      disconnect_();
      disconnect_ = nullptr;
    }
  }

  explicit operator bool() const { return static_cast<bool>(disconnect_); }

private:
  std::function<void()> disconnect_;
};

} // namespace sigc

#endif
