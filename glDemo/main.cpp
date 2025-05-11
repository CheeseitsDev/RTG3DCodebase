
#include "core.h"
#include "TextureLoader.h"
#include "ArcballCamera.h"
#include "GUClock.h"
#include "PrincipleAxes.h"
#include "shader_setup.h"
#include "helper.h"
#include "AIMesh.h"
#include "Cube.h"
#include "Scene.h"


using namespace std;
using namespace glm;


#pragma region Global variables

GUClock* g_gameClock = nullptr;

// Mouse tracking
bool				g_mouseDown = false;
double				g_prevMouseX, g_prevMouseY;

// Global Example objects
// shouldn't really be anything in here for the final submission
//ArcballCamera* g_mainCamera = nullptr;
CGPrincipleAxes* g_principleAxes = nullptr;

int g_showing = 2;
int g_NumExamples = 3;

//Global Game Object
Scene* g_Scene = nullptr;

// Window size
const unsigned int g_initWidth = 512;
const unsigned int g_initHeight = 512;

#pragma endregion


// Function prototypes
void renderScene();
void updateScene();
void resizeWindow(GLFWwindow* _window, int _width, int _height);
void keyboardHandler(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);
void mouseMoveHandler(GLFWwindow* _window, double _xpos, double _ypos);
void mouseButtonHandler(GLFWwindow* _window, int _button, int _action, int _mods);
void mouseScrollHandler(GLFWwindow* _window, double _xoffset, double _yoffset);
void mouseEnterHandler(GLFWwindow* _window, int _entered);


int main()
{
	//
	// 1. Initialisation
	//

	g_gameClock = new GUClock();

#pragma region OpenGL and window setup

	// Initialise glfw and setup window
	glfwInit();

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	glfwWindowHint(GLFW_OPENGL_COMPAT_PROFILE, GLFW_TRUE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);

	GLFWwindow* window = glfwCreateWindow(g_initWidth, g_initHeight, "GDV5001", NULL, NULL);

	// Check window was created successfully
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window!\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	// Set callback functions to handle different events
	glfwSetFramebufferSizeCallback(window, resizeWindow); // resize window callback
	glfwSetKeyCallback(window, keyboardHandler); // Keyboard input callback
	glfwSetCursorPosCallback(window, mouseMoveHandler);
	glfwSetMouseButtonCallback(window, mouseButtonHandler);
	glfwSetScrollCallback(window, mouseScrollHandler);
	glfwSetCursorEnterCallback(window, mouseEnterHandler);

	// Initialise glew
	glewInit();


	// Setup window's initial size
	resizeWindow(window, g_initWidth, g_initHeight);

#pragma endregion

	// Initialise scene - geometry and shaders etc
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // setup background colour to be black
	glClearDepth(1.0f);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);

	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//
	// Setup the Example Objects
	//

	//g_mainCamera = new ArcballCamera(0.0f, 0.0f, 1.98595f, 55.0f, 1.0f, 0.1f, 500.0f);

	g_principleAxes = new CGPrincipleAxes();

	g_Scene = new Scene();

	ifstream manifest;
	manifest.open("manifest.txt");

	g_Scene->Load(manifest);
	g_Scene->Init();

	manifest.close();


	//
	// Main loop
	// 

	while (!glfwWindowShouldClose(window))
	{
		updateScene();
		renderScene();						// Render into the current buffer
		glfwSwapBuffers(window);			// Displays what was just rendered (using double buffering).

		glfwPollEvents();					// Use this version when animating as fast as possible

		// update window title
		char timingString[256];
		sprintf_s(timingString, 256, "CIS5013: Average fps: %.0f; Average spf: %f", g_gameClock->averageFPS(), g_gameClock->averageSPF() / 1000.0f);
		glfwSetWindowTitle(window, timingString);
	}

	glfwTerminate();

	if (g_gameClock)
	{
		g_gameClock->stop();
		g_gameClock->reportTimingData();
	}

	return 0;
}


