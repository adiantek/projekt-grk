#include <Camera/Camera.hpp>
#include <ResourceLoader.hpp>
#include <Particle/ParticleSystem.hpp>
#include <Particle/Particle.hpp>
#include <Water/Water.hpp>
#include <Time/time.hpp>
#include <glm/ext.hpp>

ParticleSystem::ParticleSystem() {
	particleSystem = this;
    float quadVao[] ={
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
    };

	glGenVertexArrays(1, &this->rectVAO);
    glGenBuffers(1, &this->rectVBO);
    glBindVertexArray(this->rectVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->rectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVao), &quadVao, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

ParticleSystem::~ParticleSystem() {
    glDeleteVertexArrays(1, &this->rectVAO);
    glDeleteBuffers(1, &this->rectVBO);
}

void ParticleSystem::render() {
	glm::mat4 cameraMatrix = camera->getTransformationMatrix();
	
	if(particleTail == MAX_PARTICLES) {
		particleTail = 0;
	}
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!DODANIE POWINNO BYC ZALEZNE OD CZASU!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// if(particlesCount < MAX_PARTICLES) {
	// 	particlesList[particleTail] = Particle();
	// 	this->particlesCount += 1;
	// 	this->particleTail += 1;
	// }

	glm::vec4* particlePositionsAndLife = new glm::vec4[particlesCount];

	int count = 0;
	if(particleTail >= particleHead) {
		for(int i = particleHead; i < particleTail; i++) {
			//!!!!!!!!!!!!!!!!!!!!!modyfikacja na podstawie life remaining!!!!!!!!!!!!!!!!!!!!!!
			particlesList[i].position.y += timeExternal->deltaTime;
			particlesList[i].lifeRemaining += timeExternal->deltaTime;
			particlePositionsAndLife[count] = glm::vec4(glm::vec3(particlesList[i].position), particlesList[i].lifeRemaining);
			count += 1;
		}
	} else {
		for(int i = particleHead; i < MAX_PARTICLES; i++) {
			particlesList[i].position.y += timeExternal->deltaTime;
			particlesList[i].lifeRemaining += timeExternal->deltaTime;
			particlePositionsAndLife[count] = glm::vec4(glm::vec3(particlesList[i].position), particlesList[i].lifeRemaining);
			count += 1;
		}
		for(int i = 0; i < particleTail; i++) {
			particlesList[i].position.y += timeExternal->deltaTime;
			particlesList[i].lifeRemaining += timeExternal->deltaTime;
			particlePositionsAndLife[count] = glm::vec4(glm::vec3(particlesList[i].position), particlesList[i].lifeRemaining);
			count += 1;
		}
	}
	glUseProgram(resourceLoaderExternal->p_bubbles_shader);
    glUniform1f(resourceLoaderExternal->p_bubbles_shader_uni_distanceToSurface, waterObject->getY() - camera->position.y);

    glUniform4fv(resourceLoaderExternal->p_bubbles_shader_uni_particlePositionsAndLife, MAX_PARTICLES, glm::value_ptr(particlePositionsAndLife[0]));

    glUniformMatrix4fv(resourceLoaderExternal->p_bubbles_shader_uni_cameraMatrix, 1, GL_FALSE, glm::value_ptr(cameraMatrix));

    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, this->texture);
    // glUniform1i(resourceLoaderExternal->p_bubbles_shader_uni_texture, 0);
	glEnable(GL_DEPTH_TEST);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particlesCount);
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

void ParticleSystem::newParticles(int newParticlesCount) {
	// particlesCount = 0;
	// for(int i = 0; i < MAX_PARTICLES; i++){

	// 	Particle& p = particlesList[i]; // shortcut

	// 	if(p.isAlive()){

	// 		// Decrease life
	// 		p.lifeRemaining += timeExternal->deltaTime;
	// 		if (p.isAlive()){
	// 		particlesCount++;
	// 	}
	// }
}

void ParticleSystem::update() {
	this->findDeadParticle();
}

ParticleSystem *particleSystem = 0;