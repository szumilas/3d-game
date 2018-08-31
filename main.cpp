#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <vector>

class Camera
{
public:
	Camera() { x = 15; y = -40; z = 10;	rx = 0;	ry = 0; rz = 0; }

	void moveX(float dx) { x += dx; }
	void moveY(float dy) { y += dy; }
	void moveZ(float dz) { z += dz; }

	void rotateX(float drx) { rx += drx; }
	void rotateY(float dry) { ry += dry; }
	void rotateZ(float drz) { rz += drz; }

public:
	float x;
	float y;
	float z;

	float rx;
	float ry;
	float rz;

};

class Car
{
public:

	Car() { X = 15; Y = -15; Z = 0; a = 0; v = 0; };

	void print()
	{
		glColor3f(0.8, 0.1, 0.1);

		glBegin(GL_POLYGON);

		glVertex3f(X, Y + 4, Z);
		glVertex3f(X - 1, Y, Z);
		glVertex3f(X + 1, Y, Z);

		glEnd();

		glColor3f(0.6, 0.1, 0.1);

		glBegin(GL_POLYGON);

		glVertex3f(X - 1, Y, Z);
		glVertex3f(X + 1, Y, Z);
		glVertex3f(X + 1, Y, Z + 0.5);
		glVertex3f(X - 1, Y, Z + 0.5);

		glEnd();

		glColor3f(0.8, 0.1, 0.1);

		glBegin(GL_POLYGON);

		glVertex3f(X, Y + 4, Z + 0.5);
		glVertex3f(X - 1, Y, Z + 0.5);
		glVertex3f(X + 1, Y, Z + 0.5);

		glEnd();

	}
	void move()
	{
		Y += v * 0.1;
	}

	void accelerate()
	{
		v += 0.01;
		if (v > v_max)
			v = v_max;
	}

	void slow()
	{
		v -= 0.01;
		if (v < -v_max)
			v = -v_max;
	}

	float X;
	float Y;
	float Z;

	float a;
	float v;

	float v_max = 0.25;


};

void display();
void reshape(int, int);
void keyboard(unsigned char key, int x, int y);

void SpecialKeys(int key, int x, int y);
void SpecialKeysUp(int key, int x, int y);
void Update();

void Cube(float a, float b, float c, float d, float h);

Camera camera;
Car car;

GLboolean upPressed = false;
GLboolean downPressed = false;
GLboolean leftPressed = false;
GLboolean rightPressed = false;

void init()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

int main(int argc, char**agrv)
{
	glutInit(&argc, agrv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

	glutInitWindowPosition(10, 10);
	glutInitWindowSize(1500, 750);

	glutCreateWindow("3d game");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyboard);
	glutSpecialFunc(SpecialKeys);
	glutSpecialUpFunc(SpecialKeysUp);
	glutIdleFunc(Update);

	init();

	glutMainLoop();

	return 0;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//glTranslatef(camera.x, camera.y, camera.z);
	//glRotated(camera.rx, camera.x, 1.0, camera.z);

	gluLookAt(camera.x, camera.y, camera.z, //eye
		camera.x, camera.y + 1, camera.z, //center
		0, 0, 1); //up

	for (int X = -30; X < 30; X++)
	{
		for (int Y = -30; Y < 30; Y++)
		{
			if ((X + Y) % 2 == 0)
			{

				glBegin(GL_POLYGON);
				glColor3f(1, 1, 1);
				glVertex3f(X + 0.0, Y + 0.0, 0.0);
				glVertex3f(X + 1.0, Y + 0.0, 0.0);
				glVertex3f(X + 1.0, Y + 1.0, 0.0);
				glVertex3f(X + 0.0, Y + 1.0, 0.0);

				glEnd();
			}


		}
	}

	Cube(5, 5, 6, 6, 3);
	Cube(15, 8, 18, 12, 8);
	Cube(-25, -15, -16, -6, 4);
	Cube(0, 0, 1, 1, 8);
	Cube(-5, 5, -6, 6, 4);

	car.print();

	glBegin(GL_LINES);

	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 50.0, 0.0);

	glEnd();

	glBegin(GL_LINES);

	glVertex3f(15.0, 0.0, 0.0);
	glVertex3f(15.0, 50.0, 0.0);

	glEnd();



	glutSwapBuffers();

}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(60, 1.0, 0, 150);
	glMatrixMode(GL_MODELVIEW);

	display();
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: // Escape key
		
		exit(0);
		break;
	}


	std::vector<bool> keys(100);
	for (int q = 0; q < keys.size(); q++)
		keys[q] = GetAsyncKeyState(q);



	if (keys[65])
		camera.rotateY(-0.02);

	if (keys[87])
		camera.rotateX(-0.02);

	if (keys[68])
		camera.rotateY(0.02);

	if (keys[83])
		camera.rotateX(0.02);

	//translate
	if (keys[70]) //F
		camera.moveX(0.1);

	if (keys[72]) //H
		camera.moveX(-0.1);

	if (keys[84]) //T
		camera.moveZ(0.1);

	if (keys[71]) //G
		camera.moveZ(-0.1);

	if (keys[82]) //R
		camera.moveY(0.1);

	if (keys[89]) //Y
		camera.moveY(-0.1);

	reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
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

}

void Update()
{
	/*
	if (upPressed)
		camera.moveZ(0.01);
	if (downPressed)
		camera.moveZ(-0.01);
	if (leftPressed)
		camera.moveX(0.01);
	if (rightPressed)
		camera.moveX(-0.01);
	*/
	if (upPressed)
		car.accelerate();
	if (downPressed)
		car.slow();

	car.move();

	reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void Cube(float a, float b, float c, float d, float h)
{
	glColor3f(0, 0.5, 0);

	glBegin(GL_POLYGON);

	glVertex3f(c, 0.0, d);
	glVertex3f(a, 0.0, d);
	glVertex3f(a, h, d);
	glVertex3f(c, h, d);

	glEnd();

	glColor3f(0.5, 0, 0);

	glBegin(GL_POLYGON);

	glVertex3f(a, 0.0, b);
	glVertex3f(a, 0.0, d);
	glVertex3f(a, h, d);
	glVertex3f(a, h, b);

	glEnd();

	glColor3f(0.5, 0, 0.5);

	glBegin(GL_POLYGON);

	glVertex3f(c, 0.0, b);
	glVertex3f(c, 0.0, d);
	glVertex3f(c, h, d);
	glVertex3f(c, h, b);

	glEnd();

	glColor3f(0.5, 0.5, 0);

	glBegin(GL_POLYGON);

	glVertex3f(a, 0.0, b);
	glVertex3f(c, 0.0, b);
	glVertex3f(c, h, b);
	glVertex3f(a, h, b);

	glEnd();


}

