#include "pch.h"
#include "GraphicsEngine.h"


GraphicsEngine::GraphicsEngine(CaptureEngine * capEng)
{
	// Set our local pointer so we can access the Capture Engine
	captureEngine = capEng;
}

GraphicsEngine::~GraphicsEngine()
{
	/*
		Removed this code, it was causing an exception error

	// Delete Capture Engine if the pointer is set.
	if (captureEngine != nullptr) {
		delete captureEngine;
	}
	*/
}


void GraphicsEngine::Display()
{
	// Useful for changing arguments with the time
	const double t = (glutGet(GLUT_ELAPSED_TIME) % 10000) / 1000.0;
	const double a = t;

	// Check if key presses changed any variables
	ReadKeyStates();

	// Clear display buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Set the Matrix Mode
	glMatrixMode(GL_MODELVIEW);
	// Starting Identity Matrix
	glLoadIdentity();

	// Position the camera in 3D space.
	gluLookAt(camX, camY, camZ, focusX, focusY, focusZ, 0.0, 1.0, 0.0);

	// Toggle Wireframe
	if (WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME
	
	// Set the background color
	glClearColor(bgRed, bgGreen, bgBlue, 1.0);

	DrawCircle(0.0, 0.0, circleRadius, 1000);

	DrawHosts();

	
	
	/*
		This is a Template for creating objects in 3D space.
		First you must push a new Matrix into the framework,
		then perform any translations/rotation/coloring you desire.
		Finally, put your object into that matrix and pop the matrix.
	
	// Push a new matrix into the framework.
	glPushMatrix();
		// Code for Displaying things goes here...
		glColor3f(0.1, 0.8, 0.65);
		// Translate the object to x,y,z coords
		glTranslatef(TranslateX, TranslateY, TranslateZ);
		// Rotate by angle on x,y,z	
		glRotatef(a * 100, RotateX, RotateY, RotateZ);
		// Scale the object
		glScalef(ScaleX, ScaleY, ScaleZ);
		// Place an Object into the matrix
		// glutSolidIcosahedron();
		glutSolidSphere(2.5, 20, 20);
	// Pop the Matrix off, so that further alterations don't apply to this object.
	glPopMatrix();
	
	*/

	// Swaps the buffer built in this method with the one on screen.
	// Simply put, this renders the frame, MUST BE CALLED LAST in Display()
	glutSwapBuffers();
}

void GraphicsEngine::DrawCircle(float centerX, float centerY, float radius, int segments)
{
	glPointSize(1.5);
	glBegin(GL_POINTS);
	for (int i = 0; i < segments; i++) {
		//get the current angle 
		float theta = 2.0f * 3.1415926f * float(i) / float(segments);
		//calculate the x component
		float x = radius * cosf(theta);
		//calculate the y component 
		float y = radius * sinf(theta);
		// Draw the point vertex
		glVertex3f(centerX + x, centerY + y, 0.0);
	}
	glEnd();
}



void GraphicsEngine::DrawHosts()
{
	int hCount = 0;
	float radius = circleRadius;

	// Lock the thread so we can safely access the List.
	std::unique_lock<std::mutex> uniqueLock(captureEngine->mux, std::defer_lock);
	uniqueLock.lock();
	for (auto& c : connectionList)
	{
		// Handle strange empty iterator behavior
		if (c.GetTotalBytes() == -572662307) { 
			continue; 
		}

		std::string name = "Test";
		
		/// TODO - This initialization keeps crashing
		/// Figure out why the c is sometimes a new Connection.
		// Create Visual Connection
		VisualConnection vCon(name, c);

		int pckCount = vCon.conn.GetPacketCount();

		// Setup the host
		// vCon.SetScale(pckCount / 3, pckCount / 3, pckCount / 3);

		float theta = 2.0f * 3.1415926f * float(hCount) / float(connectionList.size());
		//calculate the x component
		vCon.tX = radius * cosf(theta);
		//calculate the y component 
		vCon.tY = radius * sinf(theta);

		// Draw the host on screen.
		DrawHost(vCon);

		// Increment Host count
		hCount++;
	}
	uniqueLock.unlock();
}

void GraphicsEngine::DrawHost(VisualConnection vCon)
{
	// Push a new matrix into the framework.
	glPushMatrix();
	// Color the object
	glColor3f(vCon.Red, vCon.Green, vCon.Blue);
	// Translate the object to x,y,z coords
	glTranslatef(vCon.tX, vCon.tY, vCon.tZ);
	// Rotate by angle on x,y,z	
	glRotatef(vCon.rA, vCon.rX, vCon.rY, vCon.rZ);
	// Scale the object
	glScalef(vCon.sX, vCon.sY, vCon.sZ);
	// Place an Object into the matrix
	// glutSolidIcosahedron();
	glutSolidSphere(vCon.radius, 20, 20);
	// Pop the Matrix off, so that further alterations don't apply to this object.
	glPopMatrix();

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
	glutPostRedisplay();
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
		// Release the thread
		updater.detach();
		exit(0);
		break;

	case 'w':
		WireFrame = !WireFrame;
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
	case GLUT_KEY_UP:
		*keyUp = false;
		break;
	case GLUT_KEY_DOWN:
		*keyDown = false;
		break;
	case GLUT_KEY_LEFT:
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
	if (*keyA) camY -= 0.035f;
	if (*keyZ) camY += 0.035f;
	if (*keyUp) camZ += 0.05f;
	if (*keyDown) camZ -= 0.05f;
	if (*keyLeft) camX -= 0.005f;
	if (*keyRight) camX += 0.005f;
	glutPostRedisplay();
	
}

void GraphicsEngine::UpdateConnections()
{
	// Keep asking the CaptureEngine for updates on the Connections
	while (true) {
		// Wait half a second
		Sleep(500);
		// Prevent other threads from accessing the connectionList while we update it
		// std::unique_lock<std::mutex> uniqueLock(captureEngine->mux, std::defer_lock);
		// uniqueLock.lock();
		// Get connections from Engine
		connectionList = captureEngine->GetConnections();
		// Unlock
		// uniqueLock.unlock();
		// Transform those connection into Visual Connections
		ProcessConnections();
	}
}

void GraphicsEngine::ProcessConnections()
{
}


void GraphicsEngine::Init()
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

	// Adjusted for suitable viewport
	glOrtho(-2.75, 2.75, 10.5, 0, -1.0, 1.0);

	// Start the Updater thread
	updater = std::thread(&GraphicsEngine::UpdateConnections, this);

}


