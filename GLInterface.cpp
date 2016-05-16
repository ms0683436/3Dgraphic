#include "GLInterface.h"
#include "GLSLSetup.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include<vector>
#include<iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include<GL/freeglut.h>
#pragma comment(lib, "glew32.lib")
using namespace std;

#define  PI   3.141592653
#define DEFAULT 0
#define MY_SHADER 1
#define MY_TEXTURE 1
#define   TSIZE  64   /* define texture dimension */
#define radius 5.0
#define cubeSize 100
#define Size 1
#define gridSize 10

//Directional Light
float light_amb[4] = {1.0,1.0,1.0,1};
float light_dif[4] = {1.0,0.0,0.0,1};
float light_spe[4] = {1.0,1.0,1.0,1};
float light_dir[4] = {0,0,100,1};

float light1_spe[] = { 1.0,1.0,1.0,1 };
float light1_dir[] = { 0.0, 1.0, 0.0, 0.0 };
float light1_dif[] = { 0.9, 0.9, 0.2, 1.0 };
//Polygon Material
float material_amb[4] = {0.1,0.1,0.1,1};
float material_dif[4] = {0.6,0.6,0.6,1};
float material_spe[4] = {0.3,0.3,0.3,1};
float shininess = 64.0f;

int shader_effect = DEFAULT;
int texture_effect = DEFAULT;

GLUquadricObj  *sphere = NULL;
//cube
float cube_point[][3] = { {1, 1,1},{-1, 1,1},{-1,-1,1},{1,-1,1}
											,{1,1,-1},{1,1,1},{1,-1,1},{1,-1,-1}
											,{-1,1,-1},{1,1,-1},{1,-1,-1},{-1,-1,-1}
											,{-1,1,1},{-1,1,-1},{-1,-1,-1},{-1,-1,1}
											,{1,1,-1},{-1,1,-1},{-1,1,1},{1,1,1}
											,{-1,-1,-1},{1,-1,-1},{1,-1,1},{-1,-1,1} };
int    cube_face[][4] = { {0,1,2,3} ,{4,5,6,7} ,{8,9,10,11},{12,13,14,15} ,{16,17,18,19},{20,21,22,23} };
float  cube_normals[][3] = { {0,0,1},{1,0,0},{0,0,-1},{-1,0,0},{0,1,0},{0,-1,0} };

float angx = 10.0 ,angy = 0.0;

struct Particle {
	float pos[3];
	float color[3];
	float vel[3];
};
vector<Particle> particles;

//float ball_pos[Size][3];
//float ball_color[Size][3];
//float vel[Size][3];
float  CubeN[][3] = { { 0,0,-1 },{ -1,0,0 },{ 0,0,1 },{ 1,0,0 },{ 0,-1,0 },{ 0,1,0 } };


/*-----Translation and rotations of eye coordinate system---*/
float   zoom = 1.0;
float   eyeDx = 0.0, eyeDy = 0.0, eyeDz = 0.0;
float   eyeAngx = 0.0, eyeAngy = 0.0, eyeAngz = 0.0;
double  Eye[3] = { 0.0, 0.0, 250.0 }, Focus[3] = { 0.0, 0.0, 0.0 },
Vup[3] = { 0.0, 1.0, 0.0 };

float   u[3][3] = { { 1.0,0.0,0.0 },{ 0.0,1.0,0.0 },{ 0.0,0.0,1.0 } };
float   eye[3];
float   cv, sv; /* cos(5.0) and sin(5.0) */

int width=1200, height=1200;

float  a[3], b[3];
float    mtx[16];     /* the modelview matrix */

//Define the coef. of the clip plane {A, B, C, D}
//The clip plane = A*x + B*y + C*z + D = 0.

float plane[5][4];
float viewPoint[5][3];
int    culling = 0;

struct grid {
	int id;
	int ballId;
	float posX;
	float posY;
	float posZ;
};
vector<vector<grid*>> G;

grid* create_grid(int id, int ballId, float x, float y, float z)
{
	// 動態配置記憶體
	grid* n = (grid*)malloc(sizeof(grid));

	n->id = id;
	n->ballId = ballId;
	n->posX = x;
	n->posY = y;
	n->posZ = z;
	return n;
}

