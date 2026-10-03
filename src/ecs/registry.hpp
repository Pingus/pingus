// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HEADER_PINGUS_ECS_REGISTRY_HPP
#define HEADER_PINGUS_ECS_REGISTRY_HPP

#include <cassert>
#include <cstdint>
#include <deque>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

namespace pingus::ecs {

/** Handle of an entity. Ids are handed out in increasing order and never
    reused within a Registry, so iteration order is creation order. */
enum class Entity : uint32_t {};

inline constexpr Entity null_entity = Entity{UINT32_MAX};

inline constexpr uint32_t to_index(Entity entity) { return static_cast<uint32_t>(entity); }

/** Minimal entity component registry.

    Components are plain structs, at most one of each type per entity.
    Storage is a per-type std::deque indexed by entity id, so references
    to components stay valid when components are added to other
    entities, but not when the entity itself is destroyed. The game has
    at most a few hundred entities, so simplicity and deterministic
    iteration order win over memory use. */
class Registry
{
private:
  struct PoolBase
  {
    virtual ~PoolBase() {}
    virtual void remove(uint32_t index) = 0;
  };

  template<typename T>
  struct Pool : public PoolBase
  {
    std::deque<std::optional<T>> items;

    Pool() : items() {}

    void remove(uint32_t index) override
    {
      if (index < items.size()) {
        items[index].reset();
      }
    }

    T* get(uint32_t index)
    {
      if (index < items.size() && items[index]) {
        return &*items[index];
      } else {
        return nullptr;
      }
    }
  };

  static uint32_t next_component_id()
  {
    static uint32_t next_id = 0;
    return next_id++;
  }

  template<typename T>
  static uint32_t component_id()
  {
    static uint32_t const id = next_component_id();
    return id;
  }

  template<typename T>
  Pool<T>* get_pool() const
  {
    uint32_t const id = component_id<T>();
    if (id < m_pools.size()) {
      return static_cast<Pool<T>*>(m_pools[id].get());
    } else {
      return nullptr;
    }
  }

  template<typename T>
  Pool<T>& assure_pool()
  {
    uint32_t const id = component_id<T>();
    if (id >= m_pools.size()) {
      m_pools.resize(id + 1);
    }
    if (!m_pools[id]) {
      m_pools[id] = std::make_unique<Pool<T>>();
    }
    return static_cast<Pool<T>&>(*m_pools[id]);
  }

public:
  Registry() :
    m_alive(),
    m_pools()
  {}

  Registry(Registry const&) = delete;
  Registry& operator=(Registry const&) = delete;

  Entity create()
  {
    m_alive.push_back(true);
    return Entity{static_cast<uint32_t>(m_alive.size() - 1)};
  }

  /** Destroy the entity and all its components */
  void destroy(Entity entity)
  {
    assert(valid(entity));
    uint32_t const index = to_index(entity);
    for (auto& pool : m_pools) {
      if (pool) {
        pool->remove(index);
      }
    }
    m_alive[index] = false;
  }

  bool valid(Entity entity) const
  {
    uint32_t const index = to_index(entity);
    return index < m_alive.size() && m_alive[index];
  }

  /** Number of entity ids handed out so far, including destroyed ones */
  size_t capacity() const { return m_alive.size(); }

  template<typename T, typename... Args>
  T& emplace(Entity entity, Args&&... args)
  {
    assert(valid(entity));
    uint32_t const index = to_index(entity);
    Pool<T>& pool = assure_pool<T>();
    if (index >= pool.items.size()) {
      pool.items.resize(index + 1);
    }
    pool.items[index].emplace(T{std::forward<Args>(args)...});
    return *pool.items[index];
  }

  template<typename T>
  void remove(Entity entity)
  {
    if (Pool<T>* pool = get_pool<T>()) {
      pool->remove(to_index(entity));
    }
  }

  template<typename T>
  bool has(Entity entity) const
  {
    return try_get<T>(entity) != nullptr;
  }

  template<typename T>
  T* try_get(Entity entity) const
  {
    Pool<T>* pool = get_pool<T>();
    return pool ? pool->get(to_index(entity)) : nullptr;
  }

  template<typename T>
  T& get(Entity entity) const
  {
    T* component = try_get<T>(entity);
    assert(component && "entity lacks the requested component");
    return *component;
  }

  /** Call func(entity, components&...) for every entity that has all the
      given components, in creation order. Entities may be created or
      destroyed during iteration; newly created entities are visited
      too, destroyed ones are skipped. */
  template<typename... Ts, typename Func>
  void each(Func&& func)
  {
    for (uint32_t index = 0; index < m_alive.size(); ++index)
    {
      if (!m_alive[index]) {
        continue;
      }

      Entity const entity{index};
      if ((has<Ts>(entity) && ...)) {
        func(entity, get<Ts>(entity)...);
      }
    }
  }

  /** Call func(entity) for every living entity, in creation order */
  template<typename Func>
  void each_entity(Func&& func)
  {
    for (uint32_t index = 0; index < m_alive.size(); ++index) {
      if (m_alive[index]) {
        func(Entity{index});
      }
    }
  }

private:
  std::vector<bool> m_alive;
  std::vector<std::unique_ptr<PoolBase>> m_pools;
};

} // namespace pingus::ecs

#endif

/* EOF */
