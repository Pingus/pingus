#include <functional>

#include "engine/resource/resource.hpp"
#include "engine/resource/resource_manager.hpp"
#include "engine/display/sprite.hpp"

class SpriteLoader
{
private:
  std::string m_key;

public:
  SpriteLoader(const std::string& key) :
    m_key(key)
  {}

  Sprite operator()()
  {
    return Sprite(m_key);
  }
};

int main()
{
  typedef resource::ResourceManager<std::string, Sprite, SpriteLoader> SpriteResourceManager;
  SpriteResourceManager resource_mgr;

  resource::Resource<Sprite> sprite = resource_mgr.get("image/entrances/snow");

  std::function<Sprite()> func(SpriteLoader("Hello World"));

  return 0;
}

/* EOF */
