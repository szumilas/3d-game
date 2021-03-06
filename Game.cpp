#define NOFULLSCREEN

#include "Game.h"

#include "GameClock.h"
#include "PlayList.h"

int Game::current_time;
int Game::previos_time = time(NULL);
int Game::noOfFrames = 0;

//int Game::windowWidth = 1500;
//int Game::windowHeight = 750;
int Game::windowWidth = 1280;
int Game::windowHeight = 720;

int Game::windowRealWidth;
int Game::windowRealHeight;

float Game::angle = 55.0f;

int Game::mouseXPos;
int Game::mouseYPos;

Menu Game::menu;
Game::State Game::gameState = Game::State::mainMenu;
Game::State Game::previousGameState = Game::State::mainMenu;


Orbit orbit;
CarGauge carGauge;


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



void Game::init()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1f);
}

void Game::displayWorld()
{
	CameraManager::Instance()->adjustCamera(MapManager::Instance()->currentCameraView);

	glPushMatrix();
	gluLookAt(CameraManager::Instance()->center.x, CameraManager::Instance()->center.y, CameraManager::Instance()->center.z, //eye
		CameraManager::Instance()->lookAt.x, CameraManager::Instance()->lookAt.y, CameraManager::Instance()->lookAt.z, //center
		0, 0, 1); //up
	SoundManager::Instance()->setCameraPosition(CameraManager::Instance()->center, CameraManager::Instance()->lookAt);

	if (MapManager::Instance()->currentCameraView == 0)
	{
		MapContainer::displaySector(orbit.getFlatCursor());
		//std::pair<Point, Point> firstCarCamera = { CameraManager::Instance()->cameraViews[1]->getCameraCenter(), CameraManager::Instance()->cameraViews[1]->getCameraLookAt() };
		//MapContainer::displayWorld(firstCarCamera);
	}
	else
		MapContainer::displayWorld(CameraManager::Instance()->getCurrentCameraPoints());
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

	if(MapContainer::Instance()->getCursorVisibility())
		orbit.displayFlatCursor();

	glPopMatrix();
}

void Game::display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glLoadIdentity();


	if (gameState == Game::State::race || gameState == Game::State::freeRide)
	{
		displayWorld();

		MapContainer::Instance()->displayRaceTimer();

		Screen2D::pushScreenCoordinateMatrix();
		//MapContainer::displayMapEditorPanel();
		if (MapContainer::Instance()->timerActive())
		{
			carGauge.display();
			MapContainer::displayCounter();
			Screen2D::Instance()->display();
		}
		Screen2D::pop_projection_matrix();
	}
	else if (gameState == Game::State::mainMenu)
	{
		glPushMatrix();
		gluLookAt(10, 0, 2.68, //eye
			0, 0, 0, //center
			0, 0, 1); //up

		menu.displayBackground();

		menu.display3DscreenForOption();

		glPopMatrix();

		Screen2D::pushScreenCoordinateMatrix();
		menu.display2DscreenForOption();
		menu.displayForeground();
		Screen2D::Instance()->display();
		menu.displayForegroundBeforeText();
		Screen2D::pop_projection_matrix();
	}
	else if (gameState == Game::State::pause)
	{
		displayWorld();

		Screen2D::pushScreenCoordinateMatrix();
		menu.display2DscreenForOption();
		menu.displayForeground();
		Screen2D::Instance()->display();
		menu.displayForegroundBeforeText();
		Screen2D::pop_projection_matrix();
	}
	else if (gameState == Game::State::mapEditor)
	{
		displayWorld();

		Screen2D::pushScreenCoordinateMatrix();
		MapContainer::displayMapEditorPanel();
		Screen2D::pop_projection_matrix();
	}

	glutSwapBuffers();
}

void Game::reshape(int width, int height)
{
	glViewport(0, 0, windowRealWidth, windowRealHeight);
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(angle, static_cast<float>(windowRealWidth) / windowRealHeight, 0.5, 10000);
	glMatrixMode(GL_MODELVIEW);

	display();
}

void Game::timer(int)
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


