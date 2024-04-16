#include "Entity.h"


Entity::Entity(const std::string& name)
  :name(name)
{
}

void Entity::addAnimation(const std::string& n, const Karen::ARef<Animation>& a)
{
  if(animations.find(n) != animations.end())
    KAREN_WARN("Animation with name: {0} already exist in Entity it won't be added", n);
  else animations[n] = a;
}

void Entity::addCollisionShape(const std::string& n, const AABB::Rect& c)
{
  if(collision_shapes.find(n) != collision_shapes.end())
    KAREN_WARN("Collision Shapes with name: {0} already exist in Entity it won't be added", n);
  else collision_shapes[n] = c;
}

const Karen::ARef<Animation>& Entity::getAnimation(const std::string& n)
{
  if(animations.find(n) != animations.end())
   return animations.at(n);
  KAREN_WARN("Animation with name: {0} don't exist in Entity", n);
}

AABB::Rect& Entity::getCollisionShape(const std::string& n)
{
 if(collision_shapes.find(n) != collision_shapes.end())
   return collision_shapes.at(n);
  KAREN_WARN("Collision Shape with name: {0} don't exist in Entity", n);
}

const Karen::ARef<Animation>& Entity::getCurrentAnimation()
{
  return animations.at(current_animation);
}

AABB::Rect& Entity::getCurrentCollisionShape()
{
  return collision_shapes.at(current_collision_shape);
}

void Entity::setCurrentAnimation(const std::string& n)
{
  if(animations.find(n) != animations.end())
    current_animation = n;
  else KAREN_ERROR("No Animation with name: {0} to set as current Animation", n);
}

void Entity::setCurrentCollisionShape(const std::string& n)
{
  if(collision_shapes.find(n) != collision_shapes.end())
    current_collision_shape = n;
  else KAREN_ERROR("No CollisionShape with name: {0} to set as current CollisionShape", n);
}
