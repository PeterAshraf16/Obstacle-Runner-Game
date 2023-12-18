#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <cstdlib> 
#include <ctime>   
#include <string.h>
#include <iostream>
#include <windows.h>
#include <playsoundapi.h>
#include <cmath>
#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)


int WIDTH = 1280;
int HEIGHT = 720;

GLuint tex;
GLuint tex1;
GLuint playertex;
char title[] = "Course Runner";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 1000;

GLuint tex_rock;

float playerX = 0.0f;
float playerZ = 29.0f;
float angleY = 0.0f;
float moveSpeed = 0.5f;
char direction = 'w';
char view = '1';
float playerY = 0.0f;
int coin1Collected = 0;
int coin2Collected = 0;
int coin3Collected = 0;
int coin4Collected = 0;
int coin1 = 1;
int coin2 = 1;
int coin3 = 1;
int coin4 = 1;
int score = 0;
float jumpDistanceZ = 7.0f;
int lives = 5;
float rockAnimation = 0.0f;
float coin1Animation = 0.0f;
float coin2Animation = 0.0f;
float coin3Animation = 0.0f;
float coin4Animation = 0.0f;
int coin11 = 0;
int coin22 = 0;
int coin33 = 0;
int coin44 = 0;
int star11 = 0;
int star22 = 0;
int star33 = 0;
int star44 = 0;
boolean GoalLevel1 = false;
boolean takingInput = true;
int currentLevel = 1; // Global variable to track the current level
bool GoalLevel2 = false;
int star1 = 1;
int star2 = 1;
int star3 = 1;
int star4 = 1;
// Positions of stars
float star1X, star1Z;
float star2X, star2Z;
float star3X, star3Z;
float star4X, star4Z;

// Flags to track if stars are collected
int star1Collected = 0;
int star2Collected = 0;
int star3Collected = 0;
int star4Collected = 0;

bool gameWon = false;

bool soundPlayed = false;
float star1Animation = 0.0f;
float star2Animation = 0.0f;
float star3Animation = 0.0f;
float star4Animation = 0.0f;

float star1AnimationTime = 0.0f;
float star2AnimationTime = 0.0f;
float star3AnimationTime = 0.0f;
float star4AnimationTime = 0.0f;
float fanRotationAngle = 0.0f;
float fanRotationSpeed = 1.0f; // Adjust as needed
bool fanCollisionDetected = false;
bool goalSoundPlayed = false;

//star1Animation = 30; // Adjust for longer or shorter animation
class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

void initLevel2();

class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 0.0f, float eyeY = 2.0f, float eyeZ = 29.0f, float centerX = 0.0f, float centerY = 2.0f, float centerZ = -100.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
		if (eye.x < 0.1 || eye.x>6.1 || eye.z < 0.1 || eye.z > 5.9 || eye.y < 0.1 || eye.y > 7) {
			eye = eye - right * d;
			center = center - right * d;
		}
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
		if (eye.x < 0.1 || eye.x > 6.1 || eye.z < 0.1 || eye.z > 5.9 || eye.y < 0.1 || eye.y > 7) {
			eye = eye - up.unit() * d;
			center = center - up.unit() * d;
		}
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
		if (eye.x < 0.1 || eye.x>6.1 || eye.z < 0.1 || eye.z > 5.9 || eye.y < 0.1 || eye.y > 7) {
			eye = eye - view * d;
			center = center - view * d;
		}
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}
};

bool flashOn = false;  // Variable to toggle flashing on/off

bool goalCollected = false;  // Variable to track goal collection

bool obstacleHit = false;  // Variable to track obstacle hits

void updateLighting() {
	// Make the updates on the moving light
	GLfloat lightPosition[] = { playerX, playerY + 2.5, playerZ, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	// Flashing effect on coin collection
	if (flashOn) {
		GLfloat flashIntensity[] = { 5000.0f, 5000.0f, 5000.0f, 5000.0f };  // Increased intensity
		glLightfv(GL_LIGHT0, GL_DIFFUSE, flashIntensity);
	}
	else if (goalCollected) {
		GLfloat flashIntensity[] = { 0.0f, 5000.0f, 0.0f, 5000.0f };  // Green intensity
		glLightfv(GL_LIGHT0, GL_DIFFUSE, flashIntensity);
	}
	else if (obstacleHit) {
		GLfloat flashIntensity[] = { 5000.0f, 0.0f, 0.0f, 5000.0f };  // Red intensity
		glLightfv(GL_LIGHT0, GL_DIFFUSE, flashIntensity);
	}
	else {
		GLfloat lightIntensity[] = { 0.7f, 0.7f, 1.0f, 1.0f };  // Original intensity
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
	}

}

void setupLights() {
	// Material properties...
	GLfloat ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat shininess[] = { 10};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	// Light source properties...
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat ambientLight[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

	GLfloat diffuseLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);

	GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	GLfloat lightPosition[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	// Enable other lights if needed (GL_LIGHT1, GL_LIGHT2, etc.)
}

void updatelights() {

}



Camera camera;

void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 480 / 480, 0.001, 3000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}

int cameraZoom = 0;

// Model Variables
Model_3DS model_house;
Model_3DS model_tree;
Model_3DS model_rocks;
Model_3DS model_Target;
Model_3DS model_coin;
Model_3DS model_box;
Model_3DS model_star;
Model_3DS model_trophy;
Model_3DS model_fan;
Model_3DS model_lamp;
// Textures
GLTexture tex_ground;
GLTexture tex_street;
GLTexture tex_rocks;

//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	glEnable(GL_LIGHT0);

	// Define Even Dimmer Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // Adjust intensity as needed
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Even Dimmer Light source 0 diffuse light
	GLfloat diffuse[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // Adjust intensity as needed
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Even Dimmer Light source 0 Specular light
	GLfloat specular[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // Adjust intensity as needed
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}


//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

void myKeyboard(unsigned char button, int x, int y);

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	camera.look();

	//*******************************//
	// eye (ex, ey, ez): defines the location of the camera.									 //
	// center (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// up (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-10, 0, -30);
	glTexCoord2f(5, 0);
	glVertex3f(10, 0, -30);
	glTexCoord2f(5, 5);
	glVertex3f(10, 0, 30);
	glTexCoord2f(0, 5);
	glVertex3f(-10, 0, 30);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

//=======================================================================
// Render Ground Function
//=======================================================================
void RenderStreet()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_street.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-10, 0, -30);
	glTexCoord2f(5, 0);
	glVertex3f(10, 0, -30);
	glTexCoord2f(5, 5);
	glVertex3f(10, 0, 30);
	glTexCoord2f(0, 5);
	glVertex3f(-10, 0, 30);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

//=======================================================================

GLuint headTexture;  // Texture ID for the head
GLuint bodyTexture;  // Texture ID for the body
GLuint armTexture;   // Texture ID for the arms
GLuint legTexture;   // Texture ID for the legs

void drawCylinder(float radius, float height, int slices) {
	glBegin(GL_TRIANGLE_FAN);

	// Center of the bottom circle
	glVertex3f(0.0f, 0.0f, 0.0f);

	// Vertices for the bottom circle
	for (int i = 0; i <= slices; ++i) {
		float theta = static_cast<float>(i) / static_cast<float>(slices) * 2.0f * 3.14;
		float x = radius * cos(theta);
		float z = radius * sin(theta);
		glVertex3f(x, 0.0f, z);
	}

	glEnd();

	glBegin(GL_TRIANGLE_FAN);

	// Center of the top circle
	glVertex3f(0.0f, height, 0.0f);

	// Vertices for the top circle
	for (int i = 0; i <= slices; ++i) {
		float theta = static_cast<float>(i) / static_cast<float>(slices) * 2.0f * 3.14;
		float x = radius * cos(theta);
		float z = radius * sin(theta);
		glVertex3f(x, height, z);
	}

	glEnd();

	glBegin(GL_QUAD_STRIP);

	// Vertices for the side surface
	for (int i = 0; i <= slices; ++i) {
		float theta = static_cast<float>(i) / static_cast<float>(slices) * 2.0f * 3.14;
		float x = radius * cos(theta);
		float z = radius * sin(theta);

		glVertex3f(x, 0.0f, z);
		glVertex3f(x, height, z);
	}

	glEnd();
}

void drawTexturedCube(float size) {
	float halfSize = size * 0.5f;

	// Define vertices for each face of the cube
	GLfloat vertices[][3] = {
		{ -halfSize, -halfSize, halfSize },  // Front bottom left
		{ halfSize, -halfSize, halfSize },   // Front bottom right
		{ halfSize, halfSize, halfSize },    // Front top right
		{ -halfSize, halfSize, halfSize },   // Front top left

		{ -halfSize, -halfSize, -halfSize }, // Back bottom left
		{ halfSize, -halfSize, -halfSize },  // Back bottom right
		{ halfSize, halfSize, -halfSize },   // Back top right
		{ -halfSize, halfSize, -halfSize }   // Back top left
	};

	// Define face indices for each quad
	GLint faces[][4] = {
		{ 0, 1, 2, 3 },  // Front face
		{ 1, 5, 6, 2 },  // Right face
		{ 4, 5, 6, 7 },  // Back face
		{ 0, 4, 7, 3 },  // Left face
		{ 0, 1, 5, 4 },  // Bottom face
		{ 2, 3, 7, 6 }   // Top face
	};

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, playertex);

	glBegin(GL_QUADS);
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 4; ++j) {
			// Calculate texture coordinates based on vertex position
			float u = (vertices[faces[i][j]][0] + halfSize) / size;
			float v = (vertices[faces[i][j]][1] + halfSize) / size;

			glTexCoord2f(u, v);
			glVertex3fv(vertices[faces[i][j]]);
		}
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
}