void GraphicsEngine::StartGLWindow()
{
	// Run me first
	Init();

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
	try {
		glutMainLoop();
	}
	catch (std::exception ex) {
		// Silently Exit
		exit(0);
	}
}




// All these callbacks are designed with specific arguments, to fit as callbacks from the GLUT Framework.
/*
  _____   ____    _   _  ____ _______    _____ _    _          _   _  _____ ______
 |  __ \ / __ \  | \ | |/ __ \__   __|  / ____| |  | |   /\   | \ | |/ ____|  ____|
 | |  | | |  | | |  \| | |  | | | |    | |    | |__| |  /  \  |  \| | |  __| |__
 | |  | | |  | | | . ` | |  | | | |    | |    |  __  | / /\ \ | . ` | | |_ |  __|
 | |__| | |__| | | |\  | |__| | | |    | |____| |  | |/ ____ \| |\  | |__| | |____
 |_____/ \____/  |_| \_|\____/  |_|     \_____|_|  |_/_/    \_\_| \_|\_____|______|
*/
void GraphicsEngine::DisplayCallback() { currentInstance->Display();}
void GraphicsEngine::ResizeCallback(int width, int height){	currentInstance->Resize(width, height);}
void GraphicsEngine::idleCallBack(){currentInstance->Idle();}
void GraphicsEngine::keyDownCallBack(unsigned char key, int x, int y){ currentInstance->KeyDown(key, x, y); }
void GraphicsEngine::KeyUpCallBack(unsigned char key, int x, int y){ currentInstance->KeyUp(key, x, y); }
void GraphicsEngine::SpecialKeyDownCallBack(int key, int x, int y){	currentInstance->SpecialKeyDown(key, x, y); }
void GraphicsEngine::SpecialKeyUpCallBack(int key, int x, int y){ currentInstance->SpecialKeyUp(key, x, y); }
