#include "SweptAABB.h"
using namespace Karen;


bool AABB::intersects(const Vec2& p, const AABB::Rect& r)
{
	return (p.x >= r.pos.x - r.size.x/2.0f && p.y >= r.pos.y - r.size.x/2.0f && p.x < r.pos.x + r.size.x/2.0f&& p.y >= r.pos.y + r.size.y/2.0f);
}

bool AABB::intersects(const AABB::Rect& r1, const AABB::Rect& r2)
{
	return (r1.pos.x - r1.size.x/2.0f < r2.pos.x + r2.size.x/2.0f && r1.pos.x + r1.size.x/2.0f > r2.pos.x - r2.size.x/2.0f && r1.pos.y - r2.size.y/2.0f< r2.pos.y + r2.size.y/2.0f && r1.pos.y + r1.size.y/2.0f > r2.pos.y - r2.size.y/2.0f);
}

bool AABB::intersects(const Vec2& ray_origin, const Vec2& ray_dir, const AABB::Rect& target, Vec2& contact_point, Vec2& contact_normal, float& t_hit_near)
{
	contact_normal = { 0,0 };
	contact_point = { 0,0 };

	const Vec2& invdir = 1.0f / ray_dir;

	Vec2 t_near = (target.pos - target.size/2.0f - ray_origin) * invdir;
	Vec2 t_far = (target.pos + target.size/2.0f - ray_origin) * invdir;

	if(std::isnan(t_far.y) || std::isnan(t_far.x)) return false;
	if(std::isnan(t_near.y) || std::isnan(t_near.x)) return false;

	if(t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
	if(t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

	if(t_near.x > t_far.y || t_near.y > t_far.x) return false;

	t_hit_near = std::max(t_near.x, t_near.y);

	float t_hit_far = std::min(t_far.x, t_far.y);

	if(t_hit_far < 0)
		return false;

	contact_point = ray_origin + t_hit_near * ray_dir;

	if(t_near.x > t_near.y)
		if (invdir.x < 0)
			contact_normal = { 1, 0 };
		else
			contact_normal = { -1, 0 };
	else if (t_near.x < t_near.y)
  {
		if (invdir.y < 0)
			contact_normal = { 0, 1 };
		else
			contact_normal = { 0, -1 };
  } 
  return true;
}

bool AABB::dynamicIntersect(const AABB::Rect& r_dynamic, Karen::Timestep ts, const AABB::Rect& r_static,
			Vec2& contact_point, Vec2& contact_normal, float& contact_time)
{
  if(r_dynamic.vel.x == 0 && r_dynamic.vel.y == 0)
		return false;
	AABB::Rect expanded_target;
	expanded_target.pos = r_static.pos;
	expanded_target.size = r_static.size + r_dynamic.size;

	if(intersects(r_dynamic.pos, r_dynamic.vel * ts.getTime(), expanded_target, contact_point, contact_normal, contact_time))
		return (contact_time >= 0.0f && contact_time < 1.0f);
	else
		return false;
}


bool AABB::dynamicResolve(AABB::Rect* r_dynamic, Karen::Timestep ts, AABB::Rect* r_static)
{
	Vec2 contact_point, contact_normal;
	float contact_time = 0.0f;
	if(AABB::dynamicIntersect(*r_dynamic, ts, *r_static, contact_point, contact_normal, contact_time))
	{
		contact_normal.y > 0 ? r_dynamic->contact[0] = r_static : nullptr;
	  contact_normal.x < 0 ? r_dynamic->contact[1] = r_static : nullptr;
		contact_normal.y < 0 ? r_dynamic->contact[2] = r_static : nullptr;
		contact_normal.x > 0 ? r_dynamic->contact[3] = r_static : nullptr;
//TODO dot product
		r_dynamic->vel += contact_normal * Vec2(std::abs(r_dynamic->vel.x), std::abs(r_dynamic->vel.y)) * (1 - contact_time);
    //r_dynamic->vel += contact_normal * glm::dot(contact_normal, r_dynamic->vel) * (1 - contact_time);

		return true;
	}

	return false;
}