void drawPlayer() {
	// draw a texture head using glutSolidSphere
	glPushMatrix();
	glTranslatef(0.0f, 1.5f, 0.0f);
	glColor3f(0.7f, 0.5f, 0.5f);
	glutSolidSphere(0.5f, 20, 20); // Head sphere
	glPopMatrix();

	////Human color
	//glColor3f(0.7f, 0.5f, 0.5f);
	//glPushMatrix();
	//glTranslatef(0.0f, 1.5f, 0.0f);
	//glutSolidSphere(0.5f, 20, 20); // Head sphere
	//glPopMatrix();


	// Body (a cube)
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, 0.75f, 0.0f);
	drawTexturedCube(1.0f); // Body cube
	glPopMatrix();

	// Left Arm (stacked spheres)
	glColor3f(0.0f, 1.0f, 0.6f);
	glPushMatrix();
	glTranslatef(-0.6f, 1.25f, 0.0f);
	for (int i = 0; i < 3; ++i) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		if (i == 2)
			glColor3f(0.7f, 0.5f, 0.5f);
		glTranslatef(0.0f, -i * 0.25f, 0.0f); // Stack spheres for the arm
		glutSolidSphere(0.2f, 10, 10); // Arm sphere
		glPopMatrix();
	}
	glPopMatrix();

	// Right Arm (stacked spheres)
	glColor3f(0.0f, 1.0f, 0.6f);
	glPushMatrix();
	glTranslatef(0.6f, 1.25f, 0.0f);
	for (int i = 0; i < 3; ++i) {
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		if (i == 2)
			glColor3f(0.7f, 0.5f, 0.5f);
		glTranslatef(0.0f, -i * 0.25f, 0.0f); // Stack spheres for the arm
		glutSolidSphere(0.2f, 10, 10); // Arm sphere
		glPopMatrix();
	}
	glPopMatrix();

	// Left Leg (stacked spheres)
	glColor3f(0.7f, 0.7f, 0.7f);
	glPushMatrix();
	glTranslatef(-0.3f, 0.25f, 0.0f);
	for (int i = 0; i < 3; ++i) {
		glPushMatrix();
		if (i == 2)
			glColor3f(0.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, -i * 0.25f, 0.0f); // Stack spheres for the leg
		glutSolidSphere(0.2f, 10, 10); // Leg sphere
		glPopMatrix();
	}
	glPopMatrix();

	// Right Leg (stacked spheres)
	glColor3f(0.7f, 0.7f, 0.7f);
	glPushMatrix();
	glTranslatef(0.3f, 0.25f, 0.0f);
	for (int i = 0; i < 3; ++i) {
		glPushMatrix();
		if (i == 2)
			glColor3f(0.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, -i * 0.25f, 0.0f); // Stack spheres for the leg
		glutSolidSphere(0.2f, 10, 10); // Leg sphere
		glPopMatrix();
	}
	glPopMatrix();
}




