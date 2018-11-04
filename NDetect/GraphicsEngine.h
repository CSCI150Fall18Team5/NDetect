#pragma once
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
	bool WireFrame = false;

	// Lighting and Materials
	const GLfloat light_ambient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	const GLfloat light_diffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat light_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat light_position[4] = { 2.0f, 5.0f, 5.0f, 0.0f };

	const GLfloat mat_ambient[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
	const GLfloat mat_diffuse[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
	const GLfloat mat_specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat high_shininess[1] = { 100.0f };

public:
	GraphicsEngine();
	~GraphicsEngine();

	// Display function draws all elements.
	void Display();

	// Called when the window resizes.
	void Resize(int width, int height);

	void Idle();

	void KeyDown(unsigned char key, int x, int y);

	void KeyUp(unsigned char key, int x, int y);

	void SpecialKeyDown(int key, int x, int y);

	void SpecialKeyUp(int key, int x, int y);

	void ReadKeyStates();


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
