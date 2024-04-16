#include "ParticleSystem.h"

#include "RandomHelper.h"

#include <glm/gtc/constants.hpp>
#include <pstl/glue_execution_defs.h>
#include <glm/gtx/compatibility.hpp>

ParticleSystem::ParticleSystem()
{
	m_particle_pool.resize(1000);
}

void ParticleSystem::onUpdate(Karen::Timestep ts)
{
	for (auto& particle : m_particle_pool)
	{
		if (!particle.active)
			continue;

		if (particle.life_remaining <= 0.0f)
		{
			particle.active = false;
			continue;
		}

		particle.life_remaining -= ts;
		particle.position += particle.velocity * ts.getTime();
		particle.rotation += 0.01f * ts;
	}
}

void ParticleSystem::onRender()
{
	for (auto& particle : m_particle_pool)
	{
		if (!particle.active)
			continue;

		float life = particle.life_remaining / particle.life_time;
    Karen::Vec4 color = glm::lerp(particle.end_color, particle.begin_color, life);

		float size = glm::lerp(particle.end_size, particle.begin_size, life);
		
	  Karen::Renderer2D::drawQuad(particle.position, Karen::Vec2(size), particle.rotation, Karen::Vec4(color));
  }
}

void ParticleSystem::emit(const ParticleProps& particleProps)
{
	Particle& particle = m_particle_pool.at(m_pool_index);
	particle.active = true;
	particle.position = particleProps.position;
	particle.rotation = Random::Float() * 2.0f * glm::pi<float>();

	particle.velocity = particleProps.velocity;
	particle.velocity.x += particleProps.velocity_variation.x * (Random::Float() - 0.5f);
	particle.velocity.y += particleProps.velocity_variation.y * (Random::Float() - 0.5f);

	particle.begin_color = particleProps.begin_color;
	particle.end_color = particleProps.end_color;

	particle.life_time = particleProps.life_time;
	particle.life_remaining = particleProps.life_time;
	particle.begin_size = particleProps.begin_size + particleProps.size_variation * (Random::Float() - 0.5f);
	particle.end_size = particleProps.end_size;

	m_pool_index = --m_pool_index % m_particle_pool.size();
}