int rockAnimationTime = 0;
int coin1AnimationTime = 0;
int coin2AnimationTime = 0;
int coin3AnimationTime = 0;
int coin4AnimationTime = 0;
void Time(int value) {
	if (rockAnimationTime != 0)
		rockAnimationTime--;
	else
		rockAnimation = 0;

	if (coin1AnimationTime != 0)
		coin1AnimationTime--;
	else
	{
		coin1Animation = 0;
		if (coin11 == 1)
			coin1 = 0;
	}
	if (coin2AnimationTime != 0)
		coin2AnimationTime--;
	else
	{
		coin2Animation = 0;
		if (coin22 == 1)
			coin2 = 0;
	}
	if (coin3AnimationTime != 0)
		coin3AnimationTime--;
	else
	{
		coin3Animation = 0;
		if (coin33 == 1)
			coin3 = 0;
	}
	if (coin4AnimationTime != 0)
		coin4AnimationTime--;
	else
	{
		coin4Animation = 0;
		if (coin44 == 1)
			coin4 = 0;
	}
	if (currentLevel == 2) {
		// Star 1 Animation
		if (star1AnimationTime != 0)
			star1AnimationTime--;
		else
		{
			star1Animation = 0;
			if (star11 == 1)
				star1 = 0;
		}
		if (star2AnimationTime != 0)
			star2AnimationTime--;
		else
		{
			star2Animation = 0;
			if (star22 == 1)
				star2 = 0;
		}
		if (star3AnimationTime != 0)
			star3AnimationTime--;
		else
		{
			star3Animation = 0;
			if (star33 == 1)
				star3 = 0;
		}
		if (star4AnimationTime != 0)
			star4AnimationTime--;
		else
		{
			star4Animation = 0;
			if (star44 == 1)
				star4 = 0;
		}
	}

	glutTimerFunc(1000, Time, 0);
}
char rockDirection = 'r';
void idle() {
	// Fan rotation logic for Level 2
	if (currentLevel == 2) {
		fanRotationAngle += 1.0f; // Adjust this value for speed
		if (fanRotationAngle >= 360.0f) {
			fanRotationAngle -= 360.0f;
		}
	}

	// Existing rock animation logic
	if (rockAnimationTime != 0) {
		if (rockDirection == 'r')
			rockAnimation += 0.01;
		else
			rockAnimation -= 0.01;

		if (rockAnimation > 0.03)
			rockDirection = 'l';
		if (rockAnimation < -0.03)
			rockDirection = 'r';
	}

	// Existing coin animation logic
	if (coin1AnimationTime != 0) {
		coin1Animation += 10;
	}
	if (coin2AnimationTime != 0) {
		coin2Animation += 10;
	}
	if (coin3AnimationTime != 0) {
		coin3Animation += 10;
	}
	if (coin4AnimationTime != 0) {
		coin4Animation += 10;
	}

	if (star1AnimationTime != 0) {
		star1Animation += 10;
	}
	if (star2AnimationTime != 0) {
		star2Animation += 10;
	}
	if (star3AnimationTime != 0) {
		star3Animation += 10;
	}
	if (star4AnimationTime != 0) {
		star4Animation += 10;
	}

	// Ensure the scene is updated
	glutPostRedisplay();
}

//=======================================================================
// Display Function
//=======================================================================
int lost = 0;



void drawLightBulb() {
	// Draw the light bulb as a sphere on top and a cylinder on the bottom
	glPushMatrix();

	// Position the light bulb
	glTranslatef(playerX, playerY + 2.5, playerZ);

	// Draw the sphere (top part of the light bulb)
	glColor3f(0.1f, 1.0f, 0.0f);  // White color
	glutSolidSphere(0.2, 10, 10);  // Adjust the sphere radius as needed
	glPopMatrix();

	// Draw the cylinder (bottom part of the light bulb)
	glPushMatrix();
	glTranslatef(playerX, playerY + 2.2, playerZ);
	glColor3f(1.0f, 1.0f, 1.0f);  // White color
	drawCylinder(0.1, 0.4, 10);  // Adjust the cylinder radius and height as needed

	glPopMatrix();
}



// Declare a global boolean variable to keep track of the light state
bool isLightOn = true;

// Modify the setupMovingLight function
void setupMovingLight() {
	// Check if the light is on before enabling it
	if (isLightOn) {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT2);  // Assuming LIGHT2 is available, adjust if needed

		// Define Light source 2 ambient light for the moving light bulb
		GLfloat ambientBulb[] = { 1.0, 0.0f, 0.0f, 1.0f }; // Strong yellow ambient color
		glLightfv(GL_LIGHT2, GL_AMBIENT, ambientBulb);

		// Define Light source 2 diffuse light for the moving light bulb
		GLfloat diffuseBulb[] = { 1.0f, 0.0f, 0.0f, 1.0f };  // Strong yellow diffuse color
		glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseBulb);

		// Define Light source 2 Specular light for the moving light bulb
		GLfloat specularBulb[] = { 1.0f, 0.0f, 1.0f, 1.0f };
		glLightfv(GL_LIGHT2, GL_SPECULAR, specularBulb);
	}
	else {
		glDisable(GL_LIGHT2);
	}
}
// Update fan rotation
void updateFan() {
	fanRotationAngle += fanRotationSpeed;
	if (fanRotationAngle > 360.0f) {
		fanRotationAngle -= 360.0f;
	}
}bool checkCollisionWithFan(float playerX, float playerZ) {
	// Fan blade radius and the angle range for gaps between blades
	float fanBladeRadius = 1.0f * 0.1f; // Adjust based on actual fan size
	float gapAngleRange = 35.0f; // Adjust based on the size of the gaps

	// Iterate over fan positions
	for (int i = -5; i < 15; i += 3) {
		float fanCenterX = i + rockAnimation - 4;
		float fanCenterZValues[] = { 10, -10, 20, -20 }; // Z positions of fans

		for (int j = 0; j < 4; j++) { // Now iterating over 4 Z positions
			float fanCenterZ = fanCenterZValues[j];

			// Calculate distance from player to fan center
			float distanceX = playerX - fanCenterX;
			float distanceZ = playerZ - fanCenterZ;
			float distance = sqrt(distanceX * distanceX + distanceZ * distanceZ);

			// Check if player is within collision range
			if (distance < fanBladeRadius) {
				// Calculate the normalized angle (0 to 360)
				float normalizedAngle = fmod(fanRotationAngle, 360.0f);

				// Check if the player is in a gap between blades
				for (float angle = 0.0f; angle < 360.0f; angle += 90.0f) { // Assuming 4 blades
					if (normalizedAngle > angle + gapAngleRange / 2 && normalizedAngle < angle + 90.0f - gapAngleRange / 2) {
						return false; // Player is in a gap, no collision
					}
				}
				return true; // Player hits a blade
			}
		}
	}

	return false; // No collision with any fan
}



void checkWinCondition() {
	if (currentLevel == 2 && playerX > -2 && playerX < 1 && playerZ < -28) {
		if (!goalSoundPlayed) {
			PlaySound(TEXT("goal"), NULL, SND_ASYNC);
			goalSoundPlayed = true;

			// Assuming the sound duration is 3 seconds (3000 milliseconds)
			glutTimerFunc(1000, [](int value) {
				gameWon = true;
				soundPlayed = true; // Indicate that the sound has been played
				}, 0);
			glutTimerFunc(2000, [](int value) {
				PlaySound(TEXT("win"), NULL, SND_ASYNC);
				}, 0);

			return; // Return early to avoid immediate transition
		}
	}
	
}

bool lostSoundPlayed = false;

