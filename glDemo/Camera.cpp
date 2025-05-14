#include "Camera.h"
#include "helper.h"
#include <fstream>
#include <iostream>
#include "stringHelp.h"
#include "FPSCam.h"
#include "ArcballCamera.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////
// constructor
/////////////////////////////////////////////////////////////////////////////////////
Camera::Camera()
{
	m_type = "CAMERA";
}

/////////////////////////////////////////////////////////////////////////////////////
// destructor
/////////////////////////////////////////////////////////////////////////////////////
Camera::~Camera()
{
}

/////////////////////////////////////////////////////////////////////////////////////
// Init() - 
/////////////////////////////////////////////////////////////////////////////////////
void Camera::Init(float _screenWidth, float _screenHeight, Scene* _scene)
{
	//TODO: move the calculation of the Projection Matrix to Camera::Tick
	// so that we can do the same rescaling of the aspect ratio to match the current window
	float aspect_ratio = _screenWidth / _screenHeight;
	m_projectionMatrix = glm::perspective(glm::radians(m_fov), aspect_ratio, m_near, m_far);
}

/////////////////////////////////////////////////////////////////////////////////////
// Update() - 
/////////////////////////////////////////////////////////////////////////////////////
void Camera::Tick(float _dt, float x, float y, string currentCam, int input)
{
	if (currentCam == "FPSCAM")
	{
		fCam = dynamic_cast<FPSCam*>(this);
		aCam = nullptr;
	}
	else if (currentCam == "ACAM")
	{
		aCam = dynamic_cast<ArcballCamera*>(this);
		fCam = nullptr;
	}

	if (fCam)
	{
		// Update FPSCam view matrix (for FPS-style cameras)
		fCam->processKeyboard(_dt, input);
		fCam->processMouseMovement(x, y, true);

		// Update the view matrix using the FPSCam method
		m_viewMatrix = fCam->viewTransform();
	}
	else if (aCam)
	{
		// Update ArcballCamera view matrix (for arcball-style cameras)
		aCam->rotateCamera(x, y);
		// Update the view matrix using the ArcballCamera method
		m_viewMatrix = aCam->viewTransform();
	}
	else
	{
		// Default camera view matrix logic (if the camera is not FPSCam)
		m_viewMatrix = glm::lookAt(m_pos, m_lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
	}
}

void Camera::Load(ifstream& _file)
{
	StringHelp::String(_file, "NAME", m_name);
	StringHelp::Float3(_file, "POS", m_pos.x, m_pos.y, m_pos.z);
	StringHelp::Float3(_file, "LOOKAT", m_lookAt.x, m_lookAt.y, m_lookAt.z);
	StringHelp::Float(_file, "FOV", m_fov);
	StringHelp::Float(_file, "NEAR", m_near);
	StringHelp::Float(_file, "FAR", m_far);
}

//set the base render values for this camera in the shaders
void Camera::SetRenderValues(unsigned int _prog)
{
	GLint loc;

	//matrix for the view transform
	if (Helper::SetUniformLocation(_prog, "viewMatrix", &loc))
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(GetView()));

	//matrix for the projection transform
	if (Helper::SetUniformLocation(_prog, "projMatrix", &loc))
		glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(GetProj()));

	//the current camera is at this position
	if (Helper::SetUniformLocation(_prog, "camPos", &loc))
		glUniform3fv(loc, 1, glm::value_ptr(GetPos()));
}
