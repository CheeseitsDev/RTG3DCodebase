#pragma once
#include "core.h"
#include <stdio.h>
#include <string>
#include "RenderPass.h"

using namespace std;
class Scene;

using namespace glm;

//Base class of a GameObject
class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	//load me from the file
	virtual void Load(ifstream& _file);

	//update the GameObject
	//TODO: possibly pass keyboard / mouse stuff down here for player controls?
	virtual void Tick(float _dt);

	virtual void PreRender();//set up any shader values needed for this object
	virtual void Render();//render this object

	//various getters and setters
	void SetName(string _name) { m_name = _name; }
	string GetName() { return m_name; }
	GLuint GetShaderProg() { return m_ShaderProg; }

	//scene maybe needed for more involved cameras to connect to relvant GOs and lights/shaders etc
	virtual void Init(Scene* _scene);

	//this GameObject should be drawn in THIS render pass
	RenderPass GetRP() { return m_RP; }

	glm::vec3 GetPos() { return m_pos; }

protected:

	string m_name;
	string m_type;

	vec3		m_pos;
	vec3		m_rot;
	vec3		m_scale;
	vec3		m_rot_incr;

	glm::mat4	m_worldMatrix;

	GLuint m_ShaderProg;

	RenderPass m_RP = RP_OPAQUE;
};

