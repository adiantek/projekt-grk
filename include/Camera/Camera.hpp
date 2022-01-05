#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Camera {
public:
    Camera(int width, int height, float fov = 80.0f, float near = 0.1f, float far = 100.0f, int x = 0, int y = 0);
    ~Camera();
    /* Get camera and perspective transformation matrix */
    glm::mat4 getTransformationMatrix();
    /* Set opengl camera viewport */
    void useCameraViewport();
    /* Set viewport size */
    void setSize(int width, int height);
    /* Set camera position on window */
    void setViewPosition(int x, int y);
    /* Set camera field of view */
    void setFov(float fov);
    /* Set camera rotation */
    void setRotation(glm::quat rotation);
    /* Rotate camera by quaternion */
    void rotate(glm::quat rotation);
    
    /* Increase camera distance of view */
    void increaseCameraDistance();
    /* Decrease camera distance of view */
    void decreaseCameraDistance();

    /* Get camera rotation quaternion */
    glm::quat getRotation();
    /* Get camera direction vector */
    glm::vec3 getDirection();
    /* Get camera up vector */
    glm::vec3 getUp();
    /* Get camera side vector */
    glm::vec3 getSide();
    /* Get camera width / height aspect ratio */
    float getAspect();
    /* Camera position in world */
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f);
    
protected:
    /* Update camera perspective matrix */
    void updatePerspective();
    /* Camera rotation quaternion */
    glm::quat rotation;
    /* Perspective matrix */
    glm::mat4 perspective;
    /* Camera direction vector */
    glm::vec3 direction;
    /* Camera up vector */
    glm::vec3 up;
    /* Camera side vector */
    glm::vec3 side;

    /* default view distance value */
    float distance = 1.0f;
    
    /* Perspective neat plane */
    float near;
    /* Perspective far plane */
    float far;
    /* Field of view in degrees */
    float fov;
    /* Viewport width */
    int width;
    /* Viewport height */
    int height;
    /* Viewport position x */
    int x;
    /* Viewport position y */
    int y;
};

extern Camera *camera;