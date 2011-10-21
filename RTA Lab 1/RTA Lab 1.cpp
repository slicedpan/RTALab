#define _USE_MATH_DEFINES

#include <windows.h>
#include <mmsystem.h>	// ditto
#include <iostream>		// I/O
#include "glut.h"	// GLUT
#include "gl/glu.h"	// for gluPerspective & gluLookAt
#include "glm/glm.h"
#include "svl/SVL.h"
#include "svl/SVLgl.h"

void setupScene();
void updateScene();
void renderScene();
void exitScene();
void keypress(unsigned char key, int x, int y);
void setViewport(int width, int height);
void mouseMove(int x, int y);
void renderLeg(float orientation, float* position, float timeOffset);
void drawSpline();

bool *keyStates = new bool[256];
bool visibleSpline = true;
char msg[256];

int rotationAngle=0;
float time = 0.0f;
float yaw = 0.0f;
float pitch = 0.0f;
float speed = 5.0f;
GLUquadric *nQ;
int mouseX = 400, mouseY = 300;

float radiansToDegrees = 57.2957795f;

Vec3 p1(0.0, 1.5, 0.0);
Vec3 p2(0.0, 1.5, 5.0);
Vec3 p3(5.0, 1.5, 10.0);
Vec3 p4(2.5, 1.5, 15.0);

Vec3 bezierPosition(float t)
{
	return t * ((t * (t * p1 + (1.0f- t) * p2)) + (1.0f - t) * (t * p2 + (1.0f - t) * p3)) + (1.0f - t) * (t * (t * p2 + (1.0f - t) * p3) + (1.0f - t) * (t * p3 + (1.0f - t) * p4));
}

struct _position
{
	float x;
	float y;
	float z;
} camPosition;

GLMmodel * skullModel;
GLuint skullTex;
GLuint skullList;

GLMmodel * wallModel;
GLuint wallTex;
GLuint wallList;

GLMmodel * floorModel;
GLuint floorTex;
GLuint floorList;

bool		wireframe=false;
int         windowId;
DWORD		lastTickCount;
GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
GLfloat left_light_position[] = {0.0f, 6.0f, 4.0f, 1.0f}; 
GLfloat right_light_position[] = {0.0f, 6.0f, -4.0f, 1.0f};
GLfloat grey_ambient[] =
{0.01745, 0.01745, 0.01745}, grey_diffuse[] =
{0.161424, 0.161424, 0.161424}, grey_specular[] =
{0.727811, 0.727811, 0.727811}, grey_shininess = 100.0f;

LPVOID glutFonts[7] = {
    GLUT_BITMAP_9_BY_15,
    GLUT_BITMAP_8_BY_13,
    GLUT_BITMAP_TIMES_ROMAN_10,
    GLUT_BITMAP_TIMES_ROMAN_24,
    GLUT_BITMAP_HELVETICA_10,
    GLUT_BITMAP_HELVETICA_12,
    GLUT_BITMAP_HELVETICA_18
}; 

void glutPrint(float x, float y, LPVOID font, char* text, float r, float g, float b, float a)
{
    if(!text || !strlen(text)) return;
    bool blending = false;
    if(glIsEnabled(GL_BLEND)) blending = true;
    glEnable(GL_BLEND);
    glColor4f(r,g,b,a);
    glRasterPos2f(x,y);
    while (*text) {
        glutBitmapCharacter(font, *text);
        text++;
    }
    if(!blending) glDisable(GL_BLEND);
}

