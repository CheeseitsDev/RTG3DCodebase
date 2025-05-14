#pragma once

#include "core.h"
#include "Camera.h"

// Model an FPS camera that can move around and look around the scene, typically used for first-person shooters.
class FPSCam : public Camera {

private:
    glm::vec3 m_position;     // Camera position in world space
    glm::vec3 m_front;        // Camera front direction vector (view direction)
    glm::vec3 m_up;           // Camera up direction vector
    glm::vec3 m_right;        // Camera right direction vector (perpendicular to front and up)
    glm::vec3 m_worldUp;      // The world's up direction (usually positive Y axis)

    // Angles for the camera orientation
    float m_yaw;              // Yaw (rotation around the Y axis)
    float m_pitch;            // Pitch (rotation around the X axis)

    // Camera speed and sensitivity
    float m_speed = 10;            // Movement speed
    float m_sensitivity;      // Mouse sensitivity for looking around

    // Matrices for view and projection transforms
    glm::mat4 m_viewMatrix;   // View matrix (camera transformation)
    glm::mat4 m_projectionMatrix; // Projection matrix

    // Private API
    void calculateDerivedValues(); // Calculate derived values like front, right, and up vectors

public:
    // Constructors
    FPSCam(); // Default constructor (initializes to a specific position and orientation)
    FPSCam(glm::vec3 position, float yaw = -90.0f, float pitch = 0.0f,
        float speed = 2.5f, float sensitivity = 0.1f); // Custom constructor

    // Public methods to handle camera input
    void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true); // Mouse input to change camera orientation
    void processKeyboard(float deltaTime, int input); // Keyboard input to move camera

    // Accessor methods for camera properties
    glm::mat4 viewTransform();  // Return the camera's view matrix
    glm::mat4 projectionTransform(); // Return the camera's projection matrix
    glm::vec3 getPosition();    // Return the camera's position in world space
    glm::vec3 getFront();       // Return the camera's front direction

    // Setter methods
    void setPosition(glm::vec3 position); // Set the camera's position
    void setSpeed(float speed);          // Set the camera's movement speed
    void setSensitivity(float sensitivity); // Set the mouse sensitivity for rotation
};