void addParticle(float pos[], float vel[])
{
	Particle p;
	for (int i = 0; i < 3; i++) {
		p.pos[i] = pos[i];
		p.vel[i] = vel[i];
		p.color[i] = (double)rand() / RAND_MAX;
	}
	//if (randColor)
	//{
	//	p.color[0] = rand() % 200 / 200.0;
	//	p.color[1] = rand() % 200 / 200.0;
	//	p.color[2] = rand() % 200 / 200.0;
	//}
	//else // if is huge particle make it yellow
	//{
	//	p.color[0] = 1;
	//	p.color[1] = 1;
	//	p.color[2] = 0;
	//}
	particles.push_back(p);

	/*if (line.x1 != 0)
		line.x1 = line.x2 = line.y1 = line.y2 = 0;*/
}

void removeParticles()
{
	for (int i = 0; i < particles.size(); i++)
		particles.pop_back();
}

void ShaderSwitch()
{
	if(shader_effect == DEFAULT) shader_effect = MY_SHADER;
	else shader_effect = DEFAULT;
}

void Light()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_AMBIENT,light_amb);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,light_dif);
	glLightfv(GL_LIGHT0,GL_SPECULAR,light_spe);
	glLightfv(GL_LIGHT0,GL_POSITION,light_dir);
	glDisable(GL_LIGHT0);

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_dif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_spe);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_dir);
	glDisable(GL_LIGHT1);
	glDisable(GL_LIGHTING);
}

float dot(float v1[], float v2[])
{
	return (v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]);
}

vector<float> cross(vector<float> v1, vector<float> v2)
{
	vector<float> normal;
	float Nx = (v1[1] * v2[2] - v1[2] * v2[1]);
	float Ny = (v1[2] * v2[0] - v1[0] * v2[2]);
	float Nz = (v1[0] * v2[1] - v1[1] * v2[0]);
	normal.push_back(Nx / sqrt(Nx*Nx + Ny*Ny + Nz*Nz));
	normal.push_back(Ny / sqrt(Nx*Nx + Ny*Ny + Nz*Nz));
	normal.push_back(Nz / sqrt(Nx*Nx + Ny*Ny + Nz*Nz));
	/*for (int i = 0; i < 3; i++) {
		cout << "normal " << normal[i] << endl;
	}*/
	return normal;
}

vector<float> calNormal(float p1[], float p2[], float p3[]){
	vector<float> v1, v2;
	v1.push_back(p2[0] - p1[0]);
	v1.push_back(p2[1] - p1[1]);
	v1.push_back(p2[2] - p1[2]);
	v2.push_back(p3[0] - p2[0]);
	v2.push_back(p3[1] - p2[1]);
	v2.push_back(p3[2] - p2[2]);
	/*for (int i = 0; i < 3; i++) {
		cout << "v1 " << v1[i] << endl;
	}
	for (int i = 0; i < 3; i++) {
		cout << "v2 " << v2[i] << endl;
	}*/
	return cross(v1, v2);
}

vector<float> calPlane(float p1[], float p2[], float p3[]) {
	vector<float> temp = calNormal(p1, p2, p3);
	vector<float>clip_equ;
	for (int i = 0; i < 3; i++) {
		clip_equ.push_back(temp[i]);
		//cout << "temp " << temp[i] << endl;
	}
	clip_equ.push_back(-1*((clip_equ[0] * p1[0]) + (clip_equ[1] * p1[1]) + (clip_equ[2] * p1[2])));
	return clip_equ;
}

void Init()
{
	Light();
	if (sphere == NULL) {
		sphere = gluNewQuadric();
		gluQuadricDrawStyle(sphere, GLU_FILL);
		gluQuadricNormals(sphere, GLU_SMOOTH);
	}

	/*---- Compute cos(5.0) and sin(5.0) ----*/
	cv = cos(5.0*PI / 180.0);
	sv = sin(5.0*PI / 180.0);
	/*---- Copy eye position ---*/
	eye[0] = Eye[0];
	eye[1] = Eye[1];
	eye[2] = Eye[2];

	float tempP[] = { 0,-50,0 };
	float tempV[] = {0,0,0};
	
	for (int i = 0; i < Size; i++) {
		addParticle(tempP, tempV);
		/*ball_pos[i][0] = 0;
		ball_pos[i][1] = 20;
		ball_pos[i][2] = 0;
		vel[i][0] = rand() % 3 - 1;
		vel[i][1] = rand() % 3 - 1;
		vel[i][2] = rand() % 3 - 1;
		ball_color[i][0] = (double)rand() / RAND_MAX;
		ball_color[i][1] = (double)rand() / RAND_MAX;
		ball_color[i][2] = (double)rand() / RAND_MAX;*/
	}
}