void renderSpider()
{
	float width = 0.5f;	
	float length = 1.0f;
	float legAngle = 20.0f;
	float pos[3];

	float t = (sinf(rotationAngle / 100.0f) / 2.0f) + 0.5f;

	//spider
	glPushMatrix();
	Vec3 p = bezierPosition(t);
	glTranslatef(p[0], p[1], p[2]);

	Vec3 nextP = bezierPosition(t + 0.01f);
	float spiderYaw = atan2f(nextP[0] - p[0], nextP[2] - p[2]);
	glRotatef(spiderYaw * (180.0f / M_PI), 0.0f, 1.0f, 0.0f); 
	//legs
	glPushMatrix();

	pos[0] = width;
	pos[1] = 0.0f;
	pos[2] = 0.0f;
	renderLeg(2 * legAngle, pos, 0.0f);
	pos[0] = -width;
	renderLeg(180.0f - (2.0f * legAngle), pos, M_PI);

	pos[2] = length / 3.0f;
	pos[0] = -width * 1.1f;
	renderLeg(180.0f - legAngle, pos, 1.5f * M_PI);

	pos[0] = width * 1.1f;
	renderLeg(legAngle, pos, 0.5f * M_PI);

	pos[2] *= 2.0f;
	renderLeg(0.0f, pos, M_PI);

	pos[0] = - width * 1.1f;
	renderLeg(180.0f, pos, 0.0f);

	pos[2] = length;
	pos[0] = -width;
	renderLeg(180.0f + legAngle, pos, 0.5f * M_PI);

	pos[0] = width;
	renderLeg(-legAngle, pos, 1.5f * M_PI);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.5f);
	glScalef(1.0f, 1.2f, 2.0f);
	gluSphere(nQ, 0.5f, 15, 15);
	glPopMatrix();

	glTranslatef(0.0f, 0.35f, -0.8f);
	gluSphere(nQ, 0.8f, 15, 15);

	glPopMatrix();
	//head
	glPushMatrix();
	glTranslatef(0.0f, 0.1f, 1.3f);
	glScalef(0.8f, 0.8f, 0.8f);
	glEnable(GL_TEXTURE_2D);	
	glCallList(skullList);
	glPopMatrix();

	glPopMatrix();
}

void renderLeg(float orientation, float* position, float timeOffset)
{
	glPushMatrix();	

	glTranslatef(*position, *(position + 1), *(position + 2));
	glRotatef(orientation, 0.0f, 1.0f, 0.0f);
	glRotatef(22.5f * sinf(time * speed + timeOffset) + 45.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);

	gluCylinder(nQ, 0.15, 0.15, 1.5, 30, 5);
	gluSphere(nQ, 0.15, 5, 5);

	glTranslatef(0.0f, 0.0f, 1.5f);
	glRotatef(20.0f * cosf(time * speed + timeOffset) + 75.0f, 1.0f, 0.0f, 0.0f);

	gluCylinder(nQ, 0.15, 0.15, 2.0f, 30, 5);
	gluSphere(nQ, 0.15, 5, 5);

	glTranslatef(0.0f, 0.0f, 2.0f);
	glRotatef(-10.0f * cosf(time * speed + timeOffset) - 20.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(nQ, 0.15, 0.03, 0.75f, 30, 5);
	gluSphere(nQ, 0.16, 5, 5);

	glPopMatrix();
}

void drawSpline()
{
	float t = 0.01f;
	glBegin(GL_LINES);
	Vec3 lastPos = bezierPosition(0.0f);
	glColor4f(1.0, 0.0, 0.0, 1.0);
	while (t < 1.0f)
	{

		glVertex(lastPos);
		lastPos = bezierPosition(t);
		glVertex(lastPos);
		t += 0.01f;
	}
	glEnd();
}

void renderScene(){
        
    // Clear framebuffer & depth buffer
	float pos[3];


	glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	//Enable lighting
  

    // Reset Modelview matrix      	
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glRotatef(pitch, -1.0f, 0.0f, 0.0f);
	glRotatef(yaw, 0.0f, 1.0f, 0.0f);
	glTranslatef(-camPosition.x, -camPosition.y, -camPosition.z);

	drawSpline();

	glEnable(GL_LIGHTING);

	//Set the material properties
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, grey_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, grey_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, grey_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, grey_shininess); 

	glLightfv(GL_LIGHT0, GL_POSITION, left_light_position);
	glLightfv(GL_LIGHT1, GL_POSITION, right_light_position);

	renderSpider();

	glCallList(wallList);
	glCallList(floorList);	

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	//glDisable(GL_TEXTURE_2D);
	glLoadIdentity();

	gluOrtho2D(0.0, 800.0, 600.0, 0.0);
	glutPrint(10.0f, 10.0f, glutFonts[0], msg, 1.0f, 1.0f, 1.0f, 255.0f);

    // Swap double buffer for flicker-free animation
    glutSwapBuffers();
        
}

void mouseMove(int x, int y)
{
	mouseX = x;
	mouseY = y;
}

void moveForward(float amount)
{
	camPosition.z -= cosf(-yaw / radiansToDegrees) * amount;
	camPosition.x -= sinf(-yaw / radiansToDegrees) * amount;
}

void moveRight(float amount)
{
	camPosition.z += cosf((-yaw + 90.0f) / radiansToDegrees) * amount;
	camPosition.x += sinf((-yaw + 90.0f) / radiansToDegrees) * amount;
}

