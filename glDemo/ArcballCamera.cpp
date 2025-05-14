
#include "ArcballCamera.h"

using namespace std;
using namespace glm;

//
// Private API
//

// update position, orientation and view matrices when camera rotation and radius is modified
void ArcballCamera::calculateDerivedValues() {

	const float theta_ = glm::radians<float>(m_theta);
	const float phi_ = glm::radians<float>(m_phi);

	// calculate position vector
	//cameraPos = glm::vec4(sinf(phi_) * cosf(-theta_) * radius, sinf(-theta_) * radius, cosf(phi_) * cosf(-theta_) * radius, 1.0f);

	// calculate orientation basis R
	//R = glm::eulerAngleY(phi_) * glm::eulerAngleX(theta_);
		
	// calculate view and projection transform matrices
	m_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -m_radius)) * glm::eulerAngleX(-theta_) * glm::eulerAngleY(-phi_);
	m_projectionMatrix = glm::perspective(glm::radians<float>(m_fovY), m_aspect, m_nearPlane, m_farPlane);
}


//
// Public method implementation
//

// ArcballCamera constructors

// initialise camera parameters so it is placed at the origin looking down the -z axis (for a right-handed camera) or +z axis (for a left-handed camera)
ArcballCamera::ArcballCamera() {

	m_theta = 0.0f;
	m_phi = 0.0f;
	m_radius = 15.0f;

	m_fovY = 55.0f;
	m_aspect = 1.0f;
	m_nearPlane = 0.1f;
	m_farPlane = 500.0f;

	//F = ViewFrustum(55.0f, 1.0f, 0.1f, 500.0f);

	// calculate derived values
	calculateDerivedValues();
	//F.calculateWorldCoordPlanes(C, R);
}


// create a camera with orientation <theta, phi> representing Euler angles specified in degrees and Euclidean distance 'init_radius' from the origin.  The frustum / viewplane projection coefficients are defined in init_fovy, specified in degrees spanning the entire vertical field of view angle, init_aspect (w/h ratio), init_nearPlane and init_farPlane.  If init_farPlane = 0.0 (as determined by equalf) then the resulting frustum represents an infinite perspective projection.  This is the default
ArcballCamera::ArcballCamera(float _theta, float _phi, float _radius, float _fovY, float _aspect, float _nearPlane, float _farPlane) {

	this->m_theta = _theta;
	this->m_phi = _phi;
	this->m_radius = std::max<float>(0.0f, _radius);

	this->m_fovY = _fovY;
	this->m_aspect = _aspect;
	this->m_nearPlane = _nearPlane;
	this->m_farPlane = _farPlane;

	//F = ViewFrustum(init_fovy, init_aspect, init_nearPlane, init_farPlane);

	// calculate derived values
	calculateDerivedValues();
	//F.calculateWorldCoordPlanes(C, R);
}

#pragma region Accessor methods for stored values

// return the pivot rotation around the x axis (theta) in degrees
float ArcballCamera::getTheta() {

	return m_theta;
}

// return the pivot rotation around the y axis (phi) in degrees
float ArcballCamera::getPhi() {

	return m_phi;
}

void ArcballCamera::rotateCamera(float _dTheta, float _dPhi) {

	m_theta += _dTheta;
	m_phi += _dPhi;

	calculateDerivedValues();
}

float ArcballCamera::getRadius() {

	return m_radius;
}

void ArcballCamera::scaleRadius(float _s) {

	m_radius *= _s;
	calculateDerivedValues();
}

void ArcballCamera::incrementRadius(float _i) {

	m_radius = std::max<float>(m_radius + _i, 0.0f);
	calculateDerivedValues();
}

float ArcballCamera::getFovY() {

	return m_fovY;
}

void ArcballCamera::setFovY(float _fovY) {

	this->m_fovY = _fovY;
	calculateDerivedValues();
}

float ArcballCamera::getAspect() {

	return m_aspect;
}

void ArcballCamera::setAspect(float _aspect) {

	this->m_aspect = _aspect;
	calculateDerivedValues();
}

float ArcballCamera::getNearPlaneDistance() {

	return m_nearPlane;
}

void ArcballCamera::setNearPlaneDistance(float _nearPlaneDistance) {

	this->m_nearPlane = _nearPlaneDistance;
	calculateDerivedValues();
}

float ArcballCamera::getFarPlaneDistance() {

	return m_farPlane;
}

void ArcballCamera::setFarPlaneDistance(float _farPlaneDistance) {

	this->m_farPlane = _farPlaneDistance;
	calculateDerivedValues();
}

#pragma endregion


#pragma region Accessor methods for derived values

// return the camera location in world coordinate space
//glm::vec4 ArcballCamera::getPosition() {
//
//	return cameraPos;
//}

// return a const reference to the camera's orientation matrix in world coordinate space
//glm::mat4 ArcballCamera::getOrientationBasis() {
//
//	return R;
//}

// return a const reference to the view transform matrix for the camera
glm::mat4 ArcballCamera::viewTransform() {

	return m_viewMatrix;
}

// return a const reference the projection transform for the camera
glm::mat4 ArcballCamera::projectionTransform() {

	return m_projectionMatrix;
}

#pragma endregion