void renderEndScreen() {
	if (lives == 0) {
		//play lose sound
		if (!lostSoundPlayed) {
			PlaySound(TEXT("lose"), NULL, SND_ASYNC);
			lostSoundPlayed = true;

		}
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set up orthographic projection for 2D rendering
		glMatrixMode(GL_PROJECTION);

		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, 1280, 0, 720);


		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		// Set text color
		glColor3f(1.0, 0.0, 0.0);  // White color

		// Render "You Win!" message
		char winMessage[] = "You Lost!";
		glRasterPos2i(1280 / 2 - 50, 720 / 2);  // Adjust position as needed
		for (char* c = winMessage; *c != '\0'; c++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
		}

		// Render score message
		char scoreMessage[50];
		sprintf(scoreMessage, "Your Score: %d", score);
		glRasterPos2i(1280 / 2 - 50, 720 / 2 - 30);  // Adjust position as needed
		for (char* c = scoreMessage; *c != '\0'; c++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
		}

		// Restore the original projection matrix
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		// Swap buffers if using double buffering
		glutSwapBuffers();
		takingInput = false;
	}
	else {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set up orthographic projection for 2D rendering
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, 1280, 0, 720);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		// Set text color
		glColor3f(0.0, 1.0, 0.0);  // White color

		// Render "You Win!" message
		char winMessage[] = "You Win!";
		glRasterPos2i(1280 / 2 - 50, 720 / 2);  // Adjust position as needed
		for (char* c = winMessage; *c != '\0'; c++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
		}

		// Render score message
		char scoreMessage[50];
		sprintf(scoreMessage, "Your Score: %d", score);
		glRasterPos2i(1280 / 2 - 50, 720 / 2 - 30);  // Adjust position as needed
		for (char* c = scoreMessage; *c != '\0'; c++) {
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
		}

		// Restore the original projection matrix
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		// Swap buffers if using double buffering
		glutSwapBuffers();
		takingInput = false;
	}
}
float declight = 0.0;

void updatelights(int value) {
	declight=declight+0.01;
	glutTimerFunc(100,updatelights,0);

}

