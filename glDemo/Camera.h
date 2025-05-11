#pragma once
#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp" 
#include "glm/gtc/type_ptr.hpp"
#include "core.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

using namespace glm;

class cTransform;
class Light;
class Scene;

//base class for a camera

class Camera
{
public:
	Camera();
	virtual ~Camera();

	//initialise the camera _width _height
	//scene maybe needed for more involved cameras to connect to relvant GOs and lights/shaders etc
	//TODO move _w and _h to tick for cameras so apsect ratio can be updated if we change the size of the window
	virtual void Init(float _w, float _h, Scene* _scene);

	//tick this camera
	//TODO: possibly pass keyboard / mouse stuff down here for player controls?
	virtual void Tick(float _dt, int input);

	//load camera info from the mainfest
	virtual void Load(ifstream& _file);

	//getters
	string GetType() { return m_type; }

	glm::mat4 GetProj() { return m_projectionMatrix; }
	glm::mat4 GetView() { return m_viewMatrix; }

	glm::vec3 GetPos() { return m_pos; }

	float GetFOV() { return m_fov; }
	float GetNear() { return m_near; }
	float GetFar() { return m_far; }

	string GetName() { return m_name; }
	void SetName(string _name) { m_name = _name; }

	void Move(glm::vec3 _d) { m_pos += _d; }

	//where am I looking at
	vec3 GetLookAt() { return m_lookAt; }
	void SetLookAt(vec3 _pos) { m_lookAt = _pos; }

	//set up shader values for when using this camera
	virtual void SetRenderValues(unsigned int _prog);

	void SetAspect(float _aspect);

protected:

	//standard transforms needed to render with this a basic camera
	glm::mat4 m_projectionMatrix;		// projection matrix
	glm::mat4 m_viewMatrix;			// view matrix

	//my camera is here
	glm::vec3 m_pos;

	//Location this camera is looking at
	glm::vec3 m_lookAt;

	float m_fov; //field of view
	float m_near;//near plane distance
	float m_far;//far plane distance
	float m_aspect;

	string m_name;
	string m_type;
};

