// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HEADER_PINGUS_UTIL_SIGNAL_HPP
#define HEADER_PINGUS_UTIL_SIGNAL_HPP

#include <algorithm>
#include <functional>
#include <memory>
#include <utility>
#include <vector>

namespace pingus {

namespace detail {

struct SlotState
{
  bool connected = true;
};

} // namespace detail

/** Handle to a slot connected to a Signal. Disconnecting is safe after
    the Signal is gone and while it is being emitted. Dropping the
    Connection does not disconnect the slot. */
class Connection
{
public:
  Connection() : m_state() {}
  explicit Connection(std::weak_ptr<detail::SlotState> state) :
    m_state(std::move(state))
  {}

  void disconnect()
  {
    if (auto state = m_state.lock()) {
      state->connected = false;
    }
    m_state.reset();
  }

  bool connected() const
  {
    auto state = m_state.lock();
    return state && state->connected;
  }

private:
  std::weak_ptr<detail::SlotState> m_state;
};

template<typename Signature>
class Signal;

/** Minimal signal/slot: connect() callables, call the Signal to invoke
    all of them in connection order. */
template<typename... Args>
class Signal<void (Args...)>
{
public:
  using Slot = std::function<void (Args...)>;

  Signal() : m_entries() {}
  Signal(Signal const&) = delete;
  Signal& operator=(Signal const&) = delete;

  Connection connect(Slot slot)
  {
    std::erase_if(m_entries, [](auto const& entry) { return !entry->connected; });

    auto entry = std::make_shared<Entry>();
    entry->slot = std::move(slot);
    m_entries.push_back(entry);
    return Connection(entry);
  }

  void operator()(Args... args) const
  {
    // Iterate over a copy: slots may connect or disconnect while the
    // signal is being emitted.
    auto const entries = m_entries;
    for (auto const& entry : entries) {
      if (entry->connected) {
        entry->slot(args...);
      }
    }
  }

private:
  struct Entry : detail::SlotState
  {
    Slot slot = {};
  };

  std::vector<std::shared_ptr<Entry>> m_entries;
};

} // namespace pingus

#endif

/* EOF */
