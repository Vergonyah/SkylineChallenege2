// camera.h
// Defines the Camera class for managing the 3D camera in the scene

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Camera {
public:
  // Constructor
  Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch,
         float sensitivity);

  // Public methods
  glm::mat4 GetViewMatrix();
  void ProcessKeyboard(float deltaTime, bool forward, bool backward, bool left,
                       bool right);
  void ProcessMouseMovement(float xoffset, float yoffset);

  // Camera attributes
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;

  // Euler angles
  float Yaw;
  float Pitch;

  // Camera options
  float MovementSpeed;
  float MouseSensitivity;

private:
  // Updates the camera's internal vectors based on the Euler angles
  void updateCameraVectors();
};

#endif // CAMERA_H