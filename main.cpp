//#include <iostream>
//#include "TextureManager.h"
//
//#include <allegro5/allegro_acodec.h>
//
//namespace Game
//{
//	TextureManager textureManager;
//}
//
//int main(int argc, char **argv) {
//
//	ALLEGRO_SAMPLE *sample = NULL;
//
//	if (!al_init()) {
//		fprintf(stderr, "failed to initialize allegro!\n");
//		return -1;
//	}
//
//	if (!al_install_audio()) {
//		fprintf(stderr, "failed to initialize audio!\n");
//		return -1;
//	}
//
//	if (!al_init_acodec_addon()) {
//		fprintf(stderr, "failed to initialize audio codecs!\n");
//		return -1;
//	}
//
//	if (!al_reserve_samples(1)) {
//		fprintf(stderr, "failed to reserve samples!\n");
//		return -1;
//	}
//
//	sample = al_load_sample("Data/Sounds/engine.wav");
//
//	if (!sample) {
//		printf("Audio clip sample not loaded!\n");
//		return -1; 
//	}
//
//
//	/* Loop the sample until the display closes. */
//	al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
//
//	al_rest(1.0);
//
//	al_destroy_sample(sample);
//	return 0;
//}

#define NOFULLSCREEN

#include <IL/il.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <iostream>
#include <time.h>

#include "GlobalVariables.h"

#include "enum.h"

#include "Object3D.h"
#include "Car.h"
#include "Orbit.h"
#include "MapManager.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "MapContainer.h"
#include "CarGauge.h"
#include "Screen2D.h"

#include "carDB.h"
#include "KeyboardManager.h"

int current_time;
int previos_time = time(NULL);
int noOfFrames = 0;

int windowWidth = 1500;
int windowHeight = 750;

int windowRealWidth;
int windowRealHeight;

float angle = 55.0f;

class Camera
{
public:
	Camera() { center.x = 15; center.y = -30; center.z = 10;	 }
	
	void adjustCamera(Point icetner, Point ilookAt)
	{
		center = icetner;
		lookAt = ilookAt;
	}

public:
	
	Point center;
	Point lookAt;

	std::vector<std::pair<Point, Point>> cameraViews;

};

Wheel wheel;
Orbit orbit;
CarGauge carGauge;


void display();
void reshape(int, int);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void look(int x, int y);
void timer(int);

void SpecialKeys(int key, int x, int y);
void SpecialKeysUp(int key, int x, int y);
void Update();

void Cube(float a, float b, float c, float d, float h);

Camera camera;

GLboolean upPressed = false;
GLboolean downPressed = false;
GLboolean leftPressed = false;
GLboolean rightPressed = false;
GLboolean F1Pressed = false;
GLboolean F2Pressed = false;
GLboolean F3Pressed = false;
GLboolean F1Released = true;
GLboolean F2Released = true;
GLboolean F3Released = true;

GLboolean leftMouseButtonDown = false;
GLboolean leftMouseButtonDownPrevious = false;
GLboolean leftMouseButtonClicked = false;
GLboolean rightMouseButtonDown = false;
GLboolean scrollMouseButtonDown = false;
GLboolean scrollUpMouse = false;
GLboolean scrollDownMouse = false;

int mouseXPos;
int mouseYPos;

void init()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
}

