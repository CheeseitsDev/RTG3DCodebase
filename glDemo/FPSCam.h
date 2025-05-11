#pragma once

#include "core.h"
#include "Camera.h"

// First-Person Shooter Camera (FPSCam)
// The FPSCam class is designed for use in a first-person perspective, where the camera can move along
// the x, y, and z axes, and can rotate around the x (pitch) and y (yaw) axes.

class FPSCam : public Camera {

private:
    // Camera position in world space
    glm::vec3 m_position;

    // Camera rotation angles: pitch (theta) and yaw (phi) in degrees
    float m_theta, m_phi;

    // Camera movement speed (could be adjusted based on input)
    float m_moveSpeed = 5.0f;

    // Derived vectors for movement
    glm::vec3 m_front;  // The direction the camera is facing (forward)
    glm::vec3 m_right;  // The camera's right vector (strafe)
    glm::vec3 m_up;     // The camera's up vector (vertical movement)

    // Projection / frustum values
    float m_fovY, m_aspect, m_nearPlane, m_farPlane;

    // Derived matrices for the camera's view and projection
    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;


    // Private method to recalculate view and projection matrices based on camera state
    void calculateDerivedValues();

public:
    // Constructors
    FPSCam(); // Default constructor, initializes the camera with a default position and orientation.
    FPSCam(float _theta, float _phi, glm::vec3 _position, float _fovy, float _aspect, float _nearPlane, float _farPlane);

    // Accessor methods for stored properties
    float getTheta();               // Get the pitch (theta) in degrees
    float getPhi();                 // Get the yaw (phi) in degrees
    void rotateCamera(float _dTheta, float _dPhi); // Rotate the camera by delta theta and phi (in degrees)

    glm::vec3 getPosition();        // Get the camera's position in world coordinates

    // Camera movement methods
    void moveForward(float deltaTime);   // Move the camera forward
    void moveBackward(float deltaTime);  // Move the camera backward
    void strafeLeft(float deltaTime);    // Move the camera left (strafe)
    void strafeRight(float deltaTime);   // Move the camera right (strafe)
    void moveUp(float deltaTime);        // Move the camera upwards
    void moveDown(float deltaTime);      // Move the camera downwards

    // Accessor methods for projection/frustum values
    float getFovY();
    void setFovY(float _fovY);

    float getAspect();
    void setAspect(float _aspect);

    float getNearPlaneDistance();
    void setNearPlaneDistance(float _nearPlaneDistance);

    float getFarPlaneDistance();
    void setFarPlaneDistance(float _farPlaneDistance);

    void inputManager(int input);

    // Accessor methods for derived values
    glm::mat4 viewTransform();         // Get the view matrix for the camera
    glm::mat4 projectionTransform();   // Get the projection matrix for the camera

    bool isMoving = false;
};