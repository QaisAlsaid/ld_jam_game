#include "Level.h"
#include <cstdlib>
#include <memory>
#include <pugixml.hpp>
#include <Karen/Karen.h>
#include "AssetManager.h"
#include "SweptAABB.h"



Level::Level()
{

}

Level::Level(const std::string& level_config_path, AssetManager* a)
{
  change(level_config_path);
  m_assets = a;
}

void Level::setAssets(AssetManager* a)
{
  m_assets = a;
}

void Level::reload()
{
  change(m_current_level);
}

void Level::change(const std::string& fp)
{
  KAREN_TRACE("Started Loading Level from config");
  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(fp.c_str(), pugi::parse_default|pugi::parse_declaration);
  if(!result)
  {
    KAREN_ERROR("Parse Error at: {0}, offset: {1}",result.description(), result.offset);
    return;
  }
  for(auto list : doc.children("EntitiesList"))
  {
    if(!list) KAREN_WARN("no EntitiesList found no level will load");
    else
    for(auto entity : list.children("Entity"))
    {
      if(!entity) {KAREN_WARN("No Entity");break;}
      const std::string& name = entity.child_value("Name");
      if(name == "") KAREN_WARN("No name given to entity it won't be added");
      else
      {
        Entity* e = new Entity(name);
        auto anims = entity.child("Animations");
        auto cols = entity.child("CollisionShapes");
        if(!anims) KAREN_WARN("Entity: {0} has no Animations", name);
        else
        {
          for(auto anim : anims)
          {
            const std::string& n = anim.child_value("Name");
            if(n == "") KAREN_WARN("Animation has no name it won't be added to Entity: {0}", name);
            else
            {
              const std::string& anmnm = anim.child_value("AnimationHandle");
              if(anmnm == "") KAREN_WARN("AnimationHandle is not given to Entity: {0}, Animation: {1}", name, n);
              else
              {
                KAREN_TRACE("Attempting to add Animation: {0}, Handle: {1}", n, anmnm);
                e->addAnimation(n, m_assets->getAnimation(anmnm));
                KAREN_TRACE("Added Animaton: {0} with Name: {1} To Entity: {2}", anmnm, n, name);
              }
            }
          }
        }
        {
          for(auto col : cols)
          {
            if(!col) {KAREN_WARN("No Shape"); break;}
            const std::string& n = col.child_value("Name");
            if(n == "") KAREN_WARN("No name given to Collision Shape in Entity: {0}", name);
            else
            {
              Karen::Vec2 pos_float;
              auto pos = col.child("Position");
              if(!pos) KAREN_WARN("No Position is given to Collision Shape: {0} in Entity: {1} it won't be added", n, name);
              else 
              {
                const std::string& x = pos.child_value("X");
                const std::string& y = pos.child_value("Y");
                if(x == "" || y == "") KAREN_WARN("Position with X: {0}, Y: {1} isn't valid it won't be added", x, y);
                else
                {
                  pos_float = Karen::Vec2(std::stof(x), std::stof(y));
                }
              }
              const std::string& type = col.child_value("Type");
              if(type == "") KAREN_WARN("No Type is given to Collision Shape {0} in Entity: {1} it won't be added", n, name);
              else if(type == "Rectangle")
              {
                bool change = false;
                Karen::Vec2 vel_float(0.0f);
                auto vel = col.child("Velocity");
                auto s = col.child("Size");
                if(!s) KAREN_WARN("No Size is given to Collision Shape: {0} Type: {1} in Entity: {2}", n, type, name);
                else 
                {
                  const std::string& width = s.child_value("Width");
                  const std::string& height = s.child_value("Height");
                  if(width == "" || height == "") 
                  {
                    KAREN_WARN("Size with Width: {0}, Height: {1} isn't valid it won't be added", width, height);
                    change = true;
                  }
                  else 
                  {
                    if(change)
                      vel_float = Karen::Vec2(std::stof(vel.child_value("X")), std::stof(vel.child_value("Y")));
                    if(!vel) KAREN_WARN("No Velocity is given to rectangle it will default to Vec2(0.0f)");
                    else
                    {
                      Karen::Vec2 size(std::stof(width), std::stof(height));
                      KAREN_TRACE("Adding Collision Shape: {0} Type: {1} Position {2} Size: {3} to Entity: {4}",n, type, pos_float, size, name);
                      AABB::Rect colsh = {pos_float, size, vel_float};
                      e->addCollisionShape(n, colsh);
                    }
                  }
                }
              }
              else if(type == "Circle")
              {
                KAREN_ERROR("Not supported"); break;
                /*
                const std::string& r = col.child_value("Radius");
                if(r == "") KAREN_WARN("No Radius is given to Collision Shape: {0} Type: {1} in Entity: {2}", n, type, name);
                else
                {
                  float r_float = std::stof(r);
                  KAREN_TRACE("Adding Collision Shape: {0} Type: {1} Position {2} Radius: {3} to Entity: {4}",n, type, pos_float, r_float, name);
                  e->addCollisionShape(n, Karen::RigidBody2D::createCircle(Karen::ShapeProps(), r_float, pos_float));
                }
              }*/
              }
              else KAREN_WARN("Unknown Type: '{0}' given to Collision Shape: {1} in Entity: {2} it won't be added", type, n, name);

            }
          }
        }
        Karen::ARef<Entity> e_ptr;
        e_ptr.reset(e);
        m_entites[name] = e_ptr;
        KAREN_WARN("Added Entity Name: {0}", name);
      }
    }
  }
  KAREN_TRACE("Loading Level exited");
}