int main(int argc, char**agrv)
{
	glutInit(&argc, agrv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowPosition(10, 10);
	glutInitWindowSize(windowWidth, windowHeight);
	
	glutCreateWindow("3d game");

#ifdef FULLSCREEN  
	windowRealWidth = glutGet(GLUT_SCREEN_WIDTH);
	windowRealHeight = glutGet(GLUT_SCREEN_HEIGHT);
#else
	windowRealWidth = windowWidth;
	windowRealHeight = windowHeight;
#endif

	Screen2D::Init(windowRealWidth, windowRealHeight);
	SoundManager::Init();
	TextureManager::Init();
	MapContainer::Init(windowRealWidth, windowRealHeight);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyboard);
	glutSpecialFunc(SpecialKeys);
	glutSpecialUpFunc(SpecialKeysUp);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(look);
	glutMotionFunc(look);
	//glutIdleFunc(Update);
	glutTimerFunc(0, timer, 0);

	init();
	ilInit();


	try
	{
		//obj.loadModel();
		wheel.loadModel();

		TextureManager::Instance()->readTextures();
		SoundManager::Instance()->readSounds();

		MapContainer::Instance()->cars = { Car(CarBrand::ToyotaHilux, 0, 0, &camera.center, &camera.lookAt, true), Car(CarBrand::SuzukiVitara, -5, -5, &camera.center, &camera.lookAt), Car(CarBrand::SubaruBRZ, -10, -10, &camera.center, &camera.lookAt), Car(CarBrand::RollsRoycePhantom, -15, -15, &camera.center, &camera.lookAt), Car(CarBrand::LamborghiniHuracan, -20, -20, &camera.center, &camera.lookAt) };
		MapContainer::Instance()->initRaceTimer();

		carGauge.load(&MapContainer::Instance()->cars[1]);
		carGauge.setScreenResolution(windowRealWidth, windowRealHeight);

		MapManager::Init();

		
		MapContainer::loadWorldIntoSections(MapManager::Instance()->mapObjects);

		camera.cameraViews.push_back({ orbit.getCameraCenter(), orbit.getCameraLookAt() });
		camera.cameraViews.push_back({ MapContainer::Instance()->cars[0].getCameraCenter(), MapContainer::Instance()->cars[0].getCameraLookAt() });
		camera.cameraViews.push_back({ MapContainer::Instance()->cars[1].getCameraCenter(), MapContainer::Instance()->cars[1].getCameraLookAt() });
		

	#ifdef FULLSCREEN  
		glutFullScreen();
	#endif

		glutMainLoop();
	}
	catch (Exceptions exc)
	{
		std::string error = std::to_string(static_cast<long>(exc));
	}
	catch (...)
	{

	}


	return 0;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();

	//camera.adjustCamera(obj.getCameraCenter(), obj.getCameraLookAt());
	//camera.adjustCamera(orbit.getCameraCenter(), orbit.getCameraLookAt());

	camera.cameraViews[0] = { orbit.getCameraCenter(), orbit.getCameraLookAt() };
	camera.cameraViews[1] = { MapContainer::Instance()->cars[0].getCameraCenter(), MapContainer::Instance()->cars[0].getCameraLookAt() };
	camera.cameraViews[2] = { MapContainer::Instance()->cars[1].getCameraCenter(), MapContainer::Instance()->cars[1].getCameraLookAt() };
	


	camera.adjustCamera(camera.cameraViews[MapManager::Instance()->currentCameraView].first, camera.cameraViews[MapManager::Instance()->currentCameraView].second);

	glPushMatrix();
	gluLookAt(camera.center.x, camera.center.y, camera.center.z, //eye
		camera.lookAt.x, camera.lookAt.y, camera.lookAt.z, //center
		0, 0, 1); //up
	SoundManager::Instance()->setCameraPosition(camera.center, camera.lookAt);

	if (MapManager::Instance()->currentCameraView == 0)
		MapContainer::displaySector(orbit.getFlatCursor());
	else
		MapContainer::displayWorld(camera.cameraViews[MapManager::Instance()->currentCameraView]);
	//mapContainer.displayAllWorld();

	for (auto& car : MapContainer::Instance()->cars)
	{
		car.display();
		car.alreadyPrinted = false;
		//if (F1Pressed)
			//car.setObstacle(orbit.getFlatCursorX(), orbit.getFlatCursorY());
		if (F2Pressed)
			car.setObstacleVelocity(orbit.getFlatCursorX(), orbit.getFlatCursorY());
		if (F3Pressed)
			car.stop();
	}

	MapContainer::Instance()->displayRaceTimer();
		
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(orbit.getFlatCursorX() - 5, orbit.getFlatCursorY(), 0);
	glVertex3f(orbit.getFlatCursorX() + 5, orbit.getFlatCursorY(), 0);
	glVertex3f(orbit.getFlatCursorX(), orbit.getFlatCursorY() - 5, 0);
	glVertex3f(orbit.getFlatCursorX(), orbit.getFlatCursorY() + 5, 0);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glVertex3f(orbit.getLookAtX() - 1, orbit.getLookAtY(), 0);
	glVertex3f(orbit.getLookAtX() + 1, orbit.getLookAtY(), 0);
	glVertex3f(orbit.getLookAtX(), orbit.getLookAtY() - 1, 0);
	glVertex3f(orbit.getLookAtX(), orbit.getLookAtY() + 1, 0);
	glEnd();

	glPopMatrix();

	Screen2D::pushScreenCoordinateMatrix();
	carGauge.display();
	MapContainer::displayMapEditorPanel();
	MapContainer::displayCounter();
	Screen2D::Instance()->display();
	Screen2D::pop_projection_matrix();

	glutSwapBuffers();

}