void Game::keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: // Escape key
		if (gameState == State::mainMenu)
		{
			orbit.savePosition();
			if (!MapManager::Instance()->loadedFromPolygonsFile)
				MapManager::Instance()->saveOverlays();

			SoundManager::DeInit();
			TextureManager::DeInit();
			exit(0);
			menu.enterPreviousLevel();
			SoundManager::Instance()->playSample(Sounds::menu_click);
		}
		else if((gameState == State::race || gameState == State::freeRide) && MapContainer::Instance()->raceActive() || gameState == State::mapEditor)
		{
			menu.menuResponse.menuState = Menu::OK;
			menu.setPause();
			previousGameState = gameState;
			gameState = State::pause;

			PlayList::Instance()->unmute();
		}
		break;
	}

	MapManager::Instance()->applyMapEditorKeys(orbit);

	//reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void Game::mouse(int button, int state, int x, int y)
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

void Game::look(int x, int y)
{
	mouseXPos = x;
	mouseYPos = y;
}

void Game::SpecialKeys(int key, int x, int y)
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

void Game::SpecialKeysUp(int key, int x, int y)
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

void Game::Update()
{
	KeyboardManager::Instance()->getKeys();

	if (!leftMouseButtonDown && leftMouseButtonDownPrevious)
	{
		leftMouseButtonDownPrevious = false;
		leftMouseButtonClicked = true;
	}
	else
		leftMouseButtonClicked = false;

	KeyboardManager::Instance()->getKeys();

	if (gameState == State::race || gameState == State::freeRide)
	{
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
		if (KeyboardManager::Instance()->checkKey('R') && MapContainer::Instance()->raceActive())
			MapContainer::Instance()->cars[0].resetPositionToAIPath();
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

		for (auto& car : MapContainer::Instance()->cars)
			car.update();

		MapContainer::Instance()->updateRaceTimer();
		if (MapContainer::Instance()->raceTimer.state == RaceTimer::State::RaceFinished)
		{
			menu.menuResponse.menuState = Menu::OK;
			gameState = State::mainMenu;
			MapContainer::Instance()->cars.clear();
			menu.loadAfterRaceScreen();

			PlayList::Instance()->unmute();
			return;
		}

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
			if (F1Pressed)
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
		orbit.deactivateMovingLookAt();

		if (MapManager::Instance()->currentCameraView == -1 && !CameraManager::Instance()->updateSpecialCameraPathPosition())
		{
			MapManager::Instance()->currentCameraView = 1;
			MapContainer::Instance()->introFinished();
		}
	}
	else if (gameState == State::mainMenu || gameState == State::pause)
	{
		static clock_t menuSwitchDelay = GameClock::Instance()->getClock();

		if (leftPressed || rightPressed || checkKey(ENTER))
		{
			if (GameClock::Instance()->getClock() - menuSwitchDelay > 300)
			{
				if (leftPressed || rightPressed)
					SoundManager::Instance()->playSample(Sounds::menu_click);
				else
					SoundManager::Instance()->playSample(Sounds::menu_accept);

				if (leftPressed)
					menu.selectPrevious();
				else if (rightPressed)
					menu.selectNext();
				else if (checkKey(ENTER))
					menu.enter();

				menuSwitchDelay = GameClock::Instance()->getClock();
			}
		}
		else
		{
			menuSwitchDelay = 0;
		}

		if (!menu.update())
		{
			handleMenuResponse();
		}
	}
	else if (gameState == State::mapEditor)
	{
		/*
		if (KeyboardManager::Instance()->checkKey('I'))
		{
			int speedLimit;
			std::cin >> speedLimit;
			MapContainer::Instance()->cars[0].setCarLimit(speedLimit);
		}

		if (KeyboardManager::Instance()->checkKey('0'))
		{
			MapContainer::Instance()->hideMapEditorPanel();
		}
		if (KeyboardManager::Instance()->checkKey('9'))
		{
			MapContainer::Instance()->showMapEditorPanel();
		}

		if (KeyboardManager::Instance()->checkKey('1'))
		{
			if(MapContainer::Instance()->trailerScenery % 2 == 0)
				MapContainer::Instance()->trailerScenery++;
		}
		if (KeyboardManager::Instance()->checkKey('2'))
		{
			if (MapContainer::Instance()->trailerScenery % 2 == 1)
				MapContainer::Instance()->trailerScenery++;
		}

		if (KeyboardManager::Instance()->checkKey('3'))
		{
			if (MapContainer::Instance()->trailerScenery % 2 == 0)
				MapContainer::Instance()->trailerScenery--;
		}
		if (KeyboardManager::Instance()->checkKey('4'))
		{
			if (MapContainer::Instance()->trailerScenery % 2 == 1)
				MapContainer::Instance()->trailerScenery--;
		}

		if (KeyboardManager::Instance()->checkKey('5'))
		{
			MapContainer::Instance()->trailerScenery = 0;

			for (auto& object : MapManager::Instance()->mapObjects)
			{
				object->printObject = false;
				object->printTexture = false;
			}

			MapContainer::Instance()->cars[0].prin3DtModel = false;
			MapContainer::Instance()->cars[0].printWheels = false;
			MapContainer::Instance()->linesVisible = true;
		}

		if (KeyboardManager::Instance()->checkKey('6'))
		{
			MapContainer::Instance()->trailerScenery = 100;

			for (auto& object : MapManager::Instance()->mapObjects)
			{
				object->printObject = true;
				object->printTexture = true;
			}

			MapContainer::Instance()->cars[0].prin3DtModel = true;
			MapContainer::Instance()->cars[0].printWheels = true;
			MapContainer::Instance()->linesVisible = false;
		}
		*/

		
		if (KeyboardManager::Instance()->checkKey('Q'))
		{
			MapContainer::Instance()->PlayCameraSpline();
		}
		else if (KeyboardManager::Instance()->checkKey('W'))
		{
			MapContainer::Instance()->PlayCameraSplineAroundCarZero();
		}
		if (KeyboardManager::Instance()->checkKey('E'))
		{
			MapContainer::Instance()->AIPlay();
		}
		if (KeyboardManager::Instance()->checkKey('R'))
		{
			MapContainer::Instance()->ApplySpecialKey();
		}

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
			if (F3Pressed)
				orbit.cameraCenterDown();
			else if(F2Pressed)
				orbit.changeAlpha(-1);
			else
				orbit.changeAlpha();
		}
		else if (scrollMouseButtonDown)
		{
			if(F2Pressed)
				orbit.rotate(-1);
			else
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
			else if (F2Pressed)
				orbit.activateMovingLookAt();
			else if (F3Pressed)
				orbit.cameraCenterUp();
		}

		if (!F1Pressed && !F2Pressed && !F3Pressed && leftMouseButtonClicked)
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
		orbit.deactivateMovingLookAt();

		if (MapManager::Instance()->currentCameraView == -1 && !CameraManager::Instance()->updateSpecialCameraPathPosition())
		{
			MapManager::Instance()->currentCameraView = 0;
			glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
			MapContainer::Instance()->showMapEditorPanel();
			MapContainer::Instance()->showCursors();
			MapContainer::Instance()->introFinished();
		}

		for (auto& car : MapContainer::Instance()->cars)
			car.update();
	}


	//glutPostRedisplay();
	PlayList::Instance()->update();
}

