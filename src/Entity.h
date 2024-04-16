#ifndef ENTITY_H
#define ENTITY_H

#include "Animation.h"
#include "Karen/Core/Timestep.h"
#include "Karen/Karen.h"
#include "SweptAABB.h"


class Entity
{
public:
  Entity(const std::string& name);
  const Karen::ARef<Animation>& getAnimation(const std::string& name);
  AABB::Rect& getCollisionShape(const std::string& name);
  void addCollisionShape(const std::string& name, const AABB::Rect& shape);
  void addAnimation(const std::string& name, const Karen::ARef<Animation>& animation);
  void setCurrentAnimation(const std::string& name);
  void setCurrentCollisionShape(const std::string& name);
  const Karen::ARef<Animation>& getCurrentAnimation();
  AABB::Rect& getCurrentCollisionShape();
protected:
  std::string name, current_animation, current_collision_shape;
  std::unordered_map<std::string, Karen::ARef<Animation>> animations;
  std::unordered_map<std::string, AABB::Rect> collision_shapes;
};

#endif //ENTITY_H