void updateScene(){
	
	// Wait until at least 16ms passed since start of last frame
	// Effectively caps framerate at ~60fps
	while(timeGetTime()-lastTickCount<16);
	lastTickCount=timeGetTime();
    
    // Increment angle for next frame
    rotationAngle+=2;
	time += 0.016f;
	// Do any other updates here
	
	// Draw the next frame
    glutPostRedisplay();
	yaw += (mouseX - 400) / 10.0f;
	pitch += (300 - mouseY) / 10.0f;

	memset(msg, 0, 256);
	sprintf(msg, "x: %d, y: %d, dy: %d, dy: %d", mouseX, mouseY, mouseX - 400, 300 - mouseY);

	glutWarpPointer(400, 300);

	if (keyStates['w'] == true)
	{
		moveForward(0.2f);
	}
	if (keyStates['s'] == true)
	{
		moveForward(-0.2f);
	}
	if (keyStates['a'] == true)
	{
		moveRight(-0.2f);
	}
	if (keyStates['d'] == true)
	{
		moveRight(0.2f);
	}
	if (keyStates[' '] == true)
	{
		camPosition.y += 0.2f;
	}
	if (keyStates['c'] == true)
	{
		camPosition.y -= 0.2f;
	}
}

void keyup(unsigned char key, int x, int y)
{
	keyStates[key] = false;
}

void keypress(unsigned char key, int x, int y){
	
	// Test if user pressed ESCAPE (ascii 27)
	// If so, exit the program
    if(key==27){
		exitScene();
	}

	// 'W' key toggles wireframe mode on & off
	if(key == 'q' || key == 'Q'){
		wireframe=!wireframe;
		if(wireframe){
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	keyStates[key] = true;

	// Other possible keypresses go here
	//if(key == 'a'){...}
}

void setupScene(){

	std::cout<<"Initializing scene..."<<std::endl;
    
	//Set up Lighting Stuff
	float quadAtten = 0.01f;
	float linearAtten = 0.01f;

	glLightfv(GL_LIGHT0, GL_POSITION, left_light_position);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, &quadAtten);
	glLightfv(GL_LIGHT0, GL_LINEAR_ATTENUATION, &linearAtten);
	glLightfv(GL_LIGHT1, GL_POSITION, right_light_position);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, &quadAtten);
	glLightfv(GL_LIGHT1, GL_LINEAR_ATTENUATION, &linearAtten);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);

	camPosition.x = 0.0f;
	camPosition.y = 0.0f;
	camPosition.z = 10.0f;
	glutWarpPointer(400, 300);

	nQ = gluNewQuadric();

	glutSetCursor(GLUT_CURSOR_NONE); 

	float junk;	

	skullModel = glmReadOBJ("skull.obj");
	skullList = glmList(skullModel, GLM_SMOOTH | GLM_MATERIAL);
	floorModel = glmReadOBJ("floor.obj");
	floorList = glmList(floorModel, GLM_SMOOTH);
	wallModel = glmReadOBJ("walls.obj");
	glmVertexNormals(wallModel, 90, false);
	wallList = glmList(wallModel, GLM_SMOOTH);

	//skullTex = glmLoadTexture("skulltex.ppm", true, false, false, true, &junk, &junk);
	
    // Generate GL texture ID & load texture
	//glGenTextures(1, &textureId);
    //textureTga logoTexture("BushRumsfeld2.tga", textureId);
	//textureTga logoTexture("BushRumsfeld.tga", textureId);
        
}

void exitScene(){

    std::cout<<"Exiting scene..."<<std::endl;

    // Close window
    glutDestroyWindow(windowId);

    // Free any allocated memory

    // Exit program
    exit(0);
}

void setViewport(int width, int height) {

    // Work out window ratio, avoid divide-by-zero
    if(height==0)height=1;
	float ratio = float(width)/float(height);

	// Reset projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Fill screen with viewport
	glViewport(0, 0, width, height);

	// Set a 45 degree perspective
	gluPerspective(45, ratio, .1, 1000);

}

int main(int argc, char *argv[]){
        
    // Initialise OpenGL
    glutInit(&argc, argv); 

    // Set window position, size & create window
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(50,50);
    glutInitWindowSize(640,480);
	windowId = glutCreateWindow("IET Lab 1: DIY Spider");
    // Set GLUT callback functions
    glutReshapeFunc(setViewport);
    glutDisplayFunc(renderScene);
    glutIdleFunc(updateScene);
    glutKeyboardFunc(keypress);
	glutKeyboardUpFunc(keyup);
	glutPassiveMotionFunc(mouseMove);
	glutMotionFunc(mouseMove);	

    // Setup OpenGL state & scene resources (models, textures etc)
    setupScene();

    // Show window & start update loop
    glutMainLoop();    

    return 0;
    
}