void reshape(int width, int height)
{
	glViewport(0, 0, windowRealWidth, windowRealHeight);
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(angle, static_cast<float>(windowRealWidth) / windowRealHeight, 0.5, 10000);
	glMatrixMode(GL_MODELVIEW);

	display();
}

void timer(int)
{
	/*std::cout << mouseXPos << " " << mouseYPos << " "
		<< (leftMouseButtonDown == true ? "x " : "  ")
		<< (rightMouseButtonDown == true ? "x " : "  ")
		<< (scrollMouseButtonDown == true ? "x " : "  ")
		<< (scrollUpMouse == true ? "x " : "  ")
		<< (scrollDownMouse == true ? "x " : "  ")
		<< std::endl;*/

	glutPostRedisplay();

	//glutPostRedisplay();
	//Update();
	glutTimerFunc(1000 / FPS, timer, 0);

	Update();
	noOfFrames++;
	current_time = time(NULL);
	if (current_time - previos_time > 0)
	{
		int currentFPS = noOfFrames / (current_time - previos_time);
		glutSetWindowTitle((std::string("3d game FPS: ") + std::to_string(currentFPS)).c_str());
		previos_time = time(NULL);
		noOfFrames = 0;
	}
	//glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: // Escape key
		orbit.savePosition();
		if(!MapManager::Instance()->loadedFromPolygonsFile)
			MapManager::Instance()->saveOverlays();

		SoundManager::DeInit();
		TextureManager::DeInit();
		exit(0);
		break;
	}

	MapManager::Instance()->applyMapEditorKeys(orbit);

	//reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		leftMouseButtonDownPrevious = leftMouseButtonDown;
		leftMouseButtonDown = (state == GLUT_DOWN);
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		rightMouseButtonDown = (state == GLUT_DOWN);
	}
	else if (button == GLUT_MIDDLE_BUTTON)
	{
		scrollMouseButtonDown = (state == GLUT_DOWN);
	}
	else if (button == 3)
	{
		scrollUpMouse = true;
	}
	else if (button == 4)
	{
		scrollDownMouse = true;
	}
}

void look(int x, int y)
{
	mouseXPos = x;
	mouseYPos = y;
}

void SpecialKeys(int key, int x, int y)
{

	if (GLUT_KEY_UP == key)
		upPressed = true;
	if (GLUT_KEY_DOWN == key)
		downPressed = true;
	if (GLUT_KEY_LEFT == key)
		leftPressed = true;
	if (GLUT_KEY_RIGHT == key)
		rightPressed = true;
	if (GLUT_KEY_F1 == key && F1Released)
		F1Pressed = true;
	if (GLUT_KEY_F2 == key && F2Released)
		F2Pressed = true;
	if (GLUT_KEY_F3 == key && F3Released)
		F3Pressed = true;

}

void SpecialKeysUp(int key, int x, int y)
{
	if (GLUT_KEY_UP == key)
		upPressed = false;
	if (GLUT_KEY_DOWN == key)
		downPressed = false;
	if (GLUT_KEY_LEFT == key)
		leftPressed = false;
	if (GLUT_KEY_RIGHT == key)
		rightPressed = false;
	if (GLUT_KEY_F1 == key)
	{
		F1Pressed = false;
		F1Released = true;
	}
	if (GLUT_KEY_F2 == key)
	{
		F2Pressed = false;
		F2Released = true;
	}
	if (GLUT_KEY_F3 == key)
	{
		F3Pressed = false;
		F3Released = true;
	}

}

