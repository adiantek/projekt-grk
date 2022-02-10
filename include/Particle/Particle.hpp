#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <opengl.h>

#include <Logger.h>

class Particle {
    inline static const float GRAVITY = -50.0f;

	public:
        float lifeRemaining = 0.0f;
        float scale = 0.20f;
        float gravityEffect = 1.0f;
        glm::vec3 position;
        glm::vec3 velocity;

		Particle(glm::vec3 position, glm::vec3 velocity, float gravityEffect, 
        float lifeRemaining, float rotation, float scale);
        ~Particle();
        Particle();

		glm::vec3 getPosition();
		float getScale();
		float getRotation();
        bool isAlive();

	private:
        float rotation = 0.0f;

        float sizeBegin, sizeEnd;
        float lifeLength = 5.0f;
};