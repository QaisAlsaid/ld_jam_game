#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <Karen/Karen.h>

struct ParticleProps
{
  Karen::Vec2 position = {0.0f, 0.0f};
  Karen::Vec2 velocity = {0.0f, 0.0f}, velocity_variation = {0.0f, 0.0f};
  Karen::Vec4 begin_color = Karen::Vec4(1.0f), end_color = Karen::Vec4(0.0f);
	float begin_size = 1.0f, end_size = 0.0f, size_variation = 0.0f;
	float life_time = 1.0f;
};

class ParticleSystem
{
public:
	ParticleSystem();

	void onUpdate(Karen::Timestep ts);
	void onRender();

	void emit(const ParticleProps& ParticleProps);
private:
	struct Particle
	{
    Karen::Vec2 position;
    Karen::Vec2 velocity;
    Karen::Vec4 begin_color, end_color;
		float rotation = 0.0f;
		float begin_size, end_size;

		float life_time = 1.0f;
		float life_remaining = 0.0f;

		bool active = false;
	};
	std::vector<Particle> m_particle_pool;
	uint32_t m_pool_index = 999;
	uint32_t m_quad_varr = 0;
};

#endif //PARTICLE_SYSTEM_H
