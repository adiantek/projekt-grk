#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <opengl.h>
#include <Particle/Particle.hpp>
#include <Random.hpp>

#include <Logger.h>

class ParticleSystem {
	public:
		static const int MAX_PARTICLES = 200;
		Particle particlesList[200];
		float g_particle_position_size_data[800];

		ParticleSystem();
		~ParticleSystem();
		void update();
		void render();

	private:
		int LastUsedParticle = 0;
		int particlesCount = 0;
		int particleHead = 0;
		int particleTail = 0;

		float countTime = 0.0f;

		Random* random;

		float quadVao[24];
		GLuint rectVAO, rectVBO;

		void newParticles(int newParticlesCount);
		void findDeadParticle();

		GLuint particles_position_buffer;
		GLuint particles_color_buffer;
};
extern ParticleSystem *particleSystem;