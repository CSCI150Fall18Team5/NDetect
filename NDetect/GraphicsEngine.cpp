#include "pch.h"
#include "GraphicsEngine.h"

GraphicsEngine::GraphicsEngine(CaptureEngine * capEng, ThreadManager * tm)
{
	// Set our local pointer so we can access the Capture Engine
	captureEngine = capEng;

	// Set the ThreadManager
	threadMan = tm;
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

	// Update the rotation of all hosts
	// RotateAngle = (RotateAngle >= 360) ? 0 : RotateAngle + 0.0025;


	// Draw lines between hosts.
	DrawHostLines();
	
	// Place the hosts around
	DrawHosts();
	
	// Draw the circle that the host nodes will encompass.
	DrawCircle(0.0, 0.0, circleRadius, 1080);


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
	glColor3f(circRed, circGreen, circBlue);
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

void GraphicsEngine::DrawFilledCircle(float centerX, float centerY, float radius, int segments)
{
	float Red = 0.0, Blue = 8.5, Green = 1.5;
	int drawMode = 0;

	glPointSize(1.5);
	glBegin(GL_POINTS);
	while(radius > 0)
	{	
		glColor3f(Red, Blue, Green);
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
		// Reduce the radius to fill the circle
		radius -= 0.02;

		if (drawMode == 1) {
			Blue = (Blue <= 0.0) ? 0.0 : Blue - 0.2;
			Green = (Green <= 0.0) ? 0.0 : Green - 0.75;
		}
		else {


		}
		
	}
	glEnd();
}

void GraphicsEngine::DrawHosts()
{
	// Lock the thread so we can safely access the List.
	std::unique_lock<std::mutex> uniqueLock(threadMan->muxVisualConnections);
	for (auto & vc : visualConnections) {
		// Draw the host on screen.
		DrawHost(vc.second);
	}
	uniqueLock.unlock();	
}

void GraphicsEngine::DrawHost(VisualConnection vCon)
{
	// Decide which way to render
	if (Render3D) {


		// DrawString(vCon.tX - 0.40, vCon.tY - 0.325, vCon.Red, vCon.Green, vCon.Blue, vCon.SourceIP.c_str());
		// Align the IP Display with the radius of the host
		float textX = (vCon.tX -0.40);
		float textY = (vCon.tY - (vCon.radius * 1.9) * vCon.sX );

		/* Draw white text */
		// DrawString(textX, textY, 1.0, 5, 3.5, vCon.SourceIP.c_str());
		
		// Draw the host IP near the node
		DrawString(textX, textY, vCon.Red * 10, vCon.Green * 10, vCon.Blue * 10, vCon.SourceIP.c_str());

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
		glutSolidSphere(vCon.radius, 20, 10);
		// Pop the Matrix off, so that further alterations don't apply to this object.
		glPopMatrix();

	}
	else {

		DrawString(vCon.tX - 0.40, vCon.tY - 0.325, vCon.Red, vCon.Green, vCon.Blue, vCon.SourceIP.c_str());
		// Align the IP Display with the radius of the host
		float textX = (vCon.tX - vCon.radius) - 0.40;
		float textY = (vCon.tY - vCon.radius) - 0.325;

		// Draw the host IP near the node
		DrawString(textX, textY, 1.0, 5, 3.5, vCon.SourceIP.c_str());

		DrawFilledCircle(vCon.tX, vCon.tY, vCon.radius, 360);
	}
	

}

void GraphicsEngine::DrawHostLines()
{
	// Lock the thread so we can safely access the List.
	std::unique_lock<std::mutex> uniqueLock(threadMan->muxVisualConnections);
	for (auto & vc : visualConnections) {
		
		// Test each destination IP at the source
		for (auto & tDest : vc.second.DestIPs) {

			// Found the IP in our visual connections
			if (visualConnections.find(tDest) != visualConnections.end()) {

				// Draw the host on screen.
				DrawHostLine(vc.second, visualConnections[tDest]);

			}
		}
		
	}
	uniqueLock.unlock();
}

void GraphicsEngine::DrawHostLine(VisualConnection from, VisualConnection to)
{
	
	glLineWidth(from.packetCount / 4);
	glColor3f(0.8, 1.0, 0.8);	
	for (auto & dC: visualConnections) {

		glBegin(GL_LINES);
		glVertex3f(from.tX, from.tY, 0.0);
		glVertex3f(to.tX, to.tY, 0.0);
		glEnd();
	}
	
}

void GraphicsEngine::DrawString(float x, float y, int r, int g, int b, const char * string)
{
	int j = strlen(string);

	// Color the Text
	glColor3f(r, g, b);
	// Set the position
	glRasterPos2f(x, y);
	// Draw the characters
	for (int i = 0; i < j; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
	}

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

	case 'd':
		if (captureEngine->GetConsoleMode() == ConsoleMode::ConnectionsMade) {
			system("cls");
			captureEngine->SetConsoleMode(ConsoleMode::LiveStream);
		}
		else if (captureEngine->GetConsoleMode() == ConsoleMode::LiveStream) {
			captureEngine->SetConsoleMode(ConsoleMode::ConnectionsMade);
		}
		break;

	case 27:
	case 'q':
		threadMan->EndThreads();
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
	while (threadMan->threadsContinue) {
		// Get connections from Engine
		connectionList = captureEngine->GetConnections();
		// Transform those connection into Visual Connections
		ProcessConnections();
		// Wait before trying again
		Sleep(250);
	}
}

void GraphicsEngine::ProcessConnections()
{
	// Build upon a temp list
	std::map <std::string, VisualConnection> tempVConnections;
	
	// Get the host IP for comparing
	std::string hostIP = captureEngine->GetHostIP();

	// Get Counts
	int pckTotal = captureEngine->GetPacketCount();
	int connectionCount = connectionList.size();

	// Localhost present in connections
	bool localHostFound = false;

	// First create temporary Visual Connections from existing connections
	for (auto c : connectionList)
	{
		// Temp strings
		std::string sourceIP = c.GetSourceIP();
		std::string destIP = c.GetDestIP();

		// Add Unique IP from Connection Source, with Destination
		if (tempVConnections.find(sourceIP) == tempVConnections.end()) {
			// Add this Visual Connection
			tempVConnections[sourceIP] = VisualConnection(c);
			// Add the Destination 
			tempVConnections[sourceIP].DestIPs.push_back(destIP);
		}
		// If Destination of this connection is missing, add THAT IP address as well.
		if (tempVConnections.find(destIP) == tempVConnections.end()) {
			// Add this Visual Connection
			tempVConnections[destIP] = VisualConnection(destIP, c.GetDestPort());
			// Add the Destination 
			tempVConnections[destIP].DestIPs.push_back(sourceIP);
		}

		// Subtract one Host node if 
		if (sourceIP == hostIP || destIP == hostIP) {
			localHostFound = true;
		}
	}

	// Used to calculate each connections position on the graph.
	int counter = 0;
	bool localHostPlaced = false;
	// Host Count ( Subtract one because the localhost will be in the center.
	int hCount = tempVConnections.size();

	for (auto & tVC : tempVConnections)
	{
		// Get our Theta 
		float theta = 2.0f * 3.1415926f * float(counter++) / float(hCount);
		
		// If this is the localhost, place the dot in the center.
		if (!localHostPlaced && tVC.second.SourceIP == hostIP) {
			// Set the X,Y, and Z values of the center point
			tVC.second.SetTranslation(0.0, 0.0, 0.0);
			localHostPlaced = true;
			// Decrement host count because the localhost isn't in the circle.
			hCount--;
		}
		else {
			// Set the X,Y, and Z values of the center point
			tVC.second.SetTranslation(circleRadius * cosf(theta), circleRadius * sinf(theta), 0.0);
		}

		// Get Packet Count
		int pckCount = tVC.second.packetCount;

		// Set the scaling
		float Scale = 1.0;
		if (connectionCount > 0) {
			if ((pckCount / (pckTotal / connectionCount)) >= 2.0) {
				Scale = 2.0;
			}
			else {
				Scale = 1.0 + (pckCount / (pckTotal / connectionCount));
			}
		}
		tVC.second.SetScale(Scale, Scale, Scale);

		// Adjust Colors
		tVC.second.Red += tVC.second.totalBytes / 256;
		/*
		tVC.second.Blue -= tVC.second.totalBytes / 256;
		tVC.second.Green -= tVC.second.totalBytes / 256;
		*/

		// Set the rotation
		tVC.second.rA = RotateAngle;
		tVC.second.rX = RotateX;
		tVC.second.rY = RotateY;
		tVC.second.rZ = RotateZ;
	}
	// Lock the thread so we can safely access the List.
	std::unique_lock<std::mutex> uniqueLock(threadMan->muxVisualConnections);

	// Overwrite the connections we have
	visualConnections = tempVConnections;

	// Unlock now that we're done
	uniqueLock.unlock();
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
	// std::cout << "|==> OpenGL version:" << glGetString(GL_VERSION) << std::endl;

	// Adjusted for suitable viewport
	glOrtho(-2.75, 2.75, 10.5, 0, -1.0, 1.0);

	// Set the background color
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// Change the camera view depending on the Render Dimensions
	if (Render3D) {

		camX = 0.0;
		camY = 0.0;
		camZ = 14.0;
	}
	else {

		camX = 0.0;
		camY = 0.0;
		camZ = 14.0;

	}



	// Start the Updater thread
	threadMan->Threads[threadMan->ThreadCount++] = std::thread(&GraphicsEngine::UpdateConnections, this);

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
	glutKeyboardFunc(KeyDownCallBack);
	// Key Up Callback
	glutKeyboardUpFunc(KeyUpCallBack);
	// Handle Special Keys pressed
	glutSpecialFunc(SpecialKeyDownCallBack);
	// Handle Special Keys released
	glutSpecialUpFunc(SpecialKeyUpCallBack);
	// Function called while the Graphics Framework is idle
	glutIdleFunc(IdleCallBack);
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
void GraphicsEngine::IdleCallBack(){currentInstance->Idle();}
void GraphicsEngine::KeyDownCallBack(unsigned char key, int x, int y){ currentInstance->KeyDown(key, x, y); }
void GraphicsEngine::KeyUpCallBack(unsigned char key, int x, int y){ currentInstance->KeyUp(key, x, y); }
void GraphicsEngine::SpecialKeyDownCallBack(int key, int x, int y){	currentInstance->SpecialKeyDown(key, x, y); }
void GraphicsEngine::SpecialKeyUpCallBack(int key, int x, int y){ currentInstance->SpecialKeyUp(key, x, y); }
