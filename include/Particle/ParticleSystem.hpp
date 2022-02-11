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

		ParticleSystem();
		~ParticleSystem();
		void update();
		void render();

	private:
		int particlesCount = 0;
		int particleHead = 0;
		int particleTail = 0;

		float countTime = 0.0f;

		Random* random;

		float quadVao[24];
		GLuint rectVAO, rectVBO;

		void findDeadParticle();
};
extern ParticleSystem *particleSystem;