#include "Scene.h"
#include "GameObject.h"
#include "CameraFactory.h"
#include "Camera.h"
#include "ArcballCamera.h"
#include "FPSCam.h"
#include "LightFactory.h"
#include "Light.h"
#include "ModelFactory.h"
#include "model.h"
#include "Texture.h"
#include "Shader.h"
#include "GameObjectFactory.h"
#include <assert.h>

Scene::Scene()
{
}

Scene::~Scene()
{
	//TODO: We are being really naught and not deleting everything as we finish
	//what shoudl really go here and in similar places throughout the code base?
}

//tick all my Game Objects, lights and cameras
void Scene::Update(float _dt)
{
	//update all lights
	for (list<Light*>::iterator it = m_Lights.begin(); it != m_Lights.end(); it++)
	{
		(*it)->Tick(_dt);
	}

	//update all cameras
	for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); it++)
	{
		(*it)->Tick(_dt, moveX, moveY, CurrentCam(), input);
	}

	//update all GameObjects
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		(*it)->Tick(_dt);
	}
}

void Scene::AddGameObject(GameObject* _new)
{
	m_GameObjects.push_back(_new);
}

//I want THAT Game Object by name
GameObject* Scene::GetGameObject(string _GOName)
{
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		if ((*it)->GetName() == _GOName)
		{
			return (*it);
		}
	}
	printf("Unknown Game Object NAME : %s \n", _GOName.c_str());
	assert(0);
	return nullptr;
}

Camera* Scene::GetCamera(string _camName)
{
	for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); it++)
	{
		if ((*it)->GetName() == _camName)
		{
			return (*it);
		}
	}
	printf("Unknown Camera NAME : %s \n", _camName.c_str());
	assert(0);
	return nullptr;
}

Light* Scene::GetLight(string _lightName)
{
	for (list<Light*>::iterator it = m_Lights.begin(); it != m_Lights.end(); it++)
	{
		if ((*it)->GetName() == _lightName)
		{
			return (*it);
		}
	}
	printf("Unknown Light NAME : %s \n", _lightName.c_str());
	assert(0);
	return nullptr;
}

Texture* Scene::GetTexture(string _texName)
{
	for (list<Texture*>::iterator it = m_Textures.begin(); it != m_Textures.end(); it++)
	{
		if ((*it)->GetName() == _texName)
		{
			return (*it);
		}
	}
	printf("Unknown Texture NAME : %s \n", _texName.c_str());
	assert(0);
	return nullptr;
}

Model* Scene::GetModel(string _modelName)
{
	for (list<Model*>::iterator it = m_Models.begin(); it != m_Models.end(); it++)
	{
		if ((*it)->GetName() == _modelName)
		{
			return (*it);
		}
	}
	printf("Unknown Model NAME : %s \n", _modelName.c_str());
	assert(0);
	return nullptr;
}

Shader* Scene::GetShader(string _shaderName)
{
	for (list<Shader*>::iterator it = m_Shaders.begin(); it != m_Shaders.end(); it++)
	{
		if ((*it)->GetName() == _shaderName)
		{
			return (*it);
		}
	}
	printf("Unknown Shader NAME : %s \n", _shaderName.c_str());
	assert(0);
	return nullptr;
}


//Render Everything
void Scene::Render()
{
	//TODO: Set up for the Opaque Render Pass will go here
	//check out the example stuff back in main.cpp to see what needs setting up here
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		if ((*it)->GetRP() & RP_OPAQUE)// TODO: note the bit-wise operation. Why?
		{
			//set shader program using
			GLuint SP = (*it)->GetShaderProg();
			glUseProgram(SP);

			//set up for uniform shader values for current camera
			m_useCamera->SetRenderValues(SP);

			if (m_useCamera->GetName() == "ACAM")
			{
				aCam = dynamic_cast<ArcballCamera*>(m_useCamera);

				mat4 view = aCam->viewTransform();
				mat4 projection = aCam->projectionTransform();
			}

			if (m_useCamera->GetName() == "FPSCAM")
			{
				fCam = dynamic_cast<FPSCam*>(m_useCamera);
			}

			//loop through setting up uniform shader values for anything else
			SetShaderUniforms(SP);

			//set any uniform shader values for the actual model
			(*it)->PreRender();

			//actually render the GameObject
			(*it)->Render();
		}

		if ((*it)->GetRP() & RP_TRANSPARENT)
		{
			// Enable blending for transparency
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Common blend mode
			glDepthMask(GL_FALSE); // Disable writing to the depth buffer

			// Collect transparent objects and sort them back to front
			std::vector<GameObject*> transparentObjects;

			for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ++it) {
				if ((*it)->GetRP() & RP_TRANSPARENT) {
					transparentObjects.push_back(*it);
				}
			}

			// Sort transparent objects based on distance to camera
			std::sort(transparentObjects.begin(), transparentObjects.end(), [this](GameObject* a, GameObject* b) {
				vec3 camPos = m_useCamera->GetPos();
				float distA = glm::length(a->GetPos() - camPos);
				float distB = glm::length(b->GetPos() - camPos);
				return distA > distB; // sort back-to-front
				});

			// Render transparent objects
			for (GameObject* obj : transparentObjects) 
			{
				GLuint SP = obj->GetShaderProg();
				glUseProgram(SP);

				m_useCamera->SetRenderValues(SP);
				SetShaderUniforms(SP);

				obj->PreRender();
				obj->Render();
			}

			// Clean up
			glDepthMask(GL_TRUE);
			glDisable(GL_BLEND);
		}
	}

	//TODO: now do the same for RP_TRANSPARENT here
}

