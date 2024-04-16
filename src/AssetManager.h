#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "Animation.h"
#include "Karen/Render/API/Texture.h"
class AssetManager
{
public:
  AssetManager();
  AssetManager(const std::string& file_path);
  bool findTextures(const std::string& name);
  void loadFromFile(const std::string& file_name);
  void addTextures(const std::string& name, const std::vector<Karen::ARef<Karen::Texture2D>>& texs);
  void addAnimation(const std::string& name, const Karen::ARef<Animation>& animation);

  std::vector<Karen::ARef<Karen::Texture2D>>& getTextures(const std::string& name);
  Karen::ARef<Animation>& getAnimation(const std::string& name);
  std::unordered_map<std::string, Karen::ARef<Animation>>& getAnimations();
  std::unordered_map<std::string, std::vector<Karen::ARef<Karen::Texture2D>>>& getTextures();
private:
  std::unordered_map<std::string, std::vector<Karen::ARef<Karen::Texture2D>>> m_textures;
  std::unordered_map<std::string, Karen::ARef<Animation>> m_animations;

};
#endif //ASSET_MANAGER_H
