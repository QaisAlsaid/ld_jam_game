#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <Karen/Karen.h>

struct ParticleProps
{
  Karen::Vec2 Position;
  Karen::Vec2 Velocity, VelocityVariation;
  Karen::Vec4 ColorBegin, ColorEnd;
	float SizeBegin, SizeEnd, SizeVariation;
	float LifeTime = 1.0f;
};

class ParticleSystem
{
public:
	ParticleSystem();

	void onUpdate(Karen::Timestep ts);
	void onRender(Karen::OrthographicCamera& camera);

	void emit(const ParticleProps& ParticleProps);
private:
	struct Particle
	{
    Karen::Vec2 Position;
    Karen::Vec2 Velocity;
    Karen::Vec4 ColorBegin, ColorEnd;
		float Rotation = 0.0f;
		float SizeBegin, SizeEnd;

		float LifeTime = 1.0f;
		float LifeRemaining = 0.0f;

		bool Active = false;
	};
	std::vector<Particle> m_particle_pool;
	uint32_t m_pool_index = 999;

	uint32_t m_quad_varr = 0;
	std::unique_ptr<KAren::ARef<Karen::Shader>> m_particle_shader;
};

#endif //PARTICLE_SYSTEM_H
