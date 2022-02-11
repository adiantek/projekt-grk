#include <Camera/Camera.hpp>
#include <ResourceLoader.hpp>
#include <Particle/ParticleSystem.hpp>
#include <Particle/Particle.hpp>
#include <Water/Water.hpp>
#include <Robot/Robot.hpp>
#include <Time/Time.hpp>
#include <glm/ext.hpp>

ParticleSystem::ParticleSystem() {
	particleSystem = this;
	this->random = new Random(12);

    float quadVao[] ={
        1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,

        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,
    };

	glGenVertexArrays(1, &this->rectVAO);
    glGenBuffers(1, &this->rectVBO);
    glBindVertexArray(this->rectVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->rectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVao), &quadVao, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

ParticleSystem::~ParticleSystem() {
    glDeleteVertexArrays(1, &this->rectVAO);
    glDeleteBuffers(1, &this->rectVBO);
	delete this->random;
}

void ParticleSystem::render() {
	if(particleTail == MAX_PARTICLES) {
		particleTail = 0;
	}
	glm::mat4 robotModel = robot->getGameObjectMatrix();
	glm::vec3 robotForward = glm::normalize(robot->position - glm::vec3(robotModel * glm::vec4(robot->forward, 1.0f)));

	if(robot->mode == 3 || robot->mode == 1) {
		this->countTime += timeExternal->deltaTime;
		if(countTime >= 0.15f) {
			if(particlesCount < MAX_PARTICLES - 4) {
				glm::vec3* newPositions = robot->getPropellersPositions();
				for(int i = 0; i < 2; i++) {
					particlesList[particleTail] = Particle();
					particlesList[particleTail].position = newPositions[i];
					particlesList[particleTail].position.x += (robotForward.x/12);
					particlesList[particleTail].position.y += (robotForward.y/12);
					particlesList[particleTail].position.z += (robotForward.z/12);
					this->particlesCount += 1;
					this->particleTail += 1;
				}
				for(int i = 2; i < 4; i++) {
					particlesList[particleTail] = Particle();
					particlesList[particleTail].position = newPositions[i];
					particlesList[particleTail].position.x += (robotForward.x/8);
					particlesList[particleTail].position.y += (robotForward.y/8);
					particlesList[particleTail].position.z += (robotForward.z/8);
					this->particlesCount += 1;
					this->particleTail += 1;
				}
			}
			countTime = 0.0f;
		}
	}
	float begin = -1.0;
	float end = 1.0;
	glm::vec4 particlePositionsAndLife[MAX_PARTICLES];
	//  + glm::vec3(-robotForward);
	int count = 0;
	if(particleTail >= particleHead) {
		for(int i = particleHead; i < particleTail; i++) {
			particlesList[i].position.y += timeExternal->deltaTime*random->nextFloat(0.0, 1.0);
			particlesList[i].position.x += timeExternal->deltaTime*random->nextFloat(begin, end);
			particlesList[i].position.z += timeExternal->deltaTime*random->nextFloat(begin, end);
			if(particlesList[i].lifeRemaining < 2.5 && robot->mode == 1) {
				particlesList[i].position.x += timeExternal->deltaTime*(robotForward.x + random->nextFloat(begin, end));
				particlesList[i].position.y += timeExternal->deltaTime*(robotForward.y + random->nextFloat(0.0, 2.0));
				particlesList[i].position.z += timeExternal->deltaTime*(robotForward.z + random->nextFloat(begin, end));
			}
			particlesList[i].lifeRemaining += timeExternal->deltaTime;
			particlePositionsAndLife[count] = glm::vec4(glm::vec3(particlesList[i].position), particlesList[i].lifeRemaining);
			count += 1;
		}
	} else {
		for(int i = particleHead; i < MAX_PARTICLES; i++) {
			particlesList[i].position.y += timeExternal->deltaTime*random->nextFloat(0.0, 1.0);
			particlesList[i].position.x += timeExternal->deltaTime*random->nextFloat(begin, end);
			particlesList[i].position.z += timeExternal->deltaTime*random->nextFloat(begin, end);
			if(particlesList[i].lifeRemaining < 2.5 && robot->mode == 1) {
				particlesList[i].position.x += timeExternal->deltaTime*(robotForward.x + random->nextFloat(begin, end));
				particlesList[i].position.y += timeExternal->deltaTime*(robotForward.y + random->nextFloat(0.0, 2.0));
				particlesList[i].position.z += timeExternal->deltaTime*(robotForward.z + random->nextFloat(begin, end));
			}
			particlesList[i].lifeRemaining += timeExternal->deltaTime;
			particlePositionsAndLife[count] = glm::vec4(glm::vec3(particlesList[i].position), particlesList[i].lifeRemaining);
			count += 1;
		}
		for(int i = 0; i < particleTail; i++) {
			particlesList[i].position.y += timeExternal->deltaTime*random->nextFloat(0.0, 1.0);
			particlesList[i].position.x += timeExternal->deltaTime*random->nextFloat(begin, end);
			particlesList[i].position.z += timeExternal->deltaTime*random->nextFloat(begin, end);
			if(particlesList[i].lifeRemaining < 2.5 && robot->mode == 1) {
				particlesList[i].position.x += timeExternal->deltaTime*(robotForward.x + random->nextFloat(begin, end));
				particlesList[i].position.y += timeExternal->deltaTime*(robotForward.y + random->nextFloat(0.0, 2.0));
				particlesList[i].position.z += timeExternal->deltaTime*(robotForward.z + random->nextFloat(begin, end));
			}
			particlesList[i].lifeRemaining += timeExternal->deltaTime;
			particlePositionsAndLife[count] = glm::vec4(glm::vec3(particlesList[i].position), particlesList[i].lifeRemaining);
			count += 1;
		}
	}
	glm::mat4 cameraMatrix = camera->getTransformationMatrix();
	glm::mat4 cameraRotation = camera->getCameraRotation();

	glUseProgram(resourceLoaderExternal->p_bubbles_shader);
    glUniform1f(resourceLoaderExternal->p_bubbles_shader_uni_distanceToSurface, waterObject->getY() - camera->position.y);

	glUniform3f(resourceLoaderExternal->p_bubbles_shader_uni_cameraDirection, cameraRotation[0][0], cameraRotation[1][0], cameraRotation[2][0]);
	glUniform3f(resourceLoaderExternal->p_bubbles_shader_uni_cameraUp, cameraRotation[0][1], cameraRotation[1][1], cameraRotation[2][1]);

    glUniform4fv(resourceLoaderExternal->p_bubbles_shader_uni_particlePositionsAndLife, MAX_PARTICLES, glm::value_ptr(particlePositionsAndLife[0]));
    glUniformMatrix4fv(resourceLoaderExternal->p_bubbles_shader_uni_cameraMatrix, 1, GL_FALSE, glm::value_ptr(cameraMatrix));

    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, this->texture);
    // glUniform1i(resourceLoaderExternal->p_bubbles_shader_uni_texture, 0);
	glEnable(GL_DEPTH_TEST);
	glBindVertexArray(this->rectVAO);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, particlesCount);
}

void ParticleSystem::findDeadParticle(){
	int count = 0;
	if(particleTail >= particleHead) {
		for(int i = particleHead; i < particleTail; i++) {
			if (particlesList[i].isAlive()){
				break;
        	} else {
				this->particlesCount -= 1;
				this->particleHead += 1;
			}
		}
	} else {
		for(int i = particleHead; i < MAX_PARTICLES; i++) {
			if (particlesList[i].isAlive()){
				break;
        	} else {
				this->particlesCount -= 1;
				this->particleHead += 1;
			}
		}
		if(particleHead >= MAX_PARTICLES) {
			particleHead = 0;
		}
		for(int i = 0; i < particleTail; i++) {
			if (particlesList[i].isAlive()){
				break;
        	} else {
				this->particlesCount -= 1;
				this->particleHead += 1;
			}
		}
	}
}

void ParticleSystem::update() {
	this->findDeadParticle();
}

ParticleSystem *particleSystem = 0;