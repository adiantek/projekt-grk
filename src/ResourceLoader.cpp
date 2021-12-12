#include <ResourceLoader.hpp>
#include <Logger.h>
#include <SOIL/SOIL.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <filesystem>
#include <unordered_set>

ResourceLoader::ResourceLoader()
{
}

ResourceLoader::~ResourceLoader()
{
}

void ResourceLoader::loadTextures()
{
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
    loadTexture("assets/textures/wall.jpg", &this->txt_wall);
    loadTexture("assets/textures/wall_normal.jpg", &this->txt_wallNormal);
    loadTexture("assets/textures/wall_height.jpg", &this->txt_wallHeight);
}

void ResourceLoader::loadPrograms()
{
#define LOAD_PROGRAM(name, count, ...) if (loadProgram(#name, &this->p_##name, &this->p_##name##_loaded, count, __VA_ARGS__))

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
        this->p_shader_4_tex_with_parallax_attr_vertexBitangent = glGetAttribLocation(this->p_shader_4_tex_with_parallax, "vertexBitangent");
        this->p_shader_4_tex_with_parallax_attr_vertexNormal = glGetAttribLocation(this->p_shader_4_tex_with_parallax, "vertexNormal");
        this->p_shader_4_tex_with_parallax_attr_vertexPosition = glGetAttribLocation(this->p_shader_4_tex_with_parallax, "vertexPosition");
        this->p_shader_4_tex_with_parallax_attr_vertexTangent = glGetAttribLocation(this->p_shader_4_tex_with_parallax, "vertexTangent");
        this->p_shader_4_tex_with_parallax_attr_vertexTexCoord = glGetAttribLocation(this->p_shader_4_tex_with_parallax, "vertexTexCoord");
        this->p_shader_4_tex_with_parallax_uni_cameraPos = glGetUniformLocation(this->p_shader_4_tex_with_parallax, "cameraPos");
        this->p_shader_4_tex_with_parallax_uni_colorTexture = glGetUniformLocation(this->p_shader_4_tex_with_parallax, "colorTexture");
        this->p_shader_4_tex_with_parallax_uni_lightPos = glGetUniformLocation(this->p_shader_4_tex_with_parallax, "lightPos");
        this->p_shader_4_tex_with_parallax_uni_modelMatrix = glGetUniformLocation(this->p_shader_4_tex_with_parallax, "modelMatrix");
        this->p_shader_4_tex_with_parallax_uni_normalSampler = glGetUniformLocation(this->p_shader_4_tex_with_parallax, "normalSampler");
        this->p_shader_4_tex_with_parallax_uni_heightSampler = glGetUniformLocation(this->p_shader_4_tex_with_parallax, "depthSampler");
        this->p_shader_4_tex_with_parallax_uni_transformation = glGetUniformLocation(this->p_shader_4_tex_with_parallax, "transformation");
        this->p_shader_4_tex_with_parallax_uni_heightScale = glGetUniformLocation(this->p_shader_4_tex_with_parallax, "heightScale");
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
        this->p_shader_tex_uni_modelViewProjectionMatrix = glGetUniformLocation(this->p_shader_tex, "modelViewProjectionMatrix");
        this->p_shader_tex_uni_textureSampler = glGetUniformLocation(this->p_shader_tex, "textureSampler");
    }

#undef LOAD_PROGRAM
}

