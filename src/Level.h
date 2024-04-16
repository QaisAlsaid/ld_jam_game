#ifndef LEVEL_H
#define LEVEL_H

#include "Entity.h"
#include "AssetManager.h"
#include <vector>

class Level 
{
public:
  Level();
  Level(const std::string& file_path, AssetManager* assets);
  void reload();
  void change(const std::string& file_path);
  void setAssets(AssetManager* asset);
  inline const Karen::ARef<Entity>& getEntity(const std::string& name)
  {
    KAREN_CORE_ASSERT(m_entites.find(name) != m_entites.end(), "Entity: {0} not found", name);
    return m_entites.at(name);
  }
  inline const std::unordered_map<std::string, Karen::ARef<Entity>>& getEntites()
  {
    return m_entites;
  }
private:
  std::string m_current_level;
  std::unordered_map<std::string, Karen::ARef<Entity>> m_entites;
  AssetManager* m_assets;
};


#endif //LEVEL_H