void DrawCube()
{
	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	glVertex3f(-100, 100, 100);
	glVertex3f(100, 100, 100);
	glVertex3f(100, 100, 100);
	glVertex3f(100, 100, -100);
	glVertex3f(100, 100, -100);
	glVertex3f(-100, 100, -100);
	glVertex3f(-100, 100, -100);
	glVertex3f(-100, 100, 100);

	glVertex3f(-100, -100, 100);
	glVertex3f(100, -100, 100);
	glVertex3f(100, -100, 100);
	glVertex3f(100, -100, -100);
	glVertex3f(100, -100, -100);
	glVertex3f(-100, -100, -100);
	glVertex3f(-100, -100, -100);
	glVertex3f(-100, -100, 100);

	glVertex3f(100, 100, 100);
	glVertex3f(100, -100, 100);
	glVertex3f(100, -100, -100);
	glVertex3f(100, 100, -100);

	glVertex3f(-100, 100, 100);
	glVertex3f(-100, -100, 100);
	glVertex3f(-100, -100, -100);
	glVertex3f(-100, 100, -100);
	glEnd();
}

void compute_ab_axes(void)
{
	float  w0, w2;
	double len;

	/*----Get w0 and w2 from the modelview matrix mtx[] ---*/
	w0 = mtx[2]; w2 = mtx[10];

	len = sqrt(w0*w0 + w2*w2);
	/*---- Define the a and b axes for billboards ----*/
	b[0] = 0.0; b[1] = 1.0; b[2] = 0.0;
	a[0] = w2 / len; a[1] = 0.0; a[2] = -w0 / len;
}

