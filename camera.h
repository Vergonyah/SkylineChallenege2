#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Camera
{
public:
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float sensitivity);

    glm::mat4 GetViewMatrix();
    void ProcessKeyboard(float deltaTime, bool forward, bool backward, bool left, bool right);
    void ProcessMouseMovement(float xoffset, float yoffset);

    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;
    float MovementSpeed;
    float MouseSensitivity;

private:
    void updateCameraVectors();
};

#endif // CAMERA_H
