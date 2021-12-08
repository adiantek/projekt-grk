#include <ResourceLoader.hpp>
#include <Logger.h>
#include <SOIL/SOIL.h>

ResourceLoader::ResourceLoader() {
}

ResourceLoader::~ResourceLoader() {
}

void ResourceLoader::loadTextures() {
    loadTexture("assets/textures/grid.png", &this->txt_grid);
    loadTexture("assets/textures/grid_color.png", &this->txt_gridColor);
    loadTexture("assets/textures/earth.png", &this->txt_earth);
    loadTexture("assets/textures/earth2.png", &this->txt_earth2);
    loadTexture("assets/textures/moon.png", &this->txt_moon);
    loadTexture("assets/textures/spaceship.png", &this->txt_ship);
    loadTexture("assets/textures/earth2_normals.png", &this->txt_earthNormal);
    loadTexture("assets/textures/moon_normals.png", &this->txt_asteroidNormal);
    loadTexture("assets/textures/spaceship_normals.png", &this->txt_shipNormal);
    loadTexture("assets/textures/asteroid.png", &this->txt_asteroid);
}

bool ResourceLoader::loadResources() {
    if (this->all_loaded) {
        return true;
    }
    this->loadedResources = 0;
    this->totalResourcesCounter = 0;

    if (this->totalResources == 0) {
        // first frame - count all resources
        this->startFrame = 0;
    } else {
        this->startFrame = glfwGetTime();
    }

    this->loadTextures();

    this->totalResources = this->totalResourcesCounter;

    if (this->totalResources == this->loadedResources) {
        this->all_loaded = true;
        return true;
    }
    return false;
}

bool ResourceLoader::canLoadNextResource() {
    if (this->startFrame == 0) {
        // optimize - don't check time & don't load more resources
        return false;
    }
    if (this->totalResourcesCounter == this->loadedResources) {
        // load minimum one resource
        return true;
    }
    // max 100 ms per frame
    if (glfwGetTime() - this->startFrame < 0.1) {
        return true;
    } else {
        this->startFrame = 0;
        return false;
    }
}

void ResourceLoader::loadTexture(const char *name, GLuint *out) {
    this->totalResourcesCounter++;
    if (*out) {
        this->loadedResources++;
        return;
    }
    if (!canLoadNextResource()) {
        return;
    }
    LOGI("[ %3.0f%% ] Loading: %s", this->loadedResources * 100.0 / this->totalResources, name);
	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	int w, h;
	unsigned char *image = SOIL_load_image(name, &w, &h, 0, SOIL_LOAD_RGBA);
	if (!image) {
		LOGE("Failed loading %s: %s", name, SOIL_last_result());
	} else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
	}
    *out = id;
    this->loadedResources++;
}