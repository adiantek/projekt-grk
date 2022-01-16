#include <Logger.h>
#include <SOIL/SOIL.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <filesystem>
#include <unordered_set>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <ResourceLoader.hpp>
#include <Resources/Model.hpp>


ResourceLoader::ResourceLoader() {
    resourceLoaderExternal = this;
}

ResourceLoader::~ResourceLoader() {
}

void ResourceLoader::loadTextures() {
    loadTexture("assets/textures/grid.png", &this->tex_grid);
    loadTexture("assets/textures/grid_color.png", &this->tex_gridColor);
    loadTexture("assets/textures/earth.png", &this->tex_earth);
    loadTexture("assets/textures/earth2.png", &this->tex_earth2);
    loadTexture("assets/textures/moon.png", &this->tex_moon);
    loadTexture("assets/textures/spaceship.png", &this->tex_ship);
    loadTexture("assets/textures/earth2_normals.png", &this->tex_earthNormal);
    loadTexture("assets/textures/moon_normals.png", &this->tex_asteroidNormal);
    loadTexture("assets/textures/spaceship_normals.png", &this->tex_shipNormal);
    loadTexture("assets/textures/asteroid.png", &this->tex_asteroid);
    loadTexture("assets/textures/wall.png", &this->tex_wall);
    loadTexture("assets/textures/wall_normal.png", &this->tex_wallNormal);
    loadTexture("assets/textures/wall_height.png", &this->tex_wallHeight);
    loadTextureCubeMap(&this->tex_skybox);
}