// renderScene - function to render the current scene
void renderScene()
{
	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//mat4 cameraTransform = g_mainCamera->projectionTransform() * g_mainCamera->viewTransform();

	//mat4 cameraProjection = g_mainCamera->projectionTransform();

	switch (g_showing)
	{
	case 0:
	{
		g_Scene->Render();
	}
	break;

	case 1:
	{
		g_Scene->Render();
	}
	case 2:
		g_Scene->Render();
	}

}


// Function called to animate elements in the scene
void updateScene() 
{
	float tDelta = 0.0f;

	if (g_gameClock) {

		g_gameClock->tick();
		tDelta = (float)g_gameClock->gameTimeDelta();
	}

	g_Scene->Update(tDelta);
}


#pragma region Event handler functions
//none of this is currently passed to the Game object
//probably a good idea to do that

// Function to call when window resized
void resizeWindow(GLFWwindow* _window, int _width, int _height)
{
	/*if (g_mainCamera) {

		g_mainCamera->setAspect((float)_width / (float)_height);
	}*/

	glViewport(0, 0, _width, _height);		// Draw into entire window
}


// Function to call to handle keyboard input
void keyboardHandler(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
{
	if (_action == GLFW_PRESS) {

		// check which key was pressed...
		switch (_key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(_window, true);
			break;

		case GLFW_KEY_SPACE:
			g_showing++;
			g_showing = g_showing % g_NumExamples;
			break;
		case GLFW_KEY_BACKSPACE:
			g_Scene->ACam();
			break;
		case GLFW_KEY_F:
			g_Scene->FCam();
			break;
		case GLFW_KEY_W:
			g_Scene->SetInput(1);
			break;
		case GLFW_KEY_A:
			g_Scene->SetInput(2);
			break;
		case GLFW_KEY_S:
			g_Scene->SetInput(3);
			break;
		case GLFW_KEY_D:
			g_Scene->SetInput(4);
			break;
		default:
		{
		}
		}
	}
	else if (_action == GLFW_RELEASE) 
	{
		// handle key release events
		switch (_key)
		{
		case GLFW_KEY_W:
			g_Scene->SetInput(0);
			break;
		case GLFW_KEY_A:
			g_Scene->SetInput(0);
			break;
		case GLFW_KEY_S:
			g_Scene->SetInput(0);
			break;
		case GLFW_KEY_D:
			g_Scene->SetInput(0);
			break;
		default:
		{
		}
		}
	}
}


void mouseMoveHandler(GLFWwindow* _window, double _xpos, double _ypos) 
{
	//float tDelta = gameClock->gameTimeDelta();

	float dx = float(_xpos - g_prevMouseX);// *360.0f * tDelta;
	float dy = float(_ypos - g_prevMouseY);// *360.0f * tDelta;

	/*if (g_mainCamera)
		g_mainCamera->rotateCamera(-dy, -dx);*/

	g_Scene->MoveCam(-dx, -dy);

	g_prevMouseX = _xpos;
	g_prevMouseY = _ypos;
}

void mouseButtonHandler(GLFWwindow* _window, int _button, int _action, int _mods) 
{
	if (_button == GLFW_MOUSE_BUTTON_LEFT) 
	{
		if (_action == GLFW_PRESS) 
		{
			g_mouseDown = true;
			glfwGetCursorPos(_window, &g_prevMouseX, &g_prevMouseY);
		}
		else if (_action == GLFW_RELEASE) 
		{
			g_mouseDown = false;

			g_Scene->StopCamMovement();
		}
	}
}

void mouseScrollHandler(GLFWwindow* _window, double _xoffset, double _yoffset) {

	/*if (g_mainCamera) 
	{
		if (_yoffset < 0.0)
			g_mainCamera->scaleRadius(1.1f);
		else if (_yoffset > 0.0)
			g_mainCamera->scaleRadius(0.9f);
	}*/

	if (_yoffset < 0.0)
	{
		g_Scene->ZoomCamInAndOut(1.1f);
	}
	else if (_yoffset > 0.0)
	{
		g_Scene->ZoomCamInAndOut(0.9f);
	}
}

void mouseEnterHandler(GLFWwindow* _window, int _entered) 
{
}

#pragma endregion