void Display()
{
	glEnable(GL_DEPTH_TEST);

	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glClearColor(0,0,0,1);
	
	//Camera
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye[0], eye[1], eye[2],
		eye[0] - u[2][0], eye[1] - u[2][1], eye[2] - u[2][2],
		u[1][0], u[1][1], u[1][2]);
	glGetFloatv(GL_MODELVIEW_MATRIX, mtx);
	compute_ab_axes();
	
	//projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(zoom*90.0, (double)width / (double)height, 1, 500.0);
	glMatrixMode(GL_MODELVIEW);

	glRotatef(angx, 0.0, 1.0, 0.0);
	glRotatef(angy, 1.0, 0.0, 0.0);
	DrawCube();
	/*-------culling ----*/
	if (culling) {
		float position_y, position_x, position_z;
		position_z = 500.0;
		position_y = position_z*tan(((90.0*zoom) / 2.0)*PI / 180);
		position_x = position_y*((double)width / (double)height);
		
		//cout << "x:" << position_x  << "y:" << position_y << "z:"  << position_z << endl;

		viewPoint[0][0] = eye[0];
		viewPoint[0][1] = eye[1];
		viewPoint[0][2] = eye[2];

		viewPoint[1][0] = eye[0] - position_z*u[2][0] + position_x*u[0][0] + position_y*u[1][0];
		viewPoint[1][1] = eye[1] - position_z*u[2][1] + position_x*u[0][1] + position_y*u[1][1];
		viewPoint[1][2] = eye[2] - position_z*u[2][2] + position_x*u[0][2] + position_y*u[1][2];

		viewPoint[2][0] = eye[0] - position_z*u[2][0] - position_x*u[0][0] + position_y*u[1][0];
		viewPoint[2][1] = eye[1] - position_z*u[2][1] - position_x*u[0][1] + position_y*u[1][1];
		viewPoint[2][2] = eye[2] - position_z*u[2][2] - position_x*u[0][2] + position_y*u[1][2];

		viewPoint[3][0] = eye[0] - position_z*u[2][0] + position_x*u[0][0] - position_y*u[1][0];
		viewPoint[3][1] = eye[1] - position_z*u[2][1] + position_x*u[0][1] - position_y*u[1][1];
		viewPoint[3][2] = eye[2] - position_z*u[2][2] + position_x*u[0][2] - position_y*u[1][2];

		viewPoint[4][0] = eye[0] - position_z*u[2][0] - position_x*u[0][0] - position_y*u[1][0];
		viewPoint[4][1] = eye[1] - position_z*u[2][1] - position_x*u[0][1] - position_y*u[1][1];
		viewPoint[4][2] = eye[2] - position_z*u[2][2] - position_x*u[0][2] - position_y*u[1][2];

		/*for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 3; j++) {
				cout << viewPoint[i][j] << endl;
			}
			cout << "-------------" << endl;
		}*/
		vector<float> normal;
		normal = calPlane(viewPoint[0], viewPoint[1], viewPoint[3]);
		for (int i = 0; i < 4; i++)
			plane[0][i] = normal[i];
		normal = calPlane(viewPoint[0], viewPoint[2], viewPoint[1]);
		for (int i = 0; i < 4; i++)
			plane[1][i] = normal[i];
		normal = calPlane(viewPoint[0], viewPoint[4], viewPoint[2]);
		for (int i = 0; i < 4; i++)
			plane[2][i] = normal[i];
		normal = calPlane(viewPoint[0], viewPoint[3], viewPoint[4]);
		for (int i = 0; i < 4; i++)
			plane[3][i] = normal[i];
		normal = calPlane(viewPoint[1], viewPoint[2], viewPoint[4]);
		for (int i = 0; i < 4; i++)
			plane[4][i] = normal[i];

		/*for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 4; j++) {
				cout << plane[i][j] << endl;
			}
			cout << "-------------" << endl;
		}*/
		
		for (int i = 0; i < Size; i++) {
			float k = 0;
			for (int j = 0; j < 5; j++) {
				if (dot(particles[i].pos, plane[j]) + plane[j][3] >= 0)
					k++;
				else
					break;
			}
			if (k == 5) {
				glPushMatrix();
				glTranslatef(particles[i].pos[0], particles[i].pos[1], particles[i].pos[2]);
				glColor3f(particles[i].color[0], particles[i].color[1], particles[i].color[2]);
				gluSphere(sphere, radius,   /* radius */
					12,            /* composing of 12 slices*/
					12);           /* composing of 8 stacks */
				glPopMatrix();
			}
		}
	}
	else {
		for (int i = 0; i < Size; i++) {
			glPushMatrix();
			glTranslatef(particles[i].pos[0], particles[i].pos[1], particles[i].pos[2]);
			glColor3f(particles[i].color[0], particles[i].color[1], particles[i].color[2]);
			gluSphere(sphere, radius,   /* radius */
				12,            /* composing of 12 slices*/
				12);           /* composing of 8 stacks */
			glPopMatrix();
		}
	}
	int indexG = 0;
	for (int x = -cubeSize; x < cubeSize; x += gridSize) {
		for (int y = -cubeSize; y < cubeSize; y += gridSize) {
			for (int z = -cubeSize; z < cubeSize; z += gridSize) {
				vector <grid*> temp;
				for (int i = 0; i < Size; i++) {
					if (particles[i].pos[0] >= (x - radius) && particles[i].pos[0] < (x + radius + gridSize)) {
						if (particles[i].pos[1] >= (y - radius) && particles[i].pos[1] < (y + radius + gridSize)) {
							if (particles[i].pos[2] >= (z - radius) && particles[i].pos[2] < (z + radius + gridSize)) {
								temp.push_back(create_grid(indexG, i, particles[i].pos[0], particles[i].pos[1], particles[i].pos[2]));
							}
						}
					}	
				}
				if(!temp.empty())
						G.push_back(temp);
				indexG++;
			}
		}
	}
	//cout << indexG << endl;
	glutSwapBuffers();
}

bool MOUSE_LEFT = false, MOUSE_RIGHT = false;
void Mouse(int button,int state,int x,int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state)		MOUSE_LEFT = false;
		else		MOUSE_LEFT = true;
		break;
	case GLUT_RIGHT_BUTTON:
		if (state)		MOUSE_RIGHT = false;
		else		MOUSE_RIGHT = true;
		break;
	}
	glFinish();
}

