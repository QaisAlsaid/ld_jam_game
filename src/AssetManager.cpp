#include "AssetManager.h"
#include "Karen/Core/Log.h"
#include "Karen/Render/API/Texture.h"
#include <pugixml.hpp>
#include <string>

static std::string getNameFromPath(const std::string& path);

AssetManager::AssetManager() 
{

}

AssetManager::AssetManager(const std::string& fp) 
{
  loadFromFile(fp);
}


void AssetManager::addAnimation(const std::string& name, const Karen::ARef<Animation>& animation)
{
  std::string new_name = name;
  if(m_animations.find(name) == m_animations.end())
    m_animations[name] = animation;
  else
  {
    KAREN_CORE_WARN("Animation with name: {0} already exist it will be added as: {1}", new_name, new_name + new_name);
    addAnimation(new_name, animation);
  }
}

std::vector<Karen::ARef<Karen::Texture2D>>& AssetManager::getTextures(const std::string& name)
{
  if(m_textures.find(name) != m_textures.end())
    return m_textures.at(name);
  else
    KAREN_CORE_ASSERT("Texture With name: {0} don't exist", name);
}

Karen::ARef<Animation>& AssetManager::getAnimation(const std::string& name)
{
  if(m_animations.find(name) != m_animations.end())
    return m_animations.at(name);
  else
    KAREN_CORE_ASSERT("Animation With name: {0} don't exist", name);
}

std::unordered_map<std::string, Karen::ARef<Animation>>& AssetManager::getAnimations()
{
  return m_animations;
}

std::unordered_map<std::string, std::vector<Karen::ARef<Karen::Texture2D>>>& AssetManager::getTextures()
{
  return m_textures;
}


void AssetManager::loadFromFile(const std::string& file_path)
{
  KAREN_TRACE("Started Loading Assets from config");
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(file_path.c_str(), pugi::parse_default|pugi::parse_declaration);
  if(!result)
  {
    KAREN_ERROR("Parse Error at: {0}, offset: {1}",result.description(), result.offset);
    return;
  }
  for(pugi::xml_node list : doc.children("AssetsList"))
  {
    std::vector<Karen::ARef<Karen::Texture2D>> tuxes;
    for(auto textures : list.children("Textures"))
    {
      if(textures)
      {
        std::string name = textures.child_value("Name");
        if(name == "") KAREN_WARN("No Name in Texture node it will default to the last striped path");
        for(auto texture : textures.children("Texture"))
        {
          if(!texture)
            KAREN_WARN("No Textures in Texture node it can't be loaded");
          else
          {
            bool is_flip = true;
            auto path = texture.child_value("Path");
            auto flip = texture.child_value("Flip");
            if(std::string(flip) == "") KAREN_WARN("No flip given it will default to true");
            else if(std::string(flip) == "True")
              is_flip = true;
            else if(std::string(flip) == "False")
              is_flip = false;
            else KAREN_WARN("Unknown value given to Flip '{0}' it will default to true");
            if(std::string(path) == "")
              KAREN_WARN("No Path in Texture Node it can't be loaded");
            else
            {
              if(name == "")
                name = getNameFromPath(path);
              tuxes.push_back(Karen::Texture2D::create(path, is_flip));
            }
          }
        }
        KAREN_TRACE("Texture added: name: {0}", name);
        m_textures[name] = tuxes;
      }
      else
        KAREN_TRACE("Textures Not found at this iteration");
    }
    for(auto animation : list.children("Animation"))
    {
      if(animation)
      {
        std::string tux_pack = animation.child_value("Textures");
        std::string name = animation.child_value("Name");
        std::string speed = animation.child_value("Speed");
        std::string repeated = animation.child_value("repeated");
        if(tux_pack == "") KAREN_WARN("No Textures is given to the node Animation it won't be loaded");
        if(name == "")
        {
          KAREN_WARN("No Name is given to the node animation it will default to a_<Textuers.name>_a");
          name = "a_"+ tux_pack + "_a";
        }
        float speed_float = 0.0f;
        bool repeated_bool =  true;
          if(speed == "")
          {
            KAREN_WARN("No speed given to animation: {0} it will default to 1/count", name);
            speed_float = 1.0f/(float)getTextures(tux_pack).size(); 
          }
          else
          speed_float = std::stof(speed);
         if(repeated == "") KAREN_WARN("Repeated attribute is not given to animation: {0} it will default to True");
         else if(repeated == "False") repeated_bool = false;
         {
          KAREN_INFO("Animation added: name: {0} textures: {1}", name, tux_pack);
         }
        Animation* anim = new Animation(name, getTextures(tux_pack));
        Karen::ARef<Animation> a;
        a.reset(anim);
        a->setSpeed(speed_float);
        a->setRepeated(repeated_bool);
        m_animations[name] = a;
      }
      else 
        KAREN_TRACE("Animation Not Found at this iteration");
    }
  }
  KAREN_TRACE("exited loadFromFile AssetManager");
}


static std::string getNameFromPath(const std::string& path)
{
  auto last_dir_sep = path.find_last_of("\\/");
  last_dir_sep = last_dir_sep == std::string::npos ? 0 : last_dir_sep + 1;
  auto last_dot = path.find_last_of(".");
  auto count = last_dot == std::string::npos ? path.size() - last_dir_sep : last_dot - last_dir_sep;
  auto new_name = path.substr(last_dir_sep, count);
  return new_name;
}
