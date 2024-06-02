#pragma once

#include "Entity.h"

namespace hazel {

class ScriptableEntity {
public:
  template <typename T> T &getComponent() { return m_entity.getComponent<T>(); }

private:
  Entity m_entity;
  friend class Scene;
};

} // namespace hazel