bool ResourceLoader::loadResources()
{
    if (this->all_loaded)
    {
        return true;
    }
    this->loadedResources = 0;
    this->totalResourcesCounter = 0;

    if (this->totalResources == 0)
    {
        // first frame - count all resources
        this->startFrame = 0;
    }
    else
    {
        this->startFrame = glfwGetTime();
    }

    this->loadTextures();
    this->loadPrograms();

    this->totalResources = this->totalResourcesCounter;

    if (this->totalResources == this->loadedResources)
    {
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

bool ResourceLoader::canLoadNextResource()
{
    if (this->startFrame == 0)
    {
        // optimize - don't check time & don't load more resources
        return false;
    }
    if (this->totalResourcesCounter == this->loadedResources)
    {
        // load minimum one resource
        return true;
    }
    // max 100 ms per frame
    if (glfwGetTime() - this->startFrame < 0.00001)
    {
        return true;
    }
    else
    {
        this->startFrame = 0;
        return false;
    }
}

void ResourceLoader::loadTexture(const char *name, GLuint *out)
{
    this->totalResourcesCounter++;
    if (*out)
    {
        this->loadedResources++;
        return;
    }
    if (!canLoadNextResource())
    {
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
    if (!image)
    {
        LOGE("Failed loading %s: %s", name, SOIL_last_result());
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image);
    }
    *out = id;
    this->loadedResources++;
}

bool ResourceLoader::loadProgram(const char *name, GLuint *out_program, bool *out_loaded, int shadersCount, ...)
{
    this->totalResourcesCounter += shadersCount + 1;
    if (*out_loaded)
    {
        this->loadedResources += shadersCount + 1;
        return false;
    }
    if (!canLoadNextResource())
    {
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
            va_arg(ap, const char*);
            this->loadedResources++;
            continue;
        }
        if (!canLoadNextResource()) {
            va_end(ap);
            return false;
        }
        const char *shaderName = va_arg(ap, const char*);
        size_t shaderNameLen = strlen(shaderName);
        char *fullPath = (char *)malloc(shaderNameLen + sizeof("assets/shaders/"));
        memcpy(fullPath, "assets/shaders/", sizeof("assets/shaders/"));
        memcpy(fullPath + sizeof("assets/shaders/") - 1, shaderName, shaderNameLen + 1);
        LOGI("[ %3.0f%% ] Loading shader: %s", this->loadedResources * 100.0 / this->totalResources, shaderName);
        this->shaders[i] = compileShader(i != 0 ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER, fullPath);
        this->shadersCompiled++;
        this->loadedResources++;
    }
    va_end(ap);

    if (this->shadersCompiled == shadersCount)
    {
        LOGI("[ %3.0f%% ] Loading program: %s", this->loadedResources * 100.0 / this->totalResources, name);
        *out_loaded = true;
        *out_program = this->createProgram(name, shadersCount, this->shaders);
        this->shadersCompiled = 0;
        this->loadedResources++;
        return true;
    }
}

char *ResourceLoader::readFile(const char *file, size_t *size)
{
    FILE *f = fopen(file, "rb");
    if (!f)
    {
        LOGE("fopen(%s): %d (%s)", file, errno, strerror(errno));
        return 0;
    }
    goto readFile;
closeFile:
    if (fclose(f))
    {
        LOGE("fclose: %d (%s)", errno, strerror(errno));
    }
    *size = 0;
    return 0;
readFile:
    if (fseek(f, 0, SEEK_END))
    {
        LOGE("fseek(f, 0, SEEK_END): %d (%s)", errno, strerror(errno));
        goto closeFile;
    }
    long int s = ftell(f);
    if (s <= (long int)0)
    {
        LOGE("ftell(f): %d (%s)", errno, strerror(errno));
        goto closeFile;
    }
    if (fseek(f, 0, SEEK_SET))
    {
        LOGE("fseek(f, 0, SEEK_SET): %d (%s)", errno, strerror(errno));
        goto closeFile;
    }
    char *mem = (char *)malloc(s);
    *size = s;
    int r = fread(mem, 1, s, f);
    if (r != s)
    {
        LOGE("fread %d != %d (ret, size): %d (%s)", r, s, errno, strerror(errno));
        free(mem);
        goto closeFile;
    }
    if (fclose(f))
    {
        LOGE("fclose: %d (%s)", errno, strerror(errno));
    }
    return mem;
}

GLuint ResourceLoader::compileShader(GLenum shaderType, const char *name)
{
    size_t size;
    char *file = this->readFile(name, &size);
    if (!file)
    {
        return 0;
    }
    GLuint shader = glCreateShader(shaderType);
    if (shader == 0)
    {
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
    if (compile_result == GL_FALSE)
    {
        LOGE("Error compiling shader %s", name);
        GLint info_log_length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
        char *mem = (char *)malloc(info_log_length);
        if (mem)
        {
            glGetShaderInfoLog(shader, info_log_length, NULL, mem);
            LOGE("Logs: %s", mem);
            free(mem);
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint ResourceLoader::createProgram(const char *name, int shadersCount, GLuint *shaders)
{
    GLuint program = glCreateProgram();
    if (program == 0)
    {
        LOGE("glCreateProgram failed, unloading shaders");
        for (int i = 0; i < shadersCount; i++)
        {
            if (shaders[i]) {
                glDeleteShader(shaders[i]);
            }
        }
        return 0;
    }
    for (int i = 0; i < shadersCount; i++)
    {
        if (shaders[i]) {
            glAttachShader(program, shaders[i]);
        }
    }
    glLinkProgram(program);

    for (int i = 0; i < shadersCount; i++)
    {
        if (shaders[i]) {
            glDetachShader(program, shaders[i]);
            glDeleteShader(shaders[i]);
        }
    }

    GLint link_result = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &link_result);
    if (link_result == GL_FALSE)
    {
        LOGE("Error linking program %s", name);
        GLint info_log_length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
        char *mem = (char *)malloc(info_log_length);
        if (mem)
        {
            glGetProgramInfoLog(program, info_log_length, NULL, mem);
            LOGE("Logs: %s", mem);
            free(mem);
        }
        glDeleteProgram(program);
        return 0;
    }
    return program;
}

void ResourceLoader::dumpProgram(const char *name, GLuint program)
{
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
    for (i = 0; i < count; i++)
    {
        glGetActiveAttrib(program, (GLuint)i, bufSize, &length, &size, &type, glname);
        printf("    GLint p_%s_attr_%s = -1;\n",
            name, glname);
    }
    free(glname);
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &bufSize);
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
    glname = (GLchar *)malloc(bufSize);
    for (i = 0; i < count; i++)
    {
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
    for (i = 0; i < count; i++)
    {
        glGetActiveAttrib(program, (GLuint)i, bufSize, &length, &size, &type, glname);
        printf("        this->p_%s_attr_%s = glGetAttribLocation(this->p_%s, \"%s\");\n",
            name, glname, name, glname);
    }
    free(glname);
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &bufSize);
    glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &count);
    glname = (GLchar *)malloc(bufSize);
    for (i = 0; i < count; i++)
    {
        glGetActiveUniform(program, (GLuint)i, bufSize, &length, &size, &type, glname);
        printf("        this->p_%s_uni_%s = glGetUniformLocation(this->p_%s, \"%s\");\n",
            name, glname, name, glname);
    }
    free(glname);

    printf("    }\n");
}