void myDisplay(void)
{
	checkWinCondition();
	if (gameWon) {
		renderEndScreen();
		return;
	}
	else {
		setupCamera();
		if (lives > 0)
		{
			setupLights();
			updateLighting();

			if (GoalLevel1 && currentLevel == 1) {
				currentLevel = 2;
				initLevel2();  // Initialize Level 2
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			GLfloat lightIntensity[] = { 0.7-declight, 0.7-declight, 0.7-declight, 1.0f };
			GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
			glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
			glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

			if (currentLevel == 2) {
				RenderStreet();
			}
			else {
				// Draw Ground
				RenderGround();

			}
			// Draw moving light bulb
			setupMovingLight();
			drawLightBulb();
			if (currentLevel == 1) {

				// Draw multiple Tree Models along the sides of the ground
				for (int i = -25; i < 30; i += 5)
				{
					glPushMatrix();
					glTranslatef(9, 0, i);
					glScalef(0.5, 0.5, 0.5);
					model_tree.Draw();
					glPopMatrix();
					glPushMatrix();
					glTranslatef(-9, 0, i);
					glScalef(0.5, 0.5, 0.5);
					model_tree.Draw();
					glPopMatrix();
				}
			}
			else {
				for (int i = -25; i < 30; i += 5)
				{
					glPushMatrix();
					glTranslatef(9, 0, i);
					glScalef(0.1, 0.1, 0.1);
					model_lamp.Draw();
					glPopMatrix();
					glPushMatrix();
					glTranslatef(-9, 0, i);
					glScalef(0.1, 0.1, 0.1);
					model_lamp.Draw();
					glPopMatrix();
				}

			}




			// Draw multiple Rock Models across the ground
			if (currentLevel == 1) {
				for (int i = -5; i < 15; i += 3)
				{

					glPushMatrix();
					glTranslatef(i + rockAnimation - 4, 1, 0);
					glScalef(0.0033, 0.0033, 0.0015);
					//glBindTexture(GL_TEXTURE_2D, tex_rock);
					model_box.Draw();
					glPopMatrix();
					glPushMatrix();
					glTranslatef(i + rockAnimation - 4, 1, 20);
					glScalef(0.0033, 0.0033, 0.0015);
					//glBindTexture(GL_TEXTURE_2D, tex_rock);
					model_box.Draw();
					glPopMatrix();
					glPushMatrix();
					glTranslatef(i + rockAnimation - 5, 1, -20);
					glScalef(0.0035, 0.0033, 0.0015);
					//glBindTexture(GL_TEXTURE_2D, tex_rock);
					model_box.Draw();
					glPopMatrix();
					//glPushMatrix();
					//glTranslatef(i + rockAnimation - 4, 1, 0);
					//glScalef(0.0033, 0.0033, 0.0033);
					////glBindTexture(GL_TEXTURE_2D, tex_rock);
					//model_box.Draw();
					//glPopMatrix();
					//glPushMatrix();
					//glTranslatef(i + rockAnimation - 4, 1, 0);
					//glScalef(0.0033, 0.0033, 0.0033);
					////glBindTexture(GL_TEXTURE_2D, tex_rock);
					//model_box.Draw();
					//glPopMatrix();

				}
			}
			if (currentLevel == 2) {
				for (int i = -5; i < 15; i += 3)
				{

					glPushMatrix();
					glTranslatef(i + rockAnimation - 4, 1, 0);
					glScalef(0.0033, 0.0033, 0.0033);
					//glBindTexture(GL_TEXTURE_2D, tex_rock);
					model_box.Draw();
					glPopMatrix();

					glPushMatrix();
					glTranslatef(i + rockAnimation - 4, 3.5, 10);
					glScalef(0.1, 0.1, 0.1);
					glRotatef(90, 1, 0, 0); // Rotate 90 degrees around the x-axis
					glRotatef(fanRotationAngle, 0, 1, 0); // Rotate for spinning effect
					model_fan.Draw();
					glPopMatrix();

					glPushMatrix();
					glTranslatef(i + rockAnimation - 4, 1, 20);
					glScalef(0.0033, 0.0033, 0.0033);
					//glBindTexture(GL_TEXTURE_2D, tex_rock);
					model_box.Draw();
					glPopMatrix();

					glPushMatrix();
					glTranslatef(i + rockAnimation - 4, 3.5, -10);
					glScalef(0.1, 0.1, 0.1);
					glRotatef(90, 1, 0, 0); // Rotate 90 degrees around the x-axis
					glRotatef(fanRotationAngle, 0, 1, 0); // Rotate for spinning effect
					model_fan.Draw();
					glPopMatrix();

					glPushMatrix();
					glTranslatef(i + rockAnimation - 5, 1, -20);
					glScalef(0.0035, 0.0033, 0.0033);
					//glBindTexture(GL_TEXTURE_2D, tex_rock);
					model_box.Draw();
					glPopMatrix();

				}
			}

			
			

			if (currentLevel == 1) {
				//sky box
				glPushMatrix();
				GLUquadricObj* qobj;
				qobj = gluNewQuadric();
				glTranslated(50, 0, 0);
				glRotated(90, 1, 0, 1);
				glBindTexture(GL_TEXTURE_2D, tex);
				gluQuadricTexture(qobj, true);
				gluQuadricNormals(qobj, GL_SMOOTH);
				gluSphere(qobj, 100, 100, 100);
				gluDeleteQuadric(qobj);
				glPopMatrix();
				// Draw Player
				glPushMatrix();
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Adjust the color as needed
				glTranslatef(playerX, 0.5, playerZ);
				glRotated(angleY, 0, 1, 0);
				glScalef(0.7, 0.7, 0.7);
				drawPlayer();
				glPopMatrix();
				
				
				
				
			}
			if (currentLevel == 2) {// Draw Player
				//sky box
				glPushMatrix();
				GLUquadricObj* qobj;
				qobj = gluNewQuadric();
				glTranslated(50, 0, 0);
				glRotated(90, 1, 0, 1);
				glBindTexture(GL_TEXTURE_2D, tex1);
				gluQuadricTexture(qobj, true);
				gluQuadricNormals(qobj, GL_SMOOTH);
				gluSphere(qobj, 100, 100, 100);
				gluDeleteQuadric(qobj);
				glPopMatrix();
				glPushMatrix();
				glTranslatef(playerX, 0.5, playerZ);
				glRotated(angleY, 0, 1, 0);
				glScalef(0.7, 0.7, 0.7);
				drawPlayer();
				glPopMatrix();
				

			}

			// Draw Target Model
			if (currentLevel == 1) {
				glPushMatrix();
				glColor3f(1.0f, 0.0f, 0.0f);
				glTranslatef(0, 1, -29);
				glScalef(0.005, 0.005, 0.005);
				model_Target.Draw();
				glPopMatrix();
			}
			
			// Draw coin Model 1
			if (currentLevel == 1) {
				if (coin1 == 1)
				{
					glPushMatrix();
					glColor3f(1, 1, 0);
					glTranslatef(-2, 0.8, 13.5);
					glScalef(0.3, 0.3, 0.3);
					glRotatef(90 + coin1Animation, 1, 0, 0);
					model_coin.Draw();
					glPopMatrix();
				}
				//coin 2
				if (coin2 == 1)
				{
					glPushMatrix();
					glColor3f(1, 1, 0);
					glTranslatef(0, 0.8, 3.5);
					glScalef(0.3, 0.3, 0.3);
					glRotatef(90 + coin2Animation, 1, 0, 0);
					model_coin.Draw();
					glPopMatrix();
				}
				//coin 3
				if (coin3 == 1)
				{
					glPushMatrix();
					glColor3f(1, 1, 0);
					glTranslatef(1.5, 0.8, -6);
					glScalef(0.3, 0.3, 0.3);
					glRotatef(90 + coin3Animation, 1, 0, 0);
					model_coin.Draw();
					glPopMatrix();
				}
				//coin 4
				if (coin4 == 1)
				{
					glPushMatrix();
					glColor3f(1, 1, 0);
					glTranslatef(-1, 0.8, -16.5);
					glScalef(0.3, 0.3, 0.3);
					glRotatef(90 + coin4Animation, 1, 0, 0);
					model_coin.Draw();
					glPopMatrix();
				}
			}
			if (currentLevel == 2) {
				// Render Star 1 (only if it exists and hasn't been collected)
				if (star1 == 1) {
					glPushMatrix();
					glColor3f(1, 1, 0);
					glTranslatef(-2, 0.8, 13.5); // Position of Star 1
					glScalef(0.005, 0.005, 0.005);
					glRotatef(star1Animation, 0, 1, 0); // Apply rotation for Star 1
					model_star.Draw();
					glPopMatrix();
				}

				// Render Star 2 (only if it exists and hasn't been collected)
				if (star2 == 1) {
					glPushMatrix();
					glColor3f(1, 1, 0);
					glTranslatef(0, 0.8, 3.5); // Position of Star 2
					glScalef(0.005, 0.005, 0.005);
					glRotatef(star2Animation, 0, 1, 0); // Apply rotation for Star 2
					model_star.Draw();
					glPopMatrix();
				}

				// Render Star 3 (only if it exists and hasn't been collected)
				if (star3 == 1) {
					glPushMatrix();
					glColor3f(1, 1, 0);
					glTranslatef(1.5, 0.8, -6); // Position of Star 3
					glScalef(0.005, 0.005, 0.005);
					glRotatef(star3Animation, 0, 1, 0); // Apply rotation for Star 3
					model_star.Draw();
					glPopMatrix();
				}

				// Render Star 4 (only if it exists and hasn't been collected)
				if (star4 == 1) {
					glPushMatrix();
					glColor3f(1, 1, 0);
					glTranslatef(-1, 0.8, -16.5); // Position of Star 4
					glScalef(0.005, 0.005, 0.005);
					glRotatef(star4Animation, 0, 1, 0); // Apply rotation for Star 4
					model_star.Draw();
					glPopMatrix();
				}
			}
		
			if (currentLevel == 2) {
				glPushMatrix();
				//glColor3f(1.0f, 0.0f, 0.0f);
				glTranslatef(0, 1, -29);
				glScalef(0.005, 0.005, 0.005);
				model_trophy.Draw();
				glPopMatrix();
			}
			// Display score
			// Set up orthographic projection for 2D rendering
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			gluOrtho2D(0, 1280, 0, 720);

			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			// Set text color
			glColor3f(1.0, 1.0, 1.0);  // White color

			char scoreText[20];
			sprintf(scoreText, "Score: %d", score);
			glRasterPos2i(1280-120, 720 - 30);
			for (char* c = scoreText; *c != '\0'; c++) {
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);  // Increase text size to 24
			}
			char livesText[20];
			sprintf(livesText, "Lives: %d", lives);
			glRasterPos2i(10, 720 - 30);
			for (char* c = livesText; *c != '\0'; c++) {
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);  // Increase text size to 24
			}
			if (currentLevel == 1) {
				char livesText[50];
				sprintf(livesText, "You are playing level 1, Enjoy :)");
				glRasterPos2i(1280-800,720-30);
				for (char* c = livesText; *c != '\0'; c++) {
					glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);  // Increase text size to 24
				}
			}
			else {
				char livesText[50];
				sprintf(livesText, "You are playing level 2, Enjoy :)");
				glRasterPos2i(1280 - 800, 720 - 30);
				for (char* c = livesText; *c != '\0'; c++) {
					glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);  // Increase text size to 24
				}
			}

			// Restore the original projection matrix
			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();


			glutSwapBuffers();
		}
		else {
			if (lost == 0)
			{
				renderEndScreen();
			}

		}
	}
}

//=======================================================================
// Keyboard Function
//=======================================================================

int checkCollisionW();

int checkCollisionS();

void stopGoalFlash(int value) {
	goalCollected = false;
	glutPostRedisplay();  // Trigger a redraw to update the lighting
}

