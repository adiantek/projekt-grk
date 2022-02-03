#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>

class Camera {
   public:
    Camera(int width, int height, float fov = 80.0f, float near = 0.1f, float far = 1000.0f, int x = 0, int y = 0);
    ~Camera();
    /* Get camera and perspective transformation matrix */
    glm::mat4 getTransformationMatrix();
    /* Set opengl camera viewport */
    void useCameraViewport();
    /* Set viewport size */
    void setSize(int width, int height);
    /* Set camera field of view */
    void setFov(float fov);
    /* Set camera direction */
    void setDirection();

    void mouseMove(double deltaX, double deltaY);

    /* Increase camera distance of view */
    void increaseCameraDistance();
    /* Decrease camera distance of view */
    void decreaseCameraDistance();
    /* Camera update */
    void update();

    glm::vec3 getPosition();
    float getPitch();

    float angleYawChange;
    float anglePitchChange;

    float horizontalDistance();
    float verticalDistance();

    void calculateCameraPosition(float horizontalD, float verticalD);

    /* Get camera direction vector */
    glm::vec3 getDirection();
    /* Camera position in world */
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

   protected:
    /* Update camera perspective matrix */
    void updatePerspective();

    glm::mat4 rotationMatrix;
    /* Perspective matrix */
    glm::mat4 perspective;

    /* Camera target position */
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 6.0f);
    /* Camera direction vector */
    glm::vec3 direction = glm::normalize(position - cameraTarget);

    /* default view distance value */
    float distance = 1.4f;

    float angleYaw = 180;
    float anglePitch = 20;
    float yaw = 0;

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