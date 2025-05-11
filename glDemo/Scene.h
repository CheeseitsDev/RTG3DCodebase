#pragma once
#include "core.h"
#include <list>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class GameObject;
class Camera;
class ArcballCamera;
class FPSCam;
class Light;
class Model;
class Texture;
class Shader;

//Note quite a proper scene graph but this contains data structures for all of our bits and pieces we want to draw
class Scene
{
public:
	Scene();
	~Scene();

	//tick all GOs
	void Update(float _dt);

	//add this GO to my list
	void AddGameObject(GameObject* _new);

	//return a pointer to a given thing by its name
	GameObject* GetGameObject(string _GOName);
	Camera* GetCamera(string _camName);
	Light* GetLight(string _lightName);
	Texture* GetTexture(string _texName);
	Model* GetModel(string _modelName);
	Shader* GetShader(string _shaderName);

	//Render Everything
	void Render();

	//set up all shader uniform values for all of our lights
	void SetShaderUniforms(GLuint _shaderprog);

	//load from file
	void Load(ifstream& _file);

	//initialise links between items in the scene
	void Init();

	void ACam();

	void FCam();

	void MoveCam(float x, float y);

	void StopCamMovement();

	void ZoomCamInAndOut(float radius);

	string CurrentCam();

	void SetInput(int input);

protected:

	//data structures containing pointers to all our stuff
	int m_numCameras = 0;
	int m_numLights = 0;
	int m_numGameObjects = 0;
	int m_numModels = 0;
	int m_numTextures = 0;
	int m_numShaders = 0;
	std::list<Camera*>    m_Cameras;
	std::list<Light*>    m_Lights;
	std::list<Model*>		m_Models;
	std::list<Texture*>		m_Textures;
	std::list<Shader*>		m_Shaders;
	std::list<GameObject*> m_GameObjects;

	Camera* m_useCamera = nullptr; //current main camera in use
	ArcballCamera* aCam = nullptr;
	FPSCam* fCam = nullptr;
	int m_useCameraIndex = 0;
	float moveX;
	float moveY;
	int input;
	//TODO: pass down the same keyboard input from main so that we skip through all the cameras
};