int checkCollisionGoal() {
	if (playerX > -2 && playerX < 1 && playerZ < -28) {
		if (currentLevel == 1) {
			// Goal of Level 1 reached
			PlaySound(TEXT("goal"), NULL, SND_ASYNC);
			model_Target.visible = false;
			GoalLevel1 = true;
			goalCollected = true;

			// Schedule a callback to turn off the green flashing after a short duration
			glutTimerFunc(2500, stopGoalFlash, 0);  // 2500 milliseconds (adjust as needed)

			// Transition to Level 2
			currentLevel = 2;
			initLevel2();// Initialize Level 2 settings
			//displayLevel2();
			return 1;
		}
		/*else if (currentLevel == 2) {
			if (!goalSoundPlayed) {
				PlaySound(TEXT("goal"), NULL, SND_ASYNC);
				goalSoundPlayed = true;

				// Assuming the sound duration is 3 seconds (3000 milliseconds)
				glutTimerFunc(3000, [](int value) {
					gameWon = true;
					goalSoundPlayed = false; // Reset for possible replay or next goal
					}, 0);

				return 0; // Return 0 to avoid immediate transition
			}
		}*/
		
	}
	return 0;
}

void toggleFlash(int value) {
	flashOn = !flashOn;
	glutPostRedisplay();  // Trigger a redraw to update the lighting

	if (flashOn) {
		// Schedule another callback to turn off the flashing after a short duration
		glutTimerFunc(500, toggleFlash, 0);  // 500 milliseconds duration of each flash
	}
}
void stopFlash(int value) {
	flashOn = false;
	glutPostRedisplay();  // Trigger a redraw to update the lighting
}
int checkCollisionCoin(float coinX, float coinZ, int coinCollected) {
	// get the coin collected int
	if (playerX >= coinX - 1 && playerX <= coinX + 1 && playerZ >= coinZ && playerZ <= coinZ + 0.5 && coinCollected == 0) {
		score += 10;
		// Trigger multiple flashes
		for (int i = 0; i < 5; i++) {  // Adjust the number of flashes as needed
			glutTimerFunc(500, toggleFlash, 0);  // 500 milliseconds delay between flashes
		}
		return 1;
	}
	return 0;
}
void checkCollisionStar() {
	// Check for collision with Star 1
	if (playerX >= star1X - 1 && playerX <= star1X + 1 && playerZ >= star1Z - 1 && playerZ <= star1Z + 1 && star1Collected == 0) {
		star1Collected = 1;  // Mark the star as collected
		star1AnimationTime = 2;  // Start the animation
		score += 10;  // Update the score
		star11 = 1;
		PlaySound(TEXT("collectable"), NULL, SND_ASYNC);
		for (int i = 0; i < 5; i++) {
			glutTimerFunc(500 * i, toggleFlash, 0);  // Trigger flashes
		}
	}

	// Check for collision with Star 2
	if (playerX >= star2X - 1 && playerX <= star2X + 1 && playerZ >= star2Z - 1 && playerZ <= star2Z + 1 && star2Collected == 0) {
		star2Collected = 1;
		star2AnimationTime = 2;  // Start the animation
		score += 10;
		star22=1;
		PlaySound(TEXT("collectable"), NULL, SND_ASYNC);
		for (int i = 0; i < 5; i++) {
			glutTimerFunc(500 * i, toggleFlash, 0);
		}
	}

	// Check for collision with Star 3
	if (playerX >= star3X - 1 && playerX <= star3X + 1 && playerZ >= star3Z - 1 && playerZ <= star3Z + 1 && star3Collected == 0) {
		star3Collected = 1;
		star3AnimationTime = 2;  // Start the animation
		score += 10;
		star33=1;
		PlaySound(TEXT("collectable"), NULL, SND_ASYNC);
		for (int i = 0; i < 5; i++) {
			glutTimerFunc(500 * i, toggleFlash, 0);
		}
	}

	// Check for collision with Star 4
	if (playerX >= star4X - 1 && playerX <= star4X + 1 && playerZ >= star4Z - 1 && playerZ <= star4Z + 1 && star4Collected == 0) {
		star4Collected = 1;
		star4AnimationTime = 2;  // Start the animation
		score += 10;
		star44 = 1;
		PlaySound(TEXT("collectable"), NULL, SND_ASYNC);
		for (int i = 0; i < 5; i++) {
			glutTimerFunc(500 * i, toggleFlash, 0);
		}
	}
}


bool switchView = false;