void Update()
{
	if (!leftMouseButtonDown && leftMouseButtonDownPrevious)
	{
		leftMouseButtonDownPrevious = false;
		leftMouseButtonClicked = true;
	}
	else
		leftMouseButtonClicked = false;

	KeyboardManager::Instance()->getKeys();

	if (upPressed)
		MapContainer::Instance()->cars[0].accelerate();
	if (downPressed)
		MapContainer::Instance()->cars[0].slow();
	if (leftPressed)
		MapContainer::Instance()->cars[0].turnLeft();
	if (rightPressed)
		MapContainer::Instance()->cars[0].turnRight();
	if (KeyboardManager::Instance()->checkKey(' '))
		MapContainer::Instance()->cars[0].breakPressed();
	if (F1Pressed && F1Released)
	{
		MapContainer::Instance()->cars[0].gearUp();
		F1Released = false;
	}
	if (F2Pressed && F2Released)
	{
		MapContainer::Instance()->cars[0].gearDown();
		F2Released = false;
	}
	if (F3Pressed && F3Released)
	{
		F3Released = false;
	}

	for(auto& car : MapContainer::Instance()->cars)
		car.move();

	MapContainer::Instance()->updateRaceTimer();

	//reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	if (leftMouseButtonDown && rightMouseButtonDown)
	{
		orbit.rotate(-1);
	}
	else if (scrollUpMouse)
	{
		orbit.zoomIn();
	}
	else if (scrollDownMouse)
	{
		orbit.zoomOut();
	}
	else if (rightMouseButtonDown)
	{
		orbit.changeAlpha();
		if(F1Pressed)
			MapContainer::Instance()->removePoints();
	}
	else if (scrollMouseButtonDown)
	{
		orbit.rotate();
	}
	if (F2Pressed && F3Pressed)
	{
		MapContainer::Instance()->AIStop();
	}

	else if (leftMouseButtonDown)
	{
		if (F1Pressed)
			orbit.activateMovingXY();
	}

	if (!F1Pressed && leftMouseButtonClicked)
	{
		if (1.0 - static_cast<float>(mouseYPos) / windowRealHeight > 0.9)
			MapContainer::Instance()->pickTool((static_cast<float>(mouseXPos) / windowRealWidth - 0.5) * windowRealWidth / windowRealHeight, 1.0 - static_cast<float>(mouseYPos) / windowRealHeight);
		else
			MapContainer::Instance()->useTool(orbit.getFlatCursor());
	}

	orbit.calculateFlatCursorPosition(windowRealWidth, windowRealHeight, mouseXPos, mouseYPos, angle);


	scrollUpMouse = false;
	scrollDownMouse = false;
	orbit.deactivateMovingXY();
	//glutPostRedisplay();
}

void Cube(float a, float b, float c, float d, float h)
{
	glColor3f(0, 0.5, 0);

	glBegin(GL_QUADS);

	glVertex3f(c, d, 0);
	glVertex3f(a, d, 0);
	glVertex3f(a, d, h);
	glVertex3f(c, d, h);

	glEnd();

	glColor3f(0.5, 0, 0);

	glBegin(GL_QUADS);

	glVertex3f(a, b, 0);
	glVertex3f(a, d, 0);
	glVertex3f(a, d, h);
	glVertex3f(a, b, h);

	glEnd();

	glColor3f(0.5, 0, 0.5);

	glBegin(GL_QUADS);

	glVertex3f(c, b, 0);
	glVertex3f(c, d, 0);
	glVertex3f(c, d, h);
	glVertex3f(c, b, h);

	glEnd();

	glColor3f(0.5, 0.5, 0);

	glBegin(GL_QUADS);

	glVertex3f(a, b, 0);
	glVertex3f(c, b, 0);
	glVertex3f(c, b, h);
	glVertex3f(a, b, h);

	glEnd();

	glColor3f(0, 0.6, 0.6);

	glBegin(GL_QUADS);

	glVertex3f(a, b, h);
	glVertex3f(c, b, h);
	glVertex3f(c, d, h);
	glVertex3f(a, d, h);

	glEnd();


}