void Scene::SetShaderUniforms(GLuint _shaderprog)
{
	//everything needs to know about all the lights
	for (list<Light*>::iterator it = m_Lights.begin(); it != m_Lights.end(); it++)
	{
		(*it)->SetRenderValues(_shaderprog);
	}

}

void Scene::Load(ifstream& _file)
{
	string dummy;

	//load Cameras
	_file >> dummy >> m_numCameras; _file.ignore(256, '\n');
	cout << "CAMERAS : " << m_numCameras << endl;
	for (int i = 0; i < m_numCameras; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		string type;
		_file >> dummy >> type; _file.ignore(256, '\n');
		Camera* newCam = CameraFactory::makeNewCam(type);
		newCam->Load(_file);

		m_Cameras.push_back(newCam);

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	cout << endl << endl;

	//load Lights
	_file >> dummy >> m_numLights; _file.ignore(256, '\n');
	cout << "LIGHTS : " << m_numLights << endl;
	for (int i = 0; i < m_numLights; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		string type;
		_file >> dummy >> type; _file.ignore(256, '\n');
		Light* newLight = LightFactory::makeNewLight(type);
		newLight->Load(_file);

		m_Lights.push_back(newLight);

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	cout << endl << endl;

	//load Models
	_file >> dummy >> m_numModels; _file.ignore(256, '\n');
	cout << "MODELS : " << m_numModels << endl;
	for (int i = 0; i < m_numModels; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		string type;
		_file >> dummy >> type; _file.ignore(256, '\n');
		Model* newModel = ModelFactory::makeNewModel(type);
		newModel->Load(_file);

		m_Models.push_back(newModel);

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	cout << endl << endl;

	//load Textures
	_file >> dummy >> m_numTextures; _file.ignore(256, '\n');
	cout << "TEXTURES : " << m_numTextures << endl;
	for (int i = 0; i < m_numTextures; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		m_Textures.push_back(new Texture(_file));

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	cout << endl << endl;

	//load Shaders
	_file >> dummy >> m_numShaders; _file.ignore(256, '\n');
	cout << "SHADERS : " << m_numShaders << endl;
	for (int i = 0; i < m_numShaders; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		m_Shaders.push_back(new Shader(_file));

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}

	cout << endl << endl;

	//load GameObjects
	_file >> dummy >> m_numGameObjects; _file.ignore(256, '\n');
	cout << "GAMEOBJECTS : " << m_numGameObjects << endl;
	for (int i = 0; i < m_numGameObjects; i++)
	{
		//skip {
		_file.ignore(256, '\n');
		cout << "{\n";

		string type;
		_file >> dummy >> type; _file.ignore(256, '\n');
		GameObject* newGO = GameObjectFactory::makeNewGO(type);
		newGO->Load(_file);

		m_GameObjects.push_back(newGO);

		//skip }
		_file.ignore(256, '\n');
		cout << "}\n";
	}


}

void Scene::Init()
{
	//initialise all cameras
	//scene is passed down here to allow for linking of cameras to game objects
	int count = 0;
	for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); ++it)
	{
		(*it)->Init(100, 100, this);// TODO: set correct screen sizes here

		//if a camera is called MAIN
		//this will be the starting camera used
		if ((*it)->GetName() == "MAIN")
		{
			m_useCamera = (*it);
			m_useCameraIndex = count;
		}
		count++;
	}

	//if no MAIN camera just use the first one
	if (!m_useCamera)
	{
		m_useCamera = (*m_Cameras.begin());
		m_useCameraIndex = 0;
	}

	//set up links between everything and GameObjects
	for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		(*it)->Init(this);
	}
}

string Scene::CurrentCam()
{
	return m_useCamera->GetName();
}

void Scene::ACam()
{
	if (m_useCamera->GetName() != "ACAM")
	{
		for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); ++it)
		{
			if ((*it)->GetName() == "ACAM")
			{
				m_useCamera = (*it);
				break;
			}
		}
	}
	else
	{
		for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); ++it)
		{
			if ((*it)->GetName() == "CAM1")
			{
				m_useCamera = (*it);
				break;
			}
		}
	}
}

void Scene::FCam()
{
	if (m_useCamera->GetName() != "FPSCAM")
	{
		for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); ++it)
		{
			if ((*it)->GetName() == "FPSCAM")
			{
				m_useCamera = (*it);
				break;
			}
		}
	}
	else
	{
		for (list<Camera*>::iterator it = m_Cameras.begin(); it != m_Cameras.end(); ++it)
		{
			if ((*it)->GetName() == "CAM1")
			{
				m_useCamera = (*it);
				break;
			}
		}
	}
}

void Scene::MoveCam(float x, float y)
{
	moveX = x;
	moveY = y;
}

void Scene::StopCamMovement()
{
	moveX = 0;
	moveY = 0;
}

void Scene::ZoomCamInAndOut(float radius)
{
	if (m_useCamera->GetName() == "ACAM")
	{
		cout << radius << endl;
		aCam->scaleRadius(radius);
	}
}


void Scene::SetInput(int userInput)
{
	input = userInput;

	cout << "Input: " << input << endl;
}