void myKeyboard(unsigned char button, int x, int y)
{
	if (takingInput) {
		switch (button)
		{
		case 'w'://Player moves forward
			direction = 'w';
			if (checkCollisionW() == 1)
				break;
			if (playerZ > -29 - moveSpeed)
			{
				if (view == 1) {
					camera.center.x = playerX;
					camera.center.z = -100;
					camera.eye.z -= moveSpeed;
					playerZ -= moveSpeed;
					angleY = 0;
					camera.look();
				}
				else {
					if (switchView && view != 't')
						myKeyboard('3', 1, 1);
					camera.center.x = playerX;
					if (view != 't')
						camera.center.z = -100;
					camera.eye.z -= moveSpeed;
					playerZ -= moveSpeed;
					angleY = 0;
					camera.look();
				}

			} 
 // Check for star collision in Level 2 after moving
			if (currentLevel == 2) {
				checkCollisionStar();
			}
			break;
		case 's'://Player moves backward
			direction = 's';
			if (checkCollisionS() == 1)
				break;
			if (playerZ < 29 + moveSpeed)
			{
				if (view == 1) {
					camera.center.x = playerX;
					camera.center.z = 100;
					camera.eye.z += moveSpeed;
					playerZ += moveSpeed;
					angleY = 0;
					camera.look();
				}
				else {
					if (switchView && view != 't')
						myKeyboard('3', 1, 1);
					camera.center.x = playerX;
					if (view != 't')
						camera.center.z = 100;
					camera.eye.z += moveSpeed;
					playerZ += moveSpeed;
					angleY = 0;
					camera.look();
				}
			}
			else {
				PlaySound(TEXT("obstacleHit"), NULL, SND_ASYNC);
			}
			if (currentLevel == 2) {
				checkCollisionStar();
			}
			break;
		case 'a'://Player moves left
			direction = 'a';
			if (playerX > -9 - moveSpeed)
			{
				if (view == 1) {
					camera.center.x = -100;
					camera.center.z = playerZ;
					camera.eye.x -= moveSpeed;
					playerX -= moveSpeed;
					angleY = 90;
					camera.look();
				}
				else {
					if (switchView && view != 't')
						myKeyboard('3', 1, 1);
					if (view != 't')
						camera.center.x = -100;
					camera.center.z = playerZ;
					camera.eye.x -= moveSpeed;
					playerX -= moveSpeed;
					angleY = 90;
					camera.look();
				}
			}
			break;
		case 'd'://Player moves right.
			direction = 'd';
			if (playerX < 9 + moveSpeed)
			{
				if (view == 1) {
					camera.center.x = 100;
					camera.center.z = playerZ;
					camera.eye.x += moveSpeed;
					playerX += moveSpeed;
					angleY = 90;
					camera.look();
				}
				else {
					if (switchView && view != 't')
						myKeyboard('3', 1, 1);
					if (view != 't')
						camera.center.x = 100;
					camera.center.z = playerZ;
					camera.eye.x += moveSpeed;
					playerX += moveSpeed;
					angleY = 90;
					camera.look();
				}
			}
			break;
		case '1':
			view = '1';
			switchView = false;
			switch (direction)
			{
			case 'w':
				camera.eye.x = playerX;
				camera.eye.y = 2;
				camera.eye.z = playerZ;
				camera.center.x = playerX;
				camera.center.y = 2;
				camera.center.z = -100;
				camera.look();
				break;
			case 's':
				camera.eye.x = playerX;
				camera.eye.y = 2;
				camera.eye.z = playerZ;
				camera.center.x = playerX;
				camera.center.y = 2;
				camera.center.z = 100;
				camera.look();
				break;
			case 'a':
				camera.eye.x = playerX;
				camera.eye.y = 2;
				camera.eye.z = playerZ;
				camera.center.x = -100;
				camera.center.y = 2;
				camera.center.z = playerZ;
				camera.look();
				break;
			case 'd':
				camera.eye.x = playerX;
				camera.eye.y = 2;
				camera.eye.z = playerZ;
				camera.center.x = 100;
				camera.center.y = 2;
				camera.center.z = playerZ;
				camera.look();
				break;
			}
			break;
		case '3':
			switchView = true;
			view = '3';
			switch (direction)
			{
			
			case 'w':
				camera.eye.x = playerX;
				camera.eye.y = 3;
				camera.eye.z = playerZ + 6;
				camera.center.x = playerX;
				camera.center.y = 3;
				camera.center.z = -100;
				camera.look();
				break;
			case 's':
				camera.eye.x = playerX;
				camera.eye.y = 3;
				camera.eye.z = playerZ - 6;
				camera.center.x = playerX;
				camera.center.y = 3;
				camera.center.z = 100;
				camera.look();
				break;
			case 'a':
				camera.eye.x = playerX + 6;
				camera.eye.y = 3;
				camera.eye.z = playerZ;
				camera.center.x = -100;
				camera.center.y = 3;
				camera.center.z = playerZ;
				camera.look();
				break;
			case 'd':
				camera.eye.x = playerX - 6;
				camera.eye.y = 3;
				camera.eye.z = playerZ;
				camera.center.x = 100;
				camera.center.y = 3;
				camera.center.z = playerZ;
				camera.look();
				break;
			}
			break;
		case 't':
			view = 't';
			camera.eye.x = playerX;
			camera.eye.y = 10;
			camera.eye.z = playerZ;
			camera.center.x = playerX - 0.1;
			camera.center.y = 0;
			camera.center.z = playerZ - 0.1;
			camera.look();
			printf("playerX: %f, playerZ: %f", playerX, playerZ);
			break;
		case 'L':
		case 'l':
			// Toggle the light state
			isLightOn = !isLightOn;
			break;
		case 'p':
			currentLevel = 2;
			initLevel2();
			break;
		case 27:
			exit(0);
			break;
		default:
			break;
		}
		checkCollisionGoal();
		if (currentLevel == 1) {

			if (checkCollisionCoin(-2, 13.5, coin1Collected) == 1 && coin1Collected == 0)
			{
				PlaySound(TEXT("collectable"), NULL, SND_ASYNC);
				coin1AnimationTime = 2;
				coin1Collected = 1;
				coin11 = 1;
			}
			if (checkCollisionCoin(0, 3.5, coin2Collected) == 1 && coin2Collected == 0)
			{
				PlaySound(TEXT("collectable"), NULL, SND_ASYNC);
				coin2AnimationTime = 2;
				coin2Collected = 1;
				coin22 = 1;
			}
			if (checkCollisionCoin(1.5, -6, coin3Collected) == 1 && coin3Collected == 0)
			{
				PlaySound(TEXT("collectable"), NULL, SND_ASYNC);
				coin3AnimationTime = 2;
				coin3Collected = 1;
				coin33 = 1;
			}
			if (checkCollisionCoin(-1, -16.5, coin4Collected) == 1 && coin4Collected == 0)
			{
				PlaySound(TEXT("collectable"), NULL, SND_ASYNC);
				coin4AnimationTime = 2;
				coin4Collected = 1;
				coin44 = 1;
			}
		}
		
		glutPostRedisplay();
	}
}

void initLevel2() {
	fanRotationAngle = 0.0f;
	fanCollisionDetected = false;
	playerX = 0.0f;
	playerZ = 29.0f;
	playerY = 0.0f;
	if (view == '1')
		myKeyboard('1', 1, 1);
	else
		myKeyboard('3', 1, 1);
	camera.look();
	// Initialize positions for each star to match the coin positions in Level 1
	star1X = -2;  // Coin 1 X-coordinate
	star1Z = 13.5;  // Coin 1 Z-coordinate

	star2X = 0;  // Coin 2 X-coordinate
	star2Z = 3.5;  // Coin 2 Z-coordinate

	star3X = 1.5;  // Coin 3 X-coordinate
	star3Z = -6;  // Coin 3 Z-coordinate

	star4X = -1;  // Coin 4 X-coordinate
	star4Z = -16.5;  // Coin 4 Z-coordinate



	score = 0;
	lives = 5;

	coin1Collected = 0;
	coin2Collected = 0;
	coin3Collected = 0;
	coin4Collected = 0;



	star1 = 1; star1Animation = 0.0f; star1Collected = 0;
	star2 = 1; star2Animation = 0.0f; star2Collected = 0;
	star3 = 1; star3Animation = 0.0f; star3Collected = 0;
	star4 = 1; star4Animation = 0.0f; star4Collected = 0;


	rockAnimation = 0.0f;
	coin1Animation = 0.0f;
	coin2Animation = 0.0f;
	coin3Animation = 0.0f;
	coin4Animation = 0.0f;

	// Reset level completion flag for Level 2
	GoalLevel2 = false;
}



void toggleObstacleFlash(int value) {
	obstacleHit = !obstacleHit;
	glutPostRedisplay();  // Trigger a redraw to update the lighting

	if (obstacleHit) {
		// Schedule another callback to turn off the obstacle flashing after a short duration
		glutTimerFunc(500, toggleObstacleFlash, 0);  // 500 milliseconds duration of each flash
	}
}
bool checkCollisionWithBox(float playerX, float playerZ) {
	float boxSize = 2.0f; // Adjust based on actual box size

	// Iterate over box positions
	for (int i = -5; i < 15; i += 3) {
		float boxCenterX = i + rockAnimation - 4; // X positions of boxes
		float boxCenterZValues[] = { 0, 20, -20 }; // Z positions of boxes

		for (int j = 0; j < 3; j++) {
			float boxCenterZ = boxCenterZValues[j];

			// Check if player is within the box boundaries
			if (playerX >= boxCenterX - boxSize && playerX <= boxCenterX + boxSize &&
				playerZ >= boxCenterZ - boxSize && playerZ <= boxCenterZ + boxSize) {
				return true; // Collision detected
			}
		}
	}
	return false; // No collision
}

