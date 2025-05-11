#include "FPSCam.h"

// Constructor with default values for position, yaw, pitch, speed, and sensitivity
FPSCam::FPSCam()
    : m_position(glm::vec3(0.0f, 0.0f, 3.0f)),
    m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
    m_up(glm::vec3(0.0f, 1.0f, 0.0f)),
    m_right(glm::vec3(1.0f, 0.0f, 0.0f)),
    m_worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
    m_yaw(-90.0f), m_pitch(0.0f),
    m_speed(2.5f), m_sensitivity(0.1f)
{
    calculateDerivedValues();
}

// Constructor with custom values for position, yaw, pitch, speed, and sensitivity
FPSCam::FPSCam(glm::vec3 position, float yaw, float pitch, float speed, float sensitivity)
    : m_position(position), m_yaw(yaw), m_pitch(pitch), m_speed(speed), m_sensitivity(sensitivity)
{
    m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    calculateDerivedValues();
}

// Update the camera's orientation based on mouse movement
void FPSCam::processMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
    xOffset *= m_sensitivity;
    yOffset *= m_sensitivity;

    m_yaw += xOffset;
    m_pitch += yOffset;

    // Constrain pitch to avoid flipping the camera
    if (constrainPitch)
    {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    calculateDerivedValues();
}

// Update the camera's position based on keyboard input and delta time
void FPSCam::processKeyboard(float deltaTime, int input)
{
    float velocity = m_speed * deltaTime;

    if (input == 1)
    {
        m_position += m_front * velocity;
    }
    if (input == 2)
    {
        m_position -= m_right * velocity;
    }
    if (input == 3)
    {
        m_position -= m_front * velocity;
    }
    if (input == 4)
    {
        m_position += m_right * velocity;
    }
}

// Calculate the camera's direction vectors based on the yaw and pitch angles
void FPSCam::calculateDerivedValues()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    // Recalculate the right and up vectors
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // Right vector
    m_up = glm::normalize(glm::cross(m_right, m_front));       // Up vector
}

// Return the camera's view matrix
glm::mat4 FPSCam::viewTransform()
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

// Return the camera's projection matrix
glm::mat4 FPSCam::projectionTransform()
{
    return glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
}

// Return the camera's position in world space
glm::vec3 FPSCam::getPosition()
{
    return m_position;
}

// Return the camera's front direction
glm::vec3 FPSCam::getFront()
{
    return m_front;
}

// Set the camera's position
void FPSCam::setPosition(glm::vec3 position)
{
    m_position = position;
}

// Set the camera's movement speed
void FPSCam::setSpeed(float speed)
{
    m_speed = speed;
}

// Set the mouse sensitivity for camera rotation
void FPSCam::setSensitivity(float sensitivity)
{
    m_sensitivity = sensitivity;
}