#pragma once
#include "GameObject.h"
class Model;

//replicate the examples from the main.cpp
//each Game Object has a Mesh, a texture and a shader
//which it uses to render itself
class TransparentGO :
	public GameObject
{
public:
	TransparentGO();
	~TransparentGO();

	//load me from the file
	virtual void Load(ifstream& _file);

	//update _window allows for Keyboard access
	virtual void Tick(float _dt);

	//render this object
	virtual void PreRender();
	virtual void Render();

	virtual void Init(Scene* _scene);

protected:

	string m_ShaderName, m_TexName, m_ModelName;

	GLuint m_texture;
	Model* m_model;
};

