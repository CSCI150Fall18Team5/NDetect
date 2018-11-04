#include "pch.h"
#include "GraphicsEngine.h"


GraphicsEngine::GraphicsEngine()
{
}


GraphicsEngine::~GraphicsEngine()
{
}


void GraphicsEngine::Display()
{
	// Useful for 
	const double t = (glutGet(GLUT_ELAPSED_TIME) % 10000) / 1000.0;
	// const double a = t*90.0;

	// Clear display buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Position the camera in 3D space.
	gluLookAt(0, 0, 10, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// Toggle Wireframe
	if (WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME


	// Set the background color
	glClearColor(2.25, 0.5, 5.45, 0);

	// Push a new matrix into the framework.
	

	glPushMatrix();
		// Code for Displaying things goes here...
		glTranslatef(0.0, 0.0, -15.0);
		glutSolidSphere(8.5, 50, 50);
		glutSolidCube(5);
	glPopMatrix();


	glutSwapBuffers();

}

void GraphicsEngine::Resize(int width, int height)
{
	double Ratio;

	Wwidth = (float)width;
	Wheight = (float)height;

	Ratio = (double)width / (double)height;

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, Ratio, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GraphicsEngine::Idle()
{
}

void GraphicsEngine::KeyDown(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		*keyA = true;
		break;
	case 'z':
		*keyZ = true;
		break;
	case 27:
	case 'q':
		exit(0);
		break;

	case 'w':
		// WireFrame = !WireFrame;
		break;
	}
	ReadKeyStates();
	glutPostRedisplay();
}

void GraphicsEngine::KeyUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		*keyA = false;
		break;
	case 'z':
		*keyZ = false;
		break;
	case 27:
	case 'q':
		exit(0);
		break;

	case 'w':
		break;
	}

	ReadKeyStates();
	glutPostRedisplay();
}

void GraphicsEngine::SpecialKeyDown(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		*keyUp = true;
		break;
	case GLUT_KEY_DOWN:
		*keyDown = true;
		break;
	case GLUT_KEY_LEFT:
		*keyLeft = true;
		break;
	case GLUT_KEY_RIGHT:
		*keyRight = true;
		break;
	}
	ReadKeyStates();
	glutPostRedisplay();
}

void GraphicsEngine::SpecialKeyUp(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:					/* Clockwise rotation over X */
		*keyUp = false;
		break;
	case GLUT_KEY_DOWN:					/* Counter Clockwise rotation over X */
		*keyDown = false;
		break;
	case GLUT_KEY_LEFT:					/* Clockwise rotation over Y */
		*keyLeft = false;
		break;
	case GLUT_KEY_RIGHT:
		*keyRight = false;
		break;
	}
	ReadKeyStates();
	glutPostRedisplay();
}

void GraphicsEngine::ReadKeyStates()
{
	/*
		if (*keyA) translate_z += (float) 0.025f;
		if (*keyZ) translate_z -= (float) 0.025f;
		if (*keyUp) RotateX = (RotateX + 0.05f);
		if (*keyDown) RotateX = (RotateX - 0.05);
		if (*keyLeft) RotateY = (RotateY + 0.05);
		if (*keyRight) RotateY = (RotateY - 0.05);
		glutPostRedisplay();
	*/
}


void GraphicsEngine::DisplayCallback()
{
	currentInstance->Display();
}


void GraphicsEngine::ResizeCallback(int width, int height)
{
	currentInstance->Resize(width, height);
}

void GraphicsEngine::idleCallBack()
{
	currentInstance->Idle();
}

void GraphicsEngine::keyDownCallBack(unsigned char key, int x, int y)
{
	currentInstance->KeyDown(key, x, y);
}

void GraphicsEngine::KeyUpCallBack(unsigned char key, int x, int y)
{
	currentInstance->KeyUp(key, x, y);
}

void GraphicsEngine::SpecialKeyDownCallBack(int key, int x, int y)
{
	currentInstance->SpecialKeyDown(key, x, y);
}

void GraphicsEngine::SpecialKeyUpCallBack(int key, int x, int y)
{
	currentInstance->SpecialKeyUp(key, x, y);
}


void GraphicsEngine::StartGLWindow()
{
	// Set the static instance equal to this instance.
	currentInstance = this;

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 800);                //window screen
	glutInitWindowPosition(100, 100);            //window position
	glutCreateWindow("NDetect - Network Detective");              //program title
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Standard settings
	// glEnable(GL_CULL_FACE);
	// glCullFace(GL_BACK);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);

	// Standard lighting
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	// Material Configuation
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

	// Enable Lights, Normals, and texture
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	// Output OpenGL Version
	std::cout << "|==> OpenGL version:" << glGetString(GL_VERSION) << std::endl;
	
	// adjusted for suitable viewport
	glOrtho(-2.75, 2.75, 10.5, 0, -1.0, 1.0);	
	//callback function for display
	glutDisplayFunc(DisplayCallback);
	//callback for reshape
	glutReshapeFunc(ResizeCallback);
	// Callback function for handling key presses in the Graphics window
	glutKeyboardFunc(keyDownCallBack);						
	// Key Up Callback
	glutKeyboardUpFunc(KeyUpCallBack);
	// Handle Special Keys pressed
	glutSpecialFunc(SpecialKeyDownCallBack);
	// Handle Special Keys released
	glutSpecialUpFunc(SpecialKeyUpCallBack);
	// Function called while the Graphics Framework is idle
	glutIdleFunc(idleCallBack);
	// Opens the Graphics window.
	// Warning, this function DOES NOT RETURN!
	// Once called, we have no control of the codebase besides callbacks set above.
	glutMainLoop();
}