int oldx = 0;
int oldy = 0;
void Motion(int x,int y)
{
	int    i;
	float  ix[3], iy[3], iz[3];
	if(MOUSE_LEFT) {
		angx += (x - oldx);
		oldx = x;
		angy += (y - oldy);
		oldy = y;
		glFinish();
		glutPostRedisplay();
	}
	if (MOUSE_RIGHT) {
		if ((y - oldy) > 0) {/* pitching */
			eyeAngx += (y - oldy);
			oldy = y;
			if (eyeAngx > 360.0) eyeAngx -= 360.0;
			iy[0] = u[1][0] * cv - u[2][0] * sv;
			iy[1] = u[1][1] * cv - u[2][1] * sv;
			iy[2] = u[1][2] * cv - u[2][2] * sv;

			iz[0] = u[2][0] * cv + u[1][0] * sv;
			iz[1] = u[2][1] * cv + u[1][1] * sv;
			iz[2] = u[2][2] * cv + u[1][2] * sv;

			for (i = 0; i < 3; i++) {
				u[1][i] = iy[i];
				u[2][i] = iz[i];
			}
		}
		if ((y - oldy) < 0) {
			eyeAngx += (y - oldy);
			oldy = y;
			if (eyeAngx<0.0) eyeAngx += 360.0;
			iy[0] = u[1][0] * cv + u[2][0] * sv;
			iy[1] = u[1][1] * cv + u[2][1] * sv;
			iy[2] = u[1][2] * cv + u[2][2] * sv;

			iz[0] = u[2][0] * cv - u[1][0] * sv;
			iz[1] = u[2][1] * cv - u[1][1] * sv;
			iz[2] = u[2][2] * cv - u[1][2] * sv;

			for (i = 0; i<3; i++) {
				u[1][i] = iy[i];
				u[2][i] = iz[i];
			}
		}
		if ((x - oldx) > 0) { /* heading */
			eyeAngy += (x - oldx);
			oldx = x;
			if (eyeAngy<0.0) eyeAngy += 360.0;
			for (i = 0; i<3; i++) {
				ix[i] = cv*u[0][i] + sv*u[2][i];
				iz[i] = -sv*u[0][i] + cv*u[2][i];
			}
			for (i = 0; i<3; i++) {
				u[0][i] = ix[i];
				u[2][i] = iz[i];
			}
		}
		if ((x - oldx) < 0) {
			eyeAngy += (x - oldx);
			oldx = x;
			if (eyeAngy>360.0) eyeAngy -= 360.0;
			for (i = 0; i<3; i++) {
				ix[i] = cv*u[0][i] - sv*u[2][i];
				iz[i] = sv*u[0][i] + cv*u[2][i];
			}
			for (i = 0; i<3; i++) {
				u[0][i] = ix[i];
				u[2][i] = iz[i];
			}
		}
		glFinish();
		glutPostRedisplay();
	}
}

void MouseWheel(int wheel, int direction, int x, int y) {
	switch (direction) {/* zoom in out */
	case 1:	zoom -= 0.01;	break;
	case -1:	zoom += 0.01;	 break;
	}
}

