#pragma once
#include "Karen/Core/Timestep.h"
#include <Karen/Karen.h>


class AABB
{
public:
 struct Rect
  {
    Karen::Vec2 pos;
    Karen::Vec2 size;
    Karen::Vec2 vel;
	  Rect* contact[4];
  };
public:
  static bool intersects(const Karen::Vec2& point, const Rect& rect);
  static bool intersects(const Rect& rect1, const Rect& rect2);
  static bool intersects(const Karen::Vec2& ray_origin, const Karen::Vec2& ray_dir, const Rect& target, Karen::Vec2& contact_point, Karen::Vec2& contact_normal, float& t_hit_near);
  static bool dynamicIntersect(const Rect& rect_dynamic, Karen::Timestep ts, const Rect& r_static,
			Karen::Vec2& contact_point, Karen::Vec2& contact_normal, float& contact_time);
  static bool dynamicResolve(Rect* r_dynamic, Karen::Timestep ts, Rect* r_static);
};