void ResourceLoader::loadPrograms() {
#define LOAD_PROGRAM(name, count, ...) if (loadProgram(#name, &this->p_##name, &this->p_##name##_loaded, count, __VA_ARGS__))

    LOAD_PROGRAM(simple_color_shader, 2, "simple_color_shader.frag", "simple_color_shader.vert") {
        // this->dumpProgram("simple_color_shader", this->p_simple_color_shader);
        this->p_simple_color_shader_attr_vertexColor = glGetAttribLocation(this->p_simple_color_shader, "vertexColor");
        this->p_simple_color_shader_attr_vertexPosition = glGetAttribLocation(this->p_simple_color_shader, "vertexPosition");
        this->p_simple_color_shader_uni_transformation = glGetUniformLocation(this->p_simple_color_shader, "transformation");
    }

    LOAD_PROGRAM(skybox_shader, 2, "cubemap/cubemap.frag", "cubemap/cubemap.vert") {
        // this->dumpProgram("skybox_shader", this->p_skybox_shader);
        this->p_skybox_shader_attr_aPos = glGetAttribLocation(this->p_skybox_shader, "aPos");
        this->p_skybox_shader_uni_projection = glGetUniformLocation(this->p_skybox_shader, "projection");
        this->p_skybox_shader_uni_skybox = glGetUniformLocation(this->p_skybox_shader, "skybox");
    }

    LOAD_PROGRAM(shader_4_1, 2, "shader_4_1.frag", "shader_4_1.vert") {
        // this->dumpProgram("shader_4_1", this->p_shader_4_1);
        this->p_shader_4_1_attr_vertexNormal = glGetAttribLocation(this->p_shader_4_1, "vertexNormal");
        this->p_shader_4_1_attr_vertexPosition = glGetAttribLocation(this->p_shader_4_1, "vertexPosition");
        this->p_shader_4_1_uni_cameraPos = glGetUniformLocation(this->p_shader_4_1, "cameraPos");
        this->p_shader_4_1_uni_lightPos = glGetUniformLocation(this->p_shader_4_1, "lightPos");
        this->p_shader_4_1_uni_modelMatrix = glGetUniformLocation(this->p_shader_4_1, "modelMatrix");
        this->p_shader_4_1_uni_objectColor = glGetUniformLocation(this->p_shader_4_1, "objectColor");
        this->p_shader_4_1_uni_transformation = glGetUniformLocation(this->p_shader_4_1, "transformation");
    }
    LOAD_PROGRAM(shader_4_sun, 2, "shader_4_sun.frag", "shader_4_sun.vert") {
        // this->dumpProgram("shader_4_sun", this->p_shader_4_sun);
        this->p_shader_4_sun_attr_vertexNormal = glGetAttribLocation(this->p_shader_4_sun, "vertexNormal");
        this->p_shader_4_sun_attr_vertexPosition = glGetAttribLocation(this->p_shader_4_sun, "vertexPosition");
        this->p_shader_4_sun_uni_cameraPos = glGetUniformLocation(this->p_shader_4_sun, "cameraPos");
        this->p_shader_4_sun_uni_modelMatrix = glGetUniformLocation(this->p_shader_4_sun, "modelMatrix");
        this->p_shader_4_sun_uni_modelViewProjectionMatrix = glGetUniformLocation(this->p_shader_4_sun, "modelViewProjectionMatrix");
        this->p_shader_4_sun_uni_objectColor = glGetUniformLocation(this->p_shader_4_sun, "objectColor");
    }
    LOAD_PROGRAM(shader_4_tex, 2, "shader_4_tex.frag", "shader_4_tex.vert") {
        // this->dumpProgram("shader_4_tex", this->p_shader_4_tex);
        this->p_shader_4_tex_attr_vertexBitangent = glGetAttribLocation(this->p_shader_4_tex, "vertexBitangent");
        this->p_shader_4_tex_attr_vertexNormal = glGetAttribLocation(this->p_shader_4_tex, "vertexNormal");
        this->p_shader_4_tex_attr_vertexPosition = glGetAttribLocation(this->p_shader_4_tex, "vertexPosition");
        this->p_shader_4_tex_attr_vertexTangent = glGetAttribLocation(this->p_shader_4_tex, "vertexTangent");
        this->p_shader_4_tex_attr_vertexTexCoord = glGetAttribLocation(this->p_shader_4_tex, "vertexTexCoord");
        this->p_shader_4_tex_uni_cameraPos = glGetUniformLocation(this->p_shader_4_tex, "cameraPos");
        this->p_shader_4_tex_uni_colorTexture = glGetUniformLocation(this->p_shader_4_tex, "colorTexture");
        this->p_shader_4_tex_uni_lightPos = glGetUniformLocation(this->p_shader_4_tex, "lightPos");
        this->p_shader_4_tex_uni_modelMatrix = glGetUniformLocation(this->p_shader_4_tex, "modelMatrix");
        this->p_shader_4_tex_uni_normalSampler = glGetUniformLocation(this->p_shader_4_tex, "normalSampler");
        this->p_shader_4_tex_uni_transformation = glGetUniformLocation(this->p_shader_4_tex, "transformation");
    }
    LOAD_PROGRAM(shader_4_tex_with_parallax, 2, "shader_4_tex_with_parallax.frag", "shader_4_tex.vert") {
        // this->dumpProgram("shader_4_tex_with_parallax", this->p_shader_4_tex_with_parallax);
        this->p_shader_4_tex_with_parallax_attr_vertexBitangent = glGetAttribLocation(this->p_shader_4_tex_with_parallax, "vertexBitangent");
        this->p_shader_4_tex_with_parallax_attr_vertexNormal = glGetAttribLocation(this->p_shader_4_tex_with_parallax, "vertexNormal");
        this->p_shader_4_tex_with_parallax_attr_vertexPosition = glGetAttribLocation(this->p_shader_4_tex_with_parallax, "vertexPosition");
        this->p_shader_4_tex_with_parallax_attr_vertexTangent = glGetAttribLocation(this->p_shader_4_tex_with_parallax, "vertexTangent");
        this->p_shader_4_tex_with_parallax_attr_vertexTexCoord = glGetAttribLocation(this->p_shader_4_tex_with_parallax, "vertexTexCoord");
        this->p_shader_4_tex_with_parallax_uni_cameraPos = glGetUniformLocation(this->p_shader_4_tex_with_parallax, "cameraPos");
        this->p_shader_4_tex_with_parallax_uni_colorTexture = glGetUniformLocation(this->p_shader_4_tex_with_parallax, "colorTexture");
        this->p_shader_4_tex_with_parallax_uni_depthSampler = glGetUniformLocation(this->p_shader_4_tex_with_parallax, "depthSampler");
        this->p_shader_4_tex_with_parallax_uni_heightScale = glGetUniformLocation(this->p_shader_4_tex_with_parallax, "heightScale");
        this->p_shader_4_tex_with_parallax_uni_lightPos = glGetUniformLocation(this->p_shader_4_tex_with_parallax, "lightPos");
        this->p_shader_4_tex_with_parallax_uni_modelMatrix = glGetUniformLocation(this->p_shader_4_tex_with_parallax, "modelMatrix");
        this->p_shader_4_tex_with_parallax_uni_normalSampler = glGetUniformLocation(this->p_shader_4_tex_with_parallax, "normalSampler");
        this->p_shader_4_tex_with_parallax_uni_transformation = glGetUniformLocation(this->p_shader_4_tex_with_parallax, "transformation");
    }
    LOAD_PROGRAM(shader_color, 2, "shader_color.frag", "shader_color.vert") {
        // this->dumpProgram("shader_color", this->p_shader_color);
        this->p_shader_color_attr_vertexNormal = glGetAttribLocation(this->p_shader_color, "vertexNormal");
        this->p_shader_color_attr_vertexPosition = glGetAttribLocation(this->p_shader_color, "vertexPosition");
        this->p_shader_color_uni_lightDir = glGetUniformLocation(this->p_shader_color, "lightDir");
        this->p_shader_color_uni_modelMatrix = glGetUniformLocation(this->p_shader_color, "modelMatrix");
        this->p_shader_color_uni_modelViewProjectionMatrix = glGetUniformLocation(this->p_shader_color, "modelViewProjectionMatrix");
        this->p_shader_color_uni_objectColor = glGetUniformLocation(this->p_shader_color, "objectColor");
    }
    LOAD_PROGRAM(shader_tex, 2, "shader_tex.frag", "shader_tex.vert") {
        // this->dumpProgram("shader_tex", this->p_shader_tex);
        this->p_shader_tex_attr_vertexNormal = glGetAttribLocation(this->p_shader_tex, "vertexNormal");
        this->p_shader_tex_attr_vertexPosition = glGetAttribLocation(this->p_shader_tex, "vertexPosition");
        this->p_shader_tex_attr_vertexTexCoord = glGetAttribLocation(this->p_shader_tex, "vertexTexCoord");
        this->p_shader_tex_uni_lightDir = glGetUniformLocation(this->p_shader_tex, "lightDir");
        this->p_shader_tex_uni_modelMatrix = glGetUniformLocation(this->p_shader_tex, "modelMatrix");
        this->p_shader_tex_uni_modelViewProjectionMatrix = glGetUniformLocation(this->p_shader_tex, "transformation");
        this->p_shader_tex_uni_textureSampler = glGetUniformLocation(this->p_shader_tex, "textureSampler");
    }

    // WATER SURFACE
    LOAD_PROGRAM(water_simulation, 2, "water/simulation.frag", "water/simulation.vert") {
        // this->dumpProgram("water_simulation", this->p_water_simulation);
        this->p_water_simulation_attr_vertexPosition = glGetAttribLocation(this->p_water_simulation, "vertexPosition");
        this->p_water_simulation_uni_scale = glGetUniformLocation(this->p_water_simulation, "scale");
        this->p_water_simulation_uni_time = glGetUniformLocation(this->p_water_simulation, "time");
        this->p_water_simulation_uni_transition = glGetUniformLocation(this->p_water_simulation, "transition");
        this->p_water_simulation_uni_waveCount = glGetUniformLocation(this->p_water_simulation, "waveCount");
    }
    LOAD_PROGRAM(water_surface, 2, "water/surface.frag", "water/surface.vert") {
        // this->dumpProgram("water_surface", this->p_water_surface);
        this->p_water_surface_attr_position = glGetAttribLocation(this->p_water_surface, "position");
        this->p_water_surface_attr_texturePosition = glGetAttribLocation(this->p_water_surface, "texturePosition");
        this->p_water_surface_uni_cameraPosition = glGetUniformLocation(this->p_water_surface, "cameraPosition");
        this->p_water_surface_uni_heightMap = glGetUniformLocation(this->p_water_surface, "heightMap");
        this->p_water_surface_uni_model = glGetUniformLocation(this->p_water_surface, "model");
        this->p_water_surface_uni_normalMap = glGetUniformLocation(this->p_water_surface, "normalMap");
        this->p_water_surface_uni_skybox = glGetUniformLocation(this->p_water_surface, "skybox");
        this->p_water_surface_uni_transformation = glGetUniformLocation(this->p_water_surface, "transformation");
    }

    LOAD_PROGRAM(simplex, 2, "simplex.frag", "simplex.vert") {
        // this->dumpProgram("simplex", this->p_simplex);
        this->p_simplex_attr_vertexPosition = glGetAttribLocation(this->p_simplex, "vertexPosition");
        this->p_simplex_uni_alpha = glGetUniformLocation(this->p_simplex, "alpha");
        this->p_simplex_uni_p = glGetUniformLocation(this->p_simplex, "p");
        this->p_simplex_uni_scale = glGetUniformLocation(this->p_simplex, "scale");
    }

#undef LOAD_PROGRAM
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
    this->loadPrograms();

    this->totalResources = this->totalResourcesCounter;

    if (this->totalResources == this->loadedResources) {
        if (this->shadersCap > 0) {
            this->shadersCap = 0;
            free(this->shaders);
        }
        LOGI("[ %3.0f%% ] Done", this->loadedResources * 100.0 / this->totalResources);
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
    if (glfwGetTime() - this->startFrame < 0.00001) {
        return true;
    } else {
        this->startFrame = 0;
        return false;
    }
}

void ResourceLoader::loadTextureCubeMap(GLuint *out) {
    this->totalResourcesCounter++;
    if (*out) {
        this->loadedResources++;
        return;
    }
    if (!canLoadNextResource()) {
        return;
    }
    LOGI("[ %3.0f%% ] Loading txt: %s", this->loadedResources * 100.0 / this->totalResources, "CubeMap");
    char *names[] = {"assets/textures/skybox/left.png", "assets/textures/skybox/right.png", "assets/textures/skybox/top.png", "assets/textures/skybox/bottom.png", "assets/textures/skybox/front.png", "assets/textures/skybox/back.png"};
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    int w, h;
    for (unsigned int i = 0; i < 6; i++) {
        unsigned char *image = SOIL_load_image(names[i], &w, &h, 0, SOIL_LOAD_RGBA);
        if (!image) {
            LOGE("Failed loading %s: %s", names[i], SOIL_last_result());
        } else {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
            SOIL_free_image_data(image);
        }
    }
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    *out = id;
    this->loadedResources++;
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
    LOGI("[ %3.0f%% ] Loading txt: %s", this->loadedResources * 100.0 / this->totalResources, name);
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

bool ResourceLoader::loadProgram(const char *name, GLuint *out_program, bool *out_loaded, int shadersCount, ...) {
    this->totalResourcesCounter += shadersCount + 1;
    if (*out_loaded) {
        this->loadedResources += shadersCount + 1;
        return false;
    }
    if (!canLoadNextResource()) {
        return false;
    }
    if (this->shadersCap < shadersCount) {
        GLuint *newShaders = (GLuint *)realloc(this->shaders, shadersCount * sizeof(GLuint));
        if (newShaders == 0) {
            LOGE("realloc(%d) failed: %d (%s)", shadersCount * sizeof(GLuint), errno, strerror(errno));
            return false;
        }
        this->shaders = newShaders;
        this->shadersCap = shadersCount;
    }

    va_list ap;
    va_start(ap, shadersCount);
    for (int i = 0; i < shadersCount; i++) {
        if (i < this->shadersCompiled) {
            va_arg(ap, const char *);
            this->loadedResources++;
            continue;
        }
        if (!canLoadNextResource()) {
            va_end(ap);
            return false;
        }
        const char *shaderName = va_arg(ap, const char *);
        size_t shaderNameLen = strlen(shaderName);
        char *fullPath = (char *)malloc(shaderNameLen + sizeof("assets/shaders/"));
        memcpy(fullPath, "assets/shaders/", sizeof("assets/shaders/"));
        memcpy(fullPath + sizeof("assets/shaders/") - 1, shaderName, shaderNameLen + 1);
        LOGI("[ %3.0f%% ] Loading shader: %s", this->loadedResources * 100.0 / this->totalResources, shaderName);
        this->shaders[i] = compileShader(i % 2 != 0 ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER, fullPath);
        this->shadersCompiled++;
        this->loadedResources++;
    }
    va_end(ap);

    if (!canLoadNextResource()) {
        return false;
    }

    if (this->shadersCompiled == shadersCount)  // always true?
    {
        LOGI("[ %3.0f%% ] Loading program: %s", this->loadedResources * 100.0 / this->totalResources, name);
        *out_loaded = true;
        *out_program = this->createProgram(name, shadersCount, this->shaders);
        this->shadersCompiled = 0;
        this->loadedResources++;
        return true;
    }
    return false;
}

char *ResourceLoader::readFile(const char *file, size_t *size) {
    FILE *f = fopen(file, "rb");
    if (!f) {
        LOGE("fopen(%s): %d (%s)", file, errno, strerror(errno));
        return 0;
    }
    goto readFile;
closeFile:
    if (fclose(f)) {
        LOGE("fclose: %d (%s)", errno, strerror(errno));
    }
    *size = 0;
    return 0;
readFile:
    if (fseek(f, 0, SEEK_END)) {
        LOGE("fseek(f, 0, SEEK_END): %d (%s)", errno, strerror(errno));
        goto closeFile;
    }
    long int s = ftell(f);
    if (s <= (long int)0) {
        LOGE("ftell(f): %d (%s)", errno, strerror(errno));
        goto closeFile;
    }
    if (fseek(f, 0, SEEK_SET)) {
        LOGE("fseek(f, 0, SEEK_SET): %d (%s)", errno, strerror(errno));
        goto closeFile;
    }
    char *mem = (char *)malloc(s);
    *size = s;
    size_t r = fread(mem, 1, s, f);
    if (r != s) {
        LOGE("fread %d != %d (ret, size): %d (%s)", r, s, errno, strerror(errno));
        free(mem);
        goto closeFile;
    }
    if (fclose(f)) {
        LOGE("fclose: %d (%s)", errno, strerror(errno));
    }
    return mem;
}

void ResourceLoader::saveFile(const char *file, const char *data, size_t size) {
    FILE *f = fopen(file, "wb");
    if (!f) {
        LOGE("fopen(%s): %d (%s)", file, errno, strerror(errno));
        return;
    }
    goto writeFile;
closeFile:
    if (fclose(f)) {
        LOGE("fclose: %d (%s)", errno, strerror(errno));
    }
    return;
writeFile:
    size_t r = fwrite(data, 1, size, f);
    if (r != size) {
        LOGE("fwrite %d != %d (ret, size): %d (%s)", r, size, errno, strerror(errno));
        goto closeFile;
    }
    if (fclose(f)) {
        LOGE("fclose: %d (%s)", errno, strerror(errno));
    }
    return;
}

GLuint ResourceLoader::compileShader(GLenum shaderType, const char *name) {
    size_t size;
    char *file = this->readFile(name, &size);
    if (!file) {
        return 0;
    }
    GLuint shader = glCreateShader(shaderType);
    if (shader == 0) {
        LOGE("glCreateShader failed");
        free(file);
        return 0;
    }
    const GLint length = (const GLint)size;
    glShaderSource(shader, 1, &file, &length);
    glCompileShader(shader);
    free(file);
    GLint compile_result = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);
    if (compile_result == GL_FALSE) {
        LOGE("Error compiling shader %s", name);
        GLint info_log_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
        char *mem = (char *)malloc(info_log_length);
        if (mem) {
            glGetShaderInfoLog(shader, info_log_length, NULL, mem);
            LOGE("Logs: %s", mem);
            free(mem);
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint ResourceLoader::createProgram(const char *name, int shadersCount, GLuint *shaders) {
    GLuint program = glCreateProgram();
    if (program == 0) {
        LOGE("glCreateProgram failed, unloading shaders");
        for (int i = 0; i < shadersCount; i++) {
            if (shaders[i]) {
                glDeleteShader(shaders[i]);
            }
        }
        return 0;
    }
    for (int i = 0; i < shadersCount; i++) {
        if (shaders[i]) {
            glAttachShader(program, shaders[i]);
        }
    }
    glLinkProgram(program);

    for (int i = 0; i < shadersCount; i++) {
        if (shaders[i]) {
            glDetachShader(program, shaders[i]);
            glDeleteShader(shaders[i]);
        }
    }

    GLint link_result = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &link_result);
    if (link_result == GL_FALSE) {
        LOGE("Error linking program %s", name);
        GLint info_log_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
        char *mem = (char *)malloc(info_log_length);
        if (mem) {
            glGetProgramInfoLog(program, info_log_length, NULL, mem);
            LOGE("Logs: %s", mem);
            free(mem);
        }
        glDeleteProgram(program);
        return 0;
    }
    return program;
}

void ResourceLoader::dumpProgram(const char *name, GLuint program) {
    if (program == 0) {
        LOGW("Invalid program: %s", name);
        return;
    }
    printf("Header code:\n");
    printf("    // %s:\n", name);
    glUseProgram(program);
    GLint i;
    GLint count;
    GLint size;
    GLenum type;
    printf("    GLuint p_%s = 0;\n", name);
    printf("    bool p_%s_loaded = false;\n", name);
    GLint bufSize = 16;
    GLsizei length;
    GLchar *glname;

    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &bufSize);
    glname = (GLchar *)malloc(bufSize);
    for (i = 0; i < count; i++) {
        glGetActiveAttrib(program, (GLuint)i, bufSize, &length, &size, &type, glname);
        printf("    GLint p_%s_attr_%s = -1;\n",
               name, glname);
    }
    free(glname);
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &bufSize);
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
    glname = (GLchar *)malloc(bufSize);
    for (i = 0; i < count; i++) {
        glGetActiveUniform(program, (GLuint)i, bufSize, &length, &size, &type, glname);
        printf("    GLint p_%s_uni_%s = -1;\n",
               name, glname);
    }
    printf("\n");
    free(glname);

    printf("Source code:\n");
    printf("    {\n");

    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &count);
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &bufSize);
    glname = (GLchar *)malloc(bufSize);
    for (i = 0; i < count; i++) {
        glGetActiveAttrib(program, (GLuint)i, bufSize, &length, &size, &type, glname);
        printf("        this->p_%s_attr_%s = glGetAttribLocation(this->p_%s, \"%s\");\n",
               name, glname, name, glname);
    }
    free(glname);
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &bufSize);
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
    glname = (GLchar *)malloc(bufSize);
    for (i = 0; i < count; i++) {
        glGetActiveUniform(program, (GLuint)i, bufSize, &length, &size, &type, glname);
        printf("        this->p_%s_uni_%s = glGetUniformLocation(this->p_%s, \"%s\");\n",
               name, glname, name, glname);
    }
    free(glname);

    printf("    }\n");
}

void ResourceLoader::loadTextureExternal(char *name, GLuint *out) {
    return resourceLoaderExternal->loadTexture(name, out);
}

void ResourceLoader::loadModelExternal(char *name, Model *out) {
    Model* model = new Model();
    model->loadModel(name);

    *out = *model;
}

ResourceLoader *resourceLoaderExternal;