int checkCollisionW() {
	// Collision logic for Level 1
	if (currentLevel == 1) {
		if (playerZ == 21 || playerZ == 11 || playerZ == 1 || playerZ == -9 || playerZ == -19) {
			// Handle collision for Level 1
			PlaySound(TEXT("obstacleHit"), NULL, SND_ASYNC);
			lives--;
			rockAnimationTime = 2;
			playerZ = 29;
			playerX = 0;
			if (view == '1')
				myKeyboard('1', 1, 1);
			else
				myKeyboard('3', 1, 1);
			// Schedule multiple callbacks to toggle the obstacle flash on and off
			for (int i = 0; i < 5; ++i) {
				glutTimerFunc(500, toggleObstacleFlash, 0);
			}
			return 1;
		}
	}

	// Collision logic for Level 2
	else if (currentLevel == 2) {
		// Check for collision with fan blades or boxes
		if (checkCollisionWithFan(playerX, playerZ) || checkCollisionWithBox(playerX, playerZ)) {
			PlaySound(TEXT("obstacleHit"), NULL, SND_ASYNC);
			lives--;
			// Reset player position
			playerZ = 29;
			playerX = 0;
			if (view == '1')
				myKeyboard('1', 1, 1);
			else
				myKeyboard('3', 1, 1);

			// Schedule multiple callbacks to toggle the obstacle flash on and off
			for (int i = 0; i < 5; ++i) {
				glutTimerFunc(500 * i, toggleObstacleFlash, 0);
			}
			return 1;
		}
	}

	return 0; // No collision
}

int checkCollisionS() {
	if (playerZ == 18 || playerZ == 8 || playerZ == -2 || playerZ == -12 || playerZ == -22)
	{
		PlaySound(TEXT("obstacleHit"), NULL, SND_ASYNC);
		lives--;
		rockAnimationTime = 2;
		playerZ = 29;
		playerX = 0;
		if (view == '1')
			myKeyboard('1', 1, 1);
		else
			myKeyboard('3', 1, 1);
		// Schedule multiple callbacks to toggle the obstacle flash on and off
		for (int i = 0; i < 5; ++i) {  // Adjust the number of flashes as needed
			glutTimerFunc(500, toggleObstacleFlash, 0);  // 500 milliseconds delay between flashes
		}
		return 1;
	}
	return 0;
}
int checkJumpCollissionAhead() {
	float posZ = playerZ - jumpDistanceZ;
	if ((posZ > 18 && posZ < 21) || (posZ > 8 && posZ < 11) || (posZ > -2 && posZ < 1) || (posZ > -12 && posZ < -9)) {
		lives--;
		PlaySound(TEXT("obstacleHit"), NULL, SND_ASYNC);
		rockAnimationTime = 2;
		playerZ = 29;
		playerX = 0;
		if (view == '1')
			myKeyboard('1', 1, 1);
		else
			myKeyboard('3', 1, 1);
		return 1;
	}
	return 0;
}
int checkJumpCollissionBack() {
	float posZ = playerZ + jumpDistanceZ;
	if ((posZ > 18 && posZ < 21) || (posZ > 8 && posZ < 11) || (posZ > -2 && posZ < 1) || (posZ > -12 && posZ < -9)) {
		lives--;
		PlaySound(TEXT("obstacleHit"), NULL, SND_ASYNC);
		rockAnimationTime = 2;
		playerZ = 29;
		playerX = 0;
		if (view == '1')
			myKeyboard('1', 1, 1);
		else
			myKeyboard('3', 1, 1);
		return 1;
	}
	return 0;
}


//=======================================================================
// Motion Function
//=======================================================================
void myMotion(int x, int y)
{
	y = HEIGHT - y;

	if (cameraZoom - y > 0)
	{
		camera.eye.x += -0.1;
		camera.eye.z += -0.1;
	}
	else
	{
		camera.eye.x += 0.1;
		camera.eye.z += 0.1;
	}

	cameraZoom = y;

	glLoadIdentity();	//Clear Model_View Matrix

	camera.look();

	GLfloat light_position[] = { camera.eye.x, camera.eye.y, camera.eye.z, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();	//Re-draw scene 
}

//=======================================================================
// Mouse Function
//=======================================================================
void myMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			if (checkJumpCollissionAhead() == 0 && playerZ - jumpDistanceZ - moveSpeed > -29)
			{
				myKeyboard('w', 1, 1);
				playerZ -= jumpDistanceZ + moveSpeed;
				camera.eye.z -= jumpDistanceZ + moveSpeed;
				glutPostRedisplay();
			}
		}
	}
	if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN) {
			if (checkJumpCollissionBack() == 0 && playerZ + jumpDistanceZ < 29)
			{
				myKeyboard('s', 1, 1);
				playerZ += jumpDistanceZ - moveSpeed;
				camera.eye.z += jumpDistanceZ - moveSpeed;
				glutPostRedisplay();
			}
		}
	}
	y = HEIGHT - y;

	if (state == GLUT_DOWN)
	{
		cameraZoom = y;
	}
}

//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
}

//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	// Loading Model files
	model_tree.Load("models/tree/Tree1.3ds");
	model_rocks.Load("models/Rock/stones.3ds");
	model_coin.Load("models/Coin/Coin 4.3ds");
	model_Target.Load("models/Target/Target.3ds");
	model_box.Load("models/box/box.3ds");
	model_star.Load("models/star/star.3ds");
	model_trophy.Load("models/trophy/trophy.3ds");
	model_fan.Load("models/fan/fan.3ds");
	model_lamp.Load("models/lamp/lamp.3ds");
	//Load texture files for the head , body and the limbs
	loadBMP(&headTexture, "Textures/ground.bmp", true);
	//loadBMP(&bodyTexture, "Textures/body.bmp", true);
	//loadBMP(&armTexture, "Textures/arm.bmp", true);
	//loadBMP(&legTexture, "Textures/leg.bmp", true);


	// Loading texture files
	tex_ground.Load("Textures/ground.bmp");
	tex_street.Load("Textures/street.bmp");
	loadBMP(&tex, "Textures/blu-sky-3.bmp", true);
	loadBMP(&tex1, "Textures/download.bmp", true);
	loadBMP(&playertex, "Textures/body.bmp", true);
	loadBMP(&tex_rock, "Models/Rock/stone.bmp", true);


}

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 150);

	glutCreateWindow(title);


	glutDisplayFunc(myDisplay);

	glutKeyboardFunc(myKeyboard);

	glutMotionFunc(myMotion);

	glutMouseFunc(myMouse);

	glutReshapeFunc(myReshape);

	glutIdleFunc(idle);

	glutTimerFunc(1000, Time, 0);
	glutTimerFunc(500, updatelights, 1);

	myInit();

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}