Game::Game(int argc, char**agrv)
{
	srand(time(NULL));

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
	CameraManager::Init(&orbit);
	GameClock::Init();

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
	//glutSetCursor(GLUT_CURSOR_NONE);

	init();
	ilInit();

}

void Game::displayLoadingTexture()
{
	glViewport(0, 0, windowRealWidth, windowRealHeight);
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(angle, static_cast<float>(windowRealWidth) / windowRealHeight, 0.5, 10000);
	glMatrixMode(GL_MODELVIEW);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	gluLookAt(10, 0, 2.68, //eye
		0, 0, 0, //center
		0, 0, 1); //up

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, TextureManager::Instance()->getLoadingTextureId());
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_POLYGON);

	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0, 0);
	glVertex3f(-9.78, -30, -18.15);
	glTexCoord2f(1, 0);
	glVertex3f(-9.78, 30, -18.15);
	glTexCoord2f(1, 1);
	glVertex3f(-17.54, 30, 10.83);
	glTexCoord2f(0, 1);
	glVertex3f(-17.54, -30, 10.83);

	glEnd();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glFlush();

	glutSwapBuffers();
}

void Game::play()
{
#ifdef FULLSCREEN  
	glutFullScreen();
#endif
	glutShowWindow();

	TextureManager::Instance()->readLoadingTexture();

	displayLoadingTexture();

	try
	{
		std::cout << "reading textures...\n";
		TextureManager::Instance()->readTextures();
		std::cout << "reading sounds...\n";
		SoundManager::Instance()->readSounds();
		PlayList::Init();

		std::cout << "init cars...\n";
		MapContainer::Instance()->initCars();

		carGauge.setScreenResolution(windowRealWidth, windowRealHeight);

		std::cout << "init mapmanager...\n";
		MapManager::Init();
		std::cout << "init menu...\n";
		menu.Init(windowRealWidth, windowRealHeight);



		std::cout << "main loop...\n";
		PlayList::Instance()->start();
		glutMainLoop();
	}
	catch (Exceptions exc)
	{
		std::string error = std::to_string(static_cast<long>(exc));
	}
	catch (...)
	{

	}
}

