#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <opengl.h>
#include <Particle/Particle.hpp>

#include <Logger.h>

class ParticleSystem {
	public:
		const int MAX_PARTICLES = 100;
		Particle particlesList[100];
		float g_particle_position_size_data[400];

		ParticleSystem();
		~ParticleSystem();
		void update();
		void render();

	private:
		int LastUsedParticle = 0;
		int particlesCount = 0;
		int particleHead = 0;
		int particleTail = 0;

		float quadVao[12];
		GLuint rectVAO, rectVBO;

		void newParticles(int newParticlesCount);
		void findDeadParticle();

		GLuint particles_position_buffer;
		GLuint particles_color_buffer;
};
extern ParticleSystem *particleSystem;