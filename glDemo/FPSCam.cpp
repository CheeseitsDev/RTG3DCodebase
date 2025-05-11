#include "FPSCam.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace std;
using namespace glm;

// Private API

// Update position, orientation and view matrices based on the camera's rotation (yaw, pitch) and position
void FPSCam::calculateDerivedValues() {
    // Calculate the direction vector from the yaw and pitch
    const float theta_ = glm::radians<float>(m_theta);  // Pitch (up/down)
    const float phi_ = glm::radians<float>(m_phi);      // Yaw (left/right)

    // Front, Right, and Up vectors
    glm::vec3 front;
    front.x = cosf(theta_) * sinf(phi_);
    front.y = sinf(theta_);
    front.z = cosf(theta_) * cosf(phi_);
    m_front = glm::normalize(front);

    // Calculate the right and up vectors
    m_right = glm::normalize(glm::cross(m_front, glm::vec3(0.0f, 1.0f, 0.0f)));  // Cross product to get the right vector
    m_up = glm::normalize(glm::cross(m_right, m_front));  // Up is the cross of right and front

    // Calculate the view matrix
    m_viewMatrix = glm::lookAt(m_position, m_position + m_front, m_up);

    // Calculate the projection matrix
    m_projectionMatrix = glm::perspective(glm::radians<float>(m_fovY), m_aspect, m_nearPlane, m_farPlane);
}


// Public method implementation

// FPSCam constructors

// Initialise the camera parameters for a first-person shooter style camera
FPSCam::FPSCam() {
    m_position = glm::vec3(0.0f, 1.75f, 5.0f);  // Starting position, typically above ground level
    m_theta = 0.0f;
    m_phi = -90.0f;  // Looking down the negative z-axis (standard for FPS)
    m_fovY = 55.0f;
    m_aspect = 1.0f;
    m_nearPlane = 0.1f;
    m_farPlane = 500.0f;

    calculateDerivedValues();
}

// Create an FPS camera with initial parameters
FPSCam::FPSCam(float _theta, float _phi, glm::vec3 _position, float _fovY, float _aspect, float _nearPlane, float _farPlane) {
    m_position = _position;
    m_theta = _theta;
    m_phi = _phi;
    m_fovY = _fovY;
    m_aspect = _aspect;
    m_nearPlane = _nearPlane;
    m_farPlane = _farPlane;

    calculateDerivedValues();
}

// Accessor methods for stored values

float FPSCam::getTheta() {
    return m_theta;
}

float FPSCam::getPhi() {
    return m_phi;
}

void FPSCam::rotateCamera(float _dTheta, float _dPhi) {
    m_theta += _dTheta;
    m_phi += _dPhi;
    calculateDerivedValues();
}

glm::vec3 FPSCam::getPosition() {
    return m_position;
}

void FPSCam::moveForward(float deltaTime) {
    if (isMoving)
    {
        m_position += m_front * m_moveSpeed * deltaTime;
        calculateDerivedValues();
    }
}

void FPSCam::moveBackward(float deltaTime) {
    if (isMoving)
    {
        m_position -= m_front * m_moveSpeed * deltaTime;
        calculateDerivedValues();
    }
}

void FPSCam::strafeLeft(float deltaTime) {
    if (isMoving)
    {
        m_position -= m_right * m_moveSpeed * deltaTime;
        calculateDerivedValues();
    }
}

void FPSCam::strafeRight(float deltaTime) {
    if (isMoving)
    {
        m_position += m_right * m_moveSpeed * deltaTime;
        calculateDerivedValues();
    }
}

void FPSCam::moveUp(float deltaTime) {
    m_position += m_up * m_moveSpeed * deltaTime;
    calculateDerivedValues();
}

void FPSCam::moveDown(float deltaTime) {
    m_position -= m_up * m_moveSpeed * deltaTime;
    calculateDerivedValues();
}

float FPSCam::getFovY() {
    return m_fovY;
}

void FPSCam::setFovY(float _fovY) {
    this->m_fovY = _fovY;
    calculateDerivedValues();
}

float FPSCam::getAspect() {
    return m_aspect;
}

void FPSCam::setAspect(float _aspect) {
    this->m_aspect = _aspect;
    calculateDerivedValues();
}

float FPSCam::getNearPlaneDistance() {
    return m_nearPlane;
}

void FPSCam::setNearPlaneDistance(float _nearPlaneDistance) {
    this->m_nearPlane = _nearPlaneDistance;
    calculateDerivedValues();
}

float FPSCam::getFarPlaneDistance() {
    return m_farPlane;
}

void FPSCam::setFarPlaneDistance(float _farPlaneDistance) {
    this->m_farPlane = _farPlaneDistance;
    calculateDerivedValues();
}

void FPSCam::inputManager(int input)
{

}

// Accessor methods for derived values

glm::mat4 FPSCam::viewTransform() {
    return m_viewMatrix;
}

glm::mat4 FPSCam::projectionTransform() {
    return m_projectionMatrix;
}