bool Game::checkKey(Key key)
{
	return KeyboardManager::Instance()->checkKey(key);
}

void Game::handleMenuResponse()
{
	if (menu.menuResponse.menuState == Menu::StartQuickRace)
	{
		gameState = State::race;

		auto& r = menu.menuResponse;
		MapContainer::Instance()->raceTimer = RaceTimer();
		MapContainer::Instance()->setRaceDetails(r.selectedCar, r.selectedTrack, r.noOfLaps, r.noOfOponents);
		std::cout << "carGauge load...\n";
		carGauge.load(&MapContainer::Instance()->cars[0]);
		MapContainer::Instance()->activateRaceTimer();
		MapContainer::Instance()->initRaceTimer();

		PlayList::Instance()->mute();
	}
	else if (menu.menuResponse.menuState == Menu::StartFreeRide)
	{
		gameState = State::freeRide;

		auto& r = menu.menuResponse;
		MapContainer::Instance()->raceTimer = RaceTimer();
		MapContainer::Instance()->setFreeRide(r.selectedCar, MapManager::Instance()->ConvertCoordinatesToLocalWorld(r.startPosition));
		std::cout << "carGauge load...\n";
		carGauge.load(&MapContainer::Instance()->cars[0]);
		MapContainer::Instance()->deactivateRaceTimer();
		MapContainer::Instance()->initRaceTimer();

		PlayList::Instance()->mute();
	}
	else if (menu.menuResponse.menuState == Menu::Resume)
	{
		gameState = previousGameState;

		PlayList::Instance()->mute();
	}
	else if (menu.menuResponse.menuState == Menu::ExitToMainMenu)
	{
		menu.menuResponse.menuState = Menu::OK;
		gameState = State::mainMenu;
		MapContainer::Instance()->cars.clear();
		MapContainer::Instance()->raceTimer.setRaceFinished(true);

		PlayList::Instance()->unmute();
	}
	else if (menu.menuResponse.menuState == Menu::StartMapEditor)
	{
		gameState = State::mapEditor;
		MapContainer::Instance()->loadWorldIntoSections(MapManager::Instance()->mapObjects);
		PlayList::Instance()->mute();
		MapContainer::Instance()->raceTimer.beforeRace = false;
	}
	else if (menu.menuResponse.menuState == Menu::ExitGame)
	{
		orbit.savePosition();
		if (!MapManager::Instance()->loadedFromPolygonsFile)
			MapManager::Instance()->saveOverlays();

		SoundManager::DeInit();
		TextureManager::DeInit();
		exit(0);
	}
}