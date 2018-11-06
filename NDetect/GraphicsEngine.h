#pragma once

#include "DTOs.h"
#include "CaptureEngine.h"


class GraphicsEngine
{
	// Window Size
	float Wwidth, Wheight;

	// Key Press Array
	bool keysPressed[6];

	bool * keyUp = &keysPressed[0];
	bool * keyDown = &keysPressed[1];
	bool * keyLeft = &keysPressed[2];
	bool * keyRight = &keysPressed[3];
	bool * keyA = &keysPressed[4];
	bool * keyZ = &keysPressed[5];

	// Wireframe Toggle
	bool WireFrame = true;

	// Lighting and Materials
	const GLfloat light_ambient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	const GLfloat light_diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat light_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat light_position[4] = { 2.0f, 5.0f, 5.0f, 0.0f };

	const GLfloat mat_ambient[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
	const GLfloat mat_diffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
	const GLfloat mat_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat high_shininess[1] = { 100.0f };

	// Background colors
	float bgRed = 0.4, bgGreen = 0.12, bgBlue = 0.5, bgAlpha = 0.0;

	// Camera Position and Direction
	float camX = 0.0, camY = -15.0, camZ = 5.0;
	float focusX = 0.0, focusY = 0.0, focusZ = 0.0;

	// Translation
	float TranslateX = 0.0, TranslateY = 0.0, TranslateZ = 0.0;

	// Rotation
	float RotateX = 22, RotateY = 140.6, RotateZ = 0;

	// Scale
	float ScaleX = 2.0f, ScaleY = 1.0f, ScaleZ = 2.0f;

	// Pointer to the CaptureEngine
	CaptureEngine * captureEngine;

	// Connections obtained from the CaptureEngine
	std::list<Connection> connectionList;

	// Grabs updated list of Connections
	std::thread updater;

	////////////////////////
	// Graphics Constants

	float circleRadius = 5.0;



public:
	GraphicsEngine(CaptureEngine * capEng);
	~GraphicsEngine();

	// Display function draws all elements.
	void Display();

	void DrawCircle(float centerX, float centerY, float radius, int segments);

	void DrawHosts();

	void DrawHost(VisualConnection vCon);

	// Called when the window resizes.
	void Resize(int width, int height);

	// Runs when the framework isn't rendering a frame
	void Idle();

	// Initialization of the Framework
	void Init();

	// Key Handler functions
	void KeyDown(unsigned char key, int x, int y);
	void KeyUp(unsigned char key, int x, int y);
	void SpecialKeyDown(int key, int x, int y);
	void SpecialKeyUp(int key, int x, int y);
	void ReadKeyStates();

	// CaptureEngine Interactions
	void UpdateConnections();

	void ProcessConnections();

	// Starts the GLUT Window loop.
	void StartGLWindow();


	// Callback functions
	static void DisplayCallback();
	static void ResizeCallback(int width, int height);
	static void idleCallBack();
	static void keyDownCallBack(unsigned char key, int x, int y);
	static void KeyUpCallBack(unsigned char key, int x, int y);
	static void SpecialKeyDownCallBack(int key, int x, int y);
	static void SpecialKeyUpCallBack(int key, int x, int y);

};


// Static Instance of this class
static GraphicsEngine* currentInstance;

// These functions need to be separate from the class
// Due to the nature of GLUT as a C framework, we can't use 
// Class member functions, as they contain the "this" pointer in
// all member functions, and thus the function declaration doesn't match.
// See also: https://stackoverflow.com/questions/2420346/c-api-function-callbacks-into-c-member-function-code/2420382#2420382