void KeyboardDown(unsigned char key,int ix,int iy)
{
	int    i;
	float  x[3], y[3];
	if (key == 'c') {
		culling = 1;
	}
	else if (key == 'z') {
		culling = 0;
	}

	if (key == 'f') {
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	if (key == 'F') {
		glDisable(GL_CULL_FACE);
	}
	if(key == 'e') ShaderSwitch();
	if (key == 'l') glEnable(GL_LIGHT1);
	if (key == 'L') glDisable(GL_LIGHT1);

	 if (key == 'x') {
		eyeDy += 0.5;       /* move down */
		for (i = 0; i<3; i++) eye[i] -= 0.5*u[1][i];
	}
	 if (key == ' ') {
		eyeDy += -0.5;       /* move up */
		for (i = 0; i<3; i++) eye[i] += 0.5*u[1][i];
	}
	 if (key == 'd') {
		eyeDx += -0.5;       /* move right */
		for (i = 0; i<3; i++) eye[i] += 0.5*u[0][i];
	}
	 if (key == 'a') {
		eyeDx += 0.5;        /* move left */
		for (i = 0; i<3; i++) eye[i] -= 0.5*u[0][i];
	}
	 if (key == 'w') {
		eyeDz += 0.5;        /* forward */
		for (i = 0; i<3; i++) eye[i] -= 0.5*u[2][i];
	}
	 if (key == 's') {
		eyeDz += -0.5;       /*back */
		for (i = 0; i<3; i++) eye[i] += 0.5*u[2][i];
	}
	 if (key == 'r') {            /* rolling */
		eyeAngz += 5.0;
		if (eyeAngz>360.0) eyeAngz -= 360.0;
		for (i = 0; i<3; i++) {
			x[i] = cv*u[0][i] - sv*u[1][i];
			y[i] = sv*u[0][i] + cv*u[1][i];
		}
		for (i = 0; i<3; i++) {
			u[0][i] = x[i];
			u[1][i] = y[i];
		}
	}
	 if (key == 'R') {
		eyeAngz += -5.0;
		if (eyeAngz<0.0) eyeAngz += 360.0;
		for (i = 0; i<3; i++) {
			x[i] = cv*u[0][i] + sv*u[1][i];
			y[i] = -sv*u[0][i] + cv*u[1][i];
		}
		for (i = 0; i<3; i++) {
			u[0][i] = x[i];
			u[1][i] = y[i];
		}
	}
	glFinish();
}

void KeyboardUp(unsigned char c,int x,int y)
{
	glFinish();
}

int testRange(float test[])
{
	if (test[0] < (-1 * cubeSize) || test[0] > cubeSize) {
		if (test[1] < (-1 * cubeSize) || test[1] > cubeSize) {
			if (test[2] < (-1 * cubeSize) || test[2] > cubeSize) {
				if (test[2] < 0)	return 2;
				else		return 0;
			}
			if (test[1] < 0)	return 5;
			else		return 4;
		}
		if (test[2] < (-1 * cubeSize) || test[2] > cubeSize) {
			if (test[2] < 0)	return 2;
			else		return 0;
		}
		if (test[0] < 0)	return 3;
		else		return 1;
	}
	if (test[1] < (-1 * cubeSize) || test[1] > cubeSize) {
		if (test[0] < (-1 * cubeSize) || test[0] > cubeSize) {
			if (test[2] < (-1 * cubeSize) || test[2] > cubeSize) {
				if (test[2] < 0)	return 2;
				else		return 0;
			}
			if (test[0] < 0)	return 3;
			else		return 1;
		}
		if (test[2] < (-1 * cubeSize) || test[2] > cubeSize) {
			if (test[2] < 0)	return 2;
			else		return 0;
		}
		if (test[1] < 0)	return 5;
		else		return 4;
	}
	if (test[2] < (-1*cubeSize) || test[2] > cubeSize) {
		if (test[0] < (-1 * cubeSize) || test[0] > cubeSize) {
			if (test[1] < (-1 * cubeSize) || test[1] > cubeSize) {
				if (test[1] < 0)	return 5;
				else		return 4;
			}
			if (test[0] < 0)	return 3;
			else		return 1;
		}
		if (test[1] < (-1 * cubeSize) || test[1] > cubeSize) {
			if (test[1] < 0)	return 5;
			else		return 4;
		}
		if (test[2] < 0)	return 2;
		else		return 0;
	}
	else		 return 6;
}

void Timer(int c)
{
	int index;
	float tempV[3];
	for (int i = 0; i < Size; i++) {
		index = testRange(particles[i].pos);
		//cout <<  "index:" << index << endl;
		if (index != 6) {
			for (int j = 0; j < 3; j++) {
				//cout << "dot:" << dot(vel[i], CubeN[index]) << endl;
				//cout << "CubeN:" << CubeN[index][j] << endl;
				tempV[j] = particles[i].vel[j] - (2 * dot(particles[i].vel, CubeN[index]) * (CubeN[index][j]));
				//cout << "tempV:" << tempV[j] << endl;
			}
			for (int j = 0; j < 3; j++)
				particles[i].vel[j] = tempV[j];
		}
	}
	int temp1, temp2;
	for (int i = 0; i < G.size(); i++) {
		while (!G[i].empty()) {
			float tempP = 0;
			temp1 = G[i].back()->ballId;
			G[i].pop_back();
			if (!G[i].empty()) {
				temp2 = G[i].back()->ballId;
				G[i].pop_back();
				for (int z = 0; z < 3; z++)
					tempP += pow((particles[temp1].pos[z] - particles[temp2].pos[z]), 2);
				if (tempP <= pow((radius * 2), 2)) {
					/*cout << vel[temp1][0] << vel[temp1][1] << vel[temp1][2] << endl;
					cout << vel[temp2][0] << vel[temp2][1] << vel[temp2][2] << endl;*/
					swap(particles[temp1].vel, particles[temp2].vel);
					swap(particles[temp1].color, particles[temp2].color);
				}
			}
		}
	}

	for (int i = 0; i < Size; i++) {
		for (int j = 0; j < 3; j++)
			particles[i].pos[j] += particles[i].vel[j];
	}

	glutPostRedisplay();
	glutTimerFunc(50,Timer,0);
}
