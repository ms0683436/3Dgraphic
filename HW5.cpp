#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#define  PI   3.141592653
#define Step  0.5
#define   TSIZE  64   /* define texture dimension */

/*-----Define a unit box--------*/
/* Vertices of the box */
float  points[][3] = { { -5.0, 0.0, -5.0 },{ 5.0, 0.0, -3.0 },
{ 5.0, 3.0, -3.0 },{ -5.0, 3.0, -5.0 },
{ -5.0, 0.0, 5.0 },{ 5.0, 0.0, 3.0 },
{ 5.0, 3.0, 3.0 },{ -5.0, 3.0, 5.0 },
{ -3.0,4.0,-3.0 },{ 3.0,4.0,-2.0 },
{ 3.0,4.0,2.0 },{ -3.0,4.0,3.0 },
{ -8.0,1.0,-0.5 },{ -8.0,1.0,0.5 },
{ -8.0,2.0,0.5 },{ -8.0,2.0,-0.5 },
{ 8,1.5,0 } };

int    face[][4] = { { 0, 3, 2, 1 },{ 0, 1, 5, 4 },{ 1, 2, 6, 5 },
{ 4, 5, 6, 7 },{ 2, 3, 7, 6 },{ 0, 4, 7, 3 },
{ 3,8,11,7 },{ 2,9,8,3 },{ 6,10,9,2 },{ 7,11,10,6 },{ 8,9,10,11 },
{ 12,0,3,15 },{ 15,3,7,14 },{ 14,7,4,13 },{ 13,4,0,12 },{ 12,15,14,13 },
{ 16,6,2 },{ 16,5,6 },{ 16,5,1 },{ 16,1,2 } };

float  normals[][3] = { { 1.0,0.0,-5.0 },{ 0.0,-1.0,0.0 },{ 1.0,0.0,0.0 },
{ 1.0,0.0,5.0 },{ 0.0,1.0,0.0 },{ 3.0,10.0,3.0 },
{ 1.0,2.0,0.0 },{ 1.0,7.0,-5.0 },{ 1.0,2.0,0.0 },{ 1.0,3.0,5.0 },{ 0.0,1.0,0.0 },
{ 1.5,0.0,1.0 },{ 1.0,3.0,0.0 },{ 1.5,0.0,-1.0 },{ 1.0,3.0,0.0 },{ 1.0,0.0,0.0 },
{ 1.0,2.0,0.0 },{ 1.0,0.0,1.0 },{ 1.0,-2.0,0.0 },{ 1.0,0.0,-1.0 } };

/* indices of the box faces */
int    cube[6] = { 0, 1, 2, 3, 4, 5 };
GLUquadricObj  *sphere = NULL, *cylind = NULL, *disk;

/*-Declare car position, orientation--*/
float  self_ang = -90.0, glob_ang = 0.0, up_ang = 0.0, bld_ang = 10.0;
float  position[3] = { 0.0, 0.0, 0.0 };
float temp[3] = { 0.0,0.0,0.0 };
/*-----Define window size----*/
int width = 600, height = 600;

/*-----Translation and rotations of eye coordinate system---*/
float   zoom = 1.0;
float   eyeDx = 0.0, eyeDy = 0.0, eyeDz = 0.0;
float   eyeAngx = 0.0, eyeAngy = 0.0, eyeAngz = 0.0;		
double  Eye[3] = { 0.0, 0.0, 30.0 }, Focus[3] = { 0.0, 0.0, 0.0 },
Vup[3] = { 0.0, 1.0, 0.0 };

float   u[3][3] = { { 1.0,0.0,0.0 },{ 0.0,1.0,0.0 },{ 0.0,0.0,1.0 } };
float   eye[3];
float   cv, sv; /* cos(5.0) and sin(5.0) */

				/*-----Drawing stye
				0:4-windows, 1:x direction, 2:y direction, 3:z-dirtection, 4:perspective
				*/
int style = 0;

void draw_scene(void);
float norm2(float v[]);

/*----Define material properties for cube -----*/
float  mat_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
float  mat_diffuse[] = { 0.9, 0.9, 0.9, 1.0 };
float  mat_specular[] = { 0.9, 0.9, 0.9, 1.0 };
float  mat_shininess = 32.0;
float  flr_diffuse[] = { 0.20, 0.60, 0.3, 1.0 };
float  flr_shininess = 4.0;

/*----Define light source properties -------*/
float  moon_direction[] = { 1.0, 1.0, 1.0, 0.0 };
float  moon_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
float  moon_specular[] = { 0.9, 0.9, 0.9, 1.0 };

float  lit1_position[] = { -30.0, 9.0, -30.0, 1.0 };
float  lit1_diffuse[] = { 0.7, 0.7, 0.0, 1.0 };

float  lit2_position[] = { 0.0, 1.0, 5.0, 1.0 };
float  lit2_direction[] = { 1.0, 0.0, 0.0, 0.0 };
float  lit2_diffuse[] = { 0.9, 0.2, 0.2, 1.0 };
float  lit_cutoff = 30.0;

float  lit3_position[] = { 0.0, 1.0, -5.0, 1.0 };
float  lit3_direction[] = { 1.0, 0.0, 0.0, 0.0 };
float  lit3_diffuse[] = { 0.9, 0.9, 0.2, 1.0 };

float  lit4_position[] = { 15.0, 20.0, 0.0, 1.0 };
float  lit4_direction[] = { -15.0, -20.0, 0.0, 1.0 };
float  lit4_diffuse[] = { 0.2, 0.2, 0.9, 1.0 };

float  lit5_position[] = { -15.0, 20.0, 0.0, 1.0 };
float  lit5_direction[] = { 15.0, -20.0, 0.0, 1.0 };
float  lit5_diffuse[] = { 0.2, 0.9, 0.2, 1.0 };
float  lit45_cutoff = 30.0;

float  lit_specular[] = { 0.7, 0.7, 0.7, 1.0 };
float  global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

/*----Some global variables for transformation------*/
float  lit_angle = 0.0, lit_color4[3] = { 0.001,0.001,0.001 }, lit_color5[3] = { 0.001,0.001,0.001 };

int trun0 = 0, trun1 = 0, trun23 = 0, trun45 = 0;

float    ds = 0.0;
float range = 1.0;
unsigned char  checkboard[TSIZE][TSIZE][4];   /* checkboard textures */
unsigned char  star[TSIZE][TSIZE][4];   /* star textures */
unsigned char  brick[TSIZE][TSIZE][4];   /* brick textures */
unsigned char  waterwave[TSIZE][TSIZE][4];   /* waterwave textures */

unsigned int   textName[4];                   /* declare two texture maps*/

float  a[3], b[3];
float    mtx[16];     /* the modelview matrix */

/*----fog------*/
float fog_color[] = { 0.15, 0.20, 0.20, 0.50 };
int turn_fog = 0;
float fog_opacity = 30;

void setup_light()
{
	//glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, moon_direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, moon_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, moon_specular);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, lit1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lit_specular);
	//glEnable(GL_LIGHT1);      /*Turn on light1 */

	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, lit_cutoff);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lit2_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, lit_specular);
	//glEnable(GL_LIGHT2);      /*Turn on light2 */

	glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, lit_cutoff);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, lit3_diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, lit_specular);
	//glEnable(GL_LIGHT3);      /*Turn on light3 */

	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, lit45_cutoff);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, lit4_diffuse);
	glLightfv(GL_LIGHT4, GL_SPECULAR, lit_specular);
	//glEnable(GL_LIGHT4);      /*Turn on light4 */

	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, lit45_cutoff);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, lit5_diffuse);
	glLightfv(GL_LIGHT5, GL_SPECULAR, lit_specular);
	//glEnable(GL_LIGHT5);      /*Turn on light5 */
}

void setup_fog() {
	glFogi(GL_FOG_MODE, GL_LINEAR);  /*fog factor=GL_LINEAR,GL_EXP,or GL_EXP2*/
	glFogf(GL_FOG_DENSITY, 0.1);    /*fog opacity(density)= 0.25*/
	glFogf(GL_FOG_START, 1.0);       /*Setup two ends for GL_LINEAR*/
	glFogf(GL_FOG_END, fog_opacity);
	glFogfv(GL_FOG_COLOR, fog_color);/*set the fog color */
}

void make_check()
{
	int   i, j, c;

	for (i = 0; i < TSIZE; i++)
		for (j = 0; j < TSIZE; j++) {
			c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;
			checkboard[i][j][0] = c;
			checkboard[i][j][1] = c;
			checkboard[i][j][2] = c;
			checkboard[i][j][3] = 255;
		}
}

void Create_Texture_Star()
{
	int x, y, i;

	for (x = 0; x<64; x++)
		for (y = 0; y<64; y++)
		{
			star[x][y][0] = 255;
			star[x][y][1] = 255;
			star[x][y][2] = 255;
			star[x][y][3] = 255;
		}

	x = 31;	y = 60;
	while (x >= 12)
	{
		for (i = y; i>4; i--)
		{
			star[x][i][0] = 100;
			star[x][i][1] = 100;
			star[x][i][2] = 255;
			star[x][i][3] = 255;
		}
		y -= 3;
		x--;
	}
	x = 31;	y = 20;
	while (x >= 12)
	{
		for (i = y; i>4; i--)
		{
			star[x][i][0] = 255;
			star[x][i][1] = 255;
			star[x][i][2] = 255;
			star[x][i][3] = 255;
		}
		y -= 1;
		x--;
	}
	x = 4;	y = 40;
	while (y >= 18)
	{
		for (i = x; i<32; i++)
		{
			star[i][y][0] = 100;
			star[i][y][1] = 100;
			star[i][y][2] = 255;
			star[i][y][3] = 255;
		}
		y -= 1;
		if ((y % 2) == 1)
			x += 2;
		else
			x++;
	}

	for (x = 63; x>31; x--)
		for (y = 0; y<64; y++)
			for (i = 0; i<4; i++)
				star[x][y][i] = star[63 - x][y][i];
}

void brick_pattern()
{
	int  y, x, i;


	for (y = 0; y<12; y++) {    /*----Create the first row of bricks----*/
		for (x = 0; x<2; x++) {
			brick[y][x][0] = 140;
			brick[y][x][1] = 140;
			brick[y][x][2] = 140;
			brick[y][x][3] = 255;
		}

		for (x = 2; x<30; x++) {
			brick[y][x][0] = 200;
			brick[y][x][1] = 0;
			brick[y][x][2] = 0;
			brick[y][x][3] = 255;
		}

		for (x = 30; x<34; x++) {
			brick[y][x][0] = 140;
			brick[y][x][1] = 140;
			brick[y][x][2] = 140;
			brick[y][x][3] = 255;
		}

		for (x = 34; x<62; x++) {
			brick[y][x][0] = 200;
			brick[y][x][1] = 0;
			brick[y][x][2] = 0;
			brick[y][x][3] = 255;
		}

		for (x = 62; x<64; x++) {
			brick[y][x][0] = 140;
			brick[y][x][1] = 140;
			brick[y][x][2] = 140;
			brick[y][x][3] = 255;
		}
	}

	for (y = 12; y<16; y++)     /*--Create the cement between two breick layers */
		for (x = 0; x<64; x++) {
			brick[y][x][0] = 140;
			brick[y][x][1] = 140;
			brick[y][x][2] = 140;
			brick[y][x][3] = 255;
		}

	for (y = 16; y<28; y++) {    /*--Create the 2nd layer of bricks---*/
		for (x = 0; x<14; x++) {
			brick[y][x][0] = 200;
			brick[y][x][1] = 0;
			brick[y][x][2] = 0;
			brick[y][x][3] = 255;
		}

		for (x = 14; x<18; x++) {
			brick[y][x][0] = 140;
			brick[y][x][1] = 140;
			brick[y][x][2] = 140;
			brick[y][x][3] = 255;
		}

		for (x = 18; x<46; x++) {
			brick[y][x][0] = 200;
			brick[y][x][1] = 0;
			brick[y][x][2] = 0;
			brick[y][x][3] = 255;
		}

		for (x = 46; x<50; x++) {
			brick[y][x][0] = 140;
			brick[y][x][1] = 140;
			brick[y][x][2] = 140;
			brick[y][x][3] = 255;
		}

		for (x = 50; x<64; x++) {
			brick[y][x][0] = 200;
			brick[y][x][1] = 0;
			brick[y][x][2] = 0;
			brick[y][x][3] = 255;
		}
	}

	for (y = 28; y<32; y++) /*---Add another layer of cement----*/
		for (x = 0; x<64; x++) {
			brick[y][x][0] = 140;
			brick[y][x][1] = 140;
			brick[y][x][2] = 140;
			brick[y][x][3] = 255;
		}

	for (y = 32; y<64; y++)
		for (x = 0; x<64; x++)
			for (i = 0; i<4; i++) brick[y][x][i] = brick[y - 32][x][i];
}

void Create_Texture_Waterwave()
{
	int x, y, i;

	for (x = 0; x<64; x++)
		for (y = 0; y<32; y++)
		{
			waterwave[x][y][0] = 100;
			waterwave[x][y][1] = 100;
			waterwave[x][y][2] = 255;
			waterwave[x][y][3] = 255;
		}

	x = 0;	y = 4;
	while (y <= 28)
	{
		for (i = x; i<32; i++)
		{
			waterwave[i][y][0] = 200;
			waterwave[i][y][1] = 255;
			waterwave[i][y][2] = 255;
			waterwave[i][y][3] = 255;
		}
		y++;
		if (y < 9)
			x += 3;
		else if (y == 9)
			x++;
		else if (y == 10)
			x += 2;
		else if (y == 11)
			x++;
		else if (y == 12)
			x += 2;
		else if (y == 13)
			x++;
		else if (y == 14)
			x += 2;
		else if (y == 16)
			x++;
		else if (y == 19)
			x++;
		else if (y == 21)
			x++;
		else if (y == 23)
			x++;
		else if (y == 26)
			x++;
		else
			x++;
	}
	x = 0;	y = 0;
	while (y <= 24)
	{
		for (i = x; i<32; i++)
		{
			waterwave[i][y][0] = 100;
			waterwave[i][y][1] = 100;
			waterwave[i][y][2] = 255;
			waterwave[i][y][3] = 255;
		}
		y++;
		if (y < 5)
			x += 3;
		else if (y == 5)
			x++;
		else if (y == 6)
			x += 2;
		else if (y == 7)
			x++;
		else if (y == 8)
			x += 2;
		else if (y == 9)
			x++;
		else if (y == 10)
			x += 2;
		else if (y == 11)
			x++;
		else if (y == 14)
			x++;
		else if (y == 16)
			x++;
		else if (y == 18)
			x++;
		else if (y == 21)
			x++;
		else
			x++;
	}

	for (x = 63; x>31; x--)
		for (y = 0; y<32; y++)
			for (i = 0; i<4; i++)
				waterwave[x][y][i] = waterwave[63 - x][y][i];

	for (y = 32; y<64; y++)
		for (x = 0; x<64; x++)
			for (i = 0; i<4; i++)
			{
				if (x >= 47)
					waterwave[x][y][i] = waterwave[x - 47][y - 32][i];
				else
					waterwave[x][y][i] = waterwave[x + 16][y - 32][i];
			}
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

void  myinit()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);      /*set the background color BLACK */
										   /*Clear the Depth & Color Buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	/*---Create a light source----*/
	glEnable(GL_LIGHTING);
	setup_light();
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	glEnable(GL_NORMALIZE);   /*Enable mornalization  */

							  /*---Create quadratic objects---*/
	if (sphere == NULL) {
		sphere = gluNewQuadric();
		gluQuadricDrawStyle(sphere, GLU_FILL);
		gluQuadricNormals(sphere, GLU_SMOOTH);
	}
	if (cylind == NULL) {
		cylind = gluNewQuadric();
		gluQuadricDrawStyle(cylind, GLU_FILL);
		gluQuadricNormals(cylind, GLU_SMOOTH);
	}
	if (disk == NULL) {
		disk = gluNewQuadric();
		gluQuadricDrawStyle(disk, GLU_FILL);
		gluQuadricNormals(disk, GLU_SMOOTH);
	}

	/*-----Create Texture -----*/
	make_check();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(4, textName);
	glBindTexture(GL_TEXTURE_2D, textName[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, checkboard);

	/*-----Create another texture ----*/
	Create_Texture_Star();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glBindTexture(GL_TEXTURE_2D, textName[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, star);
	
	brick_pattern();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glBindTexture(GL_TEXTURE_2D, textName[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, brick);

	Create_Texture_Waterwave();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glBindTexture(GL_TEXTURE_2D, textName[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, waterwave);

	/*----Set up fog conditions ---*/
	glEnable(GL_FOG);                /*enable fog fade */
	setup_fog();

	/*---- Compute cos(5.0) and sin(5.0) ----*/
	cv = cos(5.0*PI / 180.0);
	sv = sin(5.0*PI / 180.0);
	/*---- Copy eye position ---*/
	eye[0] = Eye[0];
	eye[1] = Eye[1];
	eye[2] = Eye[2];
}

void draw_billboard(float x, float z, float w, float h)
{
	float  v0[3], v1[3], v2[3], v3[3];

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	/*----Compute the 4 vertices of the billboard ----*/
	v0[0] = x - (w / 2)*a[0]; v0[1] = -2.5; v0[2] = z - (w / 2)*a[2];
	v1[0] = x + (w / 2)*a[0]; v1[1] = -2.5; v1[2] = z + (w / 2)*a[2];
	v2[0] = x + (w / 2)*a[0]; v2[1] = h; v2[2] = z + (w / 2)*a[2];
	v3[0] = x - (w / 2)*a[0]; v3[1] = h; v3[2] = z - (w / 2)*a[2];

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3fv(v0);
	glTexCoord2f(1.0, 0.0); glVertex3fv(v1);
	glTexCoord2f(1.0, 1.0); glVertex3fv(v2);
	glTexCoord2f(0.0, 1.0); glVertex3fv(v3);
	glEnd();
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
}

void draw_cube()
{
	int    i;
	glBindTexture(GL_TEXTURE_2D, textName[1]);
	glEnable(GL_TEXTURE_2D);
	glLightfv(GL_LIGHT2, GL_POSITION, lit2_position);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lit2_direction);

	glLightfv(GL_LIGHT3, GL_POSITION, lit3_position);
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, lit3_direction);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_SPECULAR);
	glColor3f(0.95, 0.95, 0.95);
	glMaterialf(GL_FRONT, GL_SHININESS, 6.0);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColor3f(0.9, 0.2, 0.2);
	for (i = 0; i<20; i++) {
		glNormal3fv(normals[i]);
		glBegin(GL_POLYGON);  /* Draw the face */
		glTexCoord2f(0.0, 0.0); glVertex3fv(points[face[i][0]]);
		glTexCoord2f(0.0, range); glVertex3fv(points[face[i][1]]);
		glTexCoord2f(range, range); glVertex3fv(points[face[i][2]]);
		glTexCoord2f(range, 0.0); glVertex3fv(points[face[i][3]]);
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
}

void draw_floor()
{
	glPushMatrix();
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, textName[3]);
	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glTranslatef(ds, 0.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColor3f(1.0, 0.8, 0.8);
	glNormal3f(0.0, 1.0, 0.0);
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			glBegin(GL_POLYGON);
			glTexCoord2f(0.0, 0.0); glVertex3f((i - 5.0)*10.0, -2.5, (j - 5.0)*10.0);
			glTexCoord2f(0.0, range); glVertex3f((i - 5.0)*10.0, -2.5, (j - 4.0)*10.0);
			glTexCoord2f(range, range); glVertex3f((i - 4.0)*10.0, -2.5, (j - 4.0)*10.0);
			glTexCoord2f(range, 0.0); glVertex3f((i - 4.0)*10.0, -2.5, (j - 5.0)*10.0);
			glEnd();
		}
	}
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void draw_slope()
{
	glPushMatrix();
	
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glTranslatef(20.0, 0.0, 20.0);
	glNormal3f(0.0, 1.0, 0.0);
	glColor3f(0.7, 0.7, 0.1);
	glBegin(GL_POLYGON);
	glVertex3f(30.0, -2.4, 15.0);
	glVertex3f(-10.0, -2.4, 15.0);
	glVertex3f(-10.0, -2.4, 0);
	glVertex3f(30.0, -2.4, 0);
	glEnd();

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, textName[0]);
	glEnable(GL_TEXTURE_2D);
	glNormal3f(0.0, 3.0, -2.0);
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glTexCoord2f(range, range); glVertex3f(30.0, 7.5, 15.0);
	glTexCoord2f(range, 0.0); glVertex3f(30.0, -2.5, 0.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-10.0, -2.5, 0.0);
	glTexCoord2f(0.0, range); glVertex3f(-10.0, 7.5, 15.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void draw_axes()
{

	/*----Draw a white sphere to represent the original-----*/
	glColor3f(0.9, 0.9, 0.9);

	gluSphere(sphere, 2.0,   /* radius=2.0 */
		12,            /* composing of 12 slices*/
		12);           /* composing of 8 stacks */

					   /*----Draw three axes in colors, yellow, meginta, and cyan--*/
					   /* Draw Z axis  */
	glColor3f(0.0, 0.95, 0.95);//blue
	gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
		10.0,              /* height of the cylinder */
		12,               /* use 12-side polygon approximating circle*/
		3);               /* Divide it into 3 sections */

						  /* Draw Y axis */
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);  /*Rotate about x by -90', z becomes y */
	glColor3f(0.95, 0.0, 0.95);
	gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
		10.0,             /* height of the cylinder */
		12,               /* use 12-side polygon approximating circle*/
		3);               /* Divide it into 3 sections */
	glPopMatrix();

	/* Draw X axis */
	glColor3f(0.95, 0.95, 0.0);//yellow
	glPushMatrix();
	glRotatef(90.0, 0.0, 1.0, 0.0);  /*Rotate about y  by 90', x becomes z */
	gluCylinder(cylind, 0.5, 0.5,   /* radius of top and bottom circle */
		10.0,             /* height of the cylinder */
		12,               /* use 12-side polygon approximating circle*/
		3);               /* Divide it into 3 sections */
	glPopMatrix();
	/*-- Restore the original modelview matrix --*/
	glPopMatrix();
}

void draw_object()
{
	glPushMatrix();
	/*----Draw a white sphere to represent the original-----*/
	//glLoadIdentity();
	/*----position light1 (fixed) in eye coord sys ---*/
	glColor3f(0.9, 0.9, 0.9);
	glTranslatef(-30.0, 9.0, -30.0);
	gluSphere(sphere, 2.0,   /* radius=2.0 */
		12,            /* composing of 12 slices*/
		12);           /* composing of 8 stacks */
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-30.0, -2.5, -30.0);
	glRotatef(-90.0, 1.0, 0.0, 0.0);  /*Rotate about x by -90', z becomes y */
	glColor3f(0.95, 0.0, 0.95);
	gluCylinder(cylind, 2.0, 0.5, /* radius of top and bottom circle */
		10.0,             /* height of the cylinder */
		12,               /* use 12-side polygon approximating circle*/
		3);               /* Divide it into 3 sections */
	glPopMatrix();
}

void draw_blade()
{
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glNormal3f(0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.5, 0.0, 1.5);
	glVertex3f(0.5, 0.0, 2.5);
	glVertex3f(-0.5, 0.0, 2.5);
	glVertex3f(-0.5, 0.0, 1.5);
	glEnd();
}

void make_view(int x)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	switch (x) {
	case 4:       /* Perspective */

				  /* In this sample program, eye position and Xe, Ye, Ze are computed
				  by ourselves. Therefore, use them directly; no transform is
				  applied upon eye coordinate system
				  */
		gluLookAt(eye[0], eye[1], eye[2],
			eye[0] - u[2][0], eye[1] - u[2][1], eye[2] - u[2][2],
			u[1][0], u[1][1], u[1][2]);
		glGetFloatv(GL_MODELVIEW_MATRIX, mtx);
		compute_ab_axes();
		break;

	case 1:       /* X direction parallel viewing */
		gluLookAt(30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		break;
	case 2:       /* Y direction parallel viewing */
		gluLookAt(0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
		break;
	case 3:
		gluLookAt(0.0, 0.0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		break;
	}
}

/*------------------------------------------------------
* Procedure to make projection matrix
*/
void make_projection(int x)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (x == 4) {
		gluPerspective(zoom*90.0, (double)width / (double)height, 1.5, 50.0);
	}
	else {
		if (width>height)
			glOrtho(-40.0, 40.0, -40.0*(float)height / (float)width,
				40.0*(float)height / (float)width,
				-0.0, 100.0);
		else
			glOrtho(-40.0*(float)width / (float)height,
				40.0*(float)width / (float)height, -40.0, 40.0,
				-0.0, 100.0);
	}
	glMatrixMode(GL_MODELVIEW);
}


/*---------------------------------------------------------
* Procedure to draw view volume, eye position, focus ,...
* for perspective projection
*/
void draw_view()
{
	int    i;

	glMatrixMode(GL_MODELVIEW);

	/*----Draw Eye position-----*/
	glPushMatrix();
	glTranslatef(eye[0], eye[1], eye[2]);
	glColor3f(0.0, 1.0, 0.0);
	glutWireSphere(1.0, 10, 10);
	glPopMatrix();

	/*----Draw eye coord. axes -----*/
	glColor3f(1.0, 1.0, 0.0);           /* Draw Xe *///yellow
	glBegin(GL_LINES);
	glVertex3f(eye[0], eye[1], eye[2]);
	glVertex3f(eye[0] + 20.0*u[0][0], eye[1] + 20.0*u[0][1], eye[2] + 20.0*u[0][2]);
	glEnd();

	glColor3f(1.0, 0.0, 1.0);          /* Draw Ye */
	glBegin(GL_LINES);
	glVertex3f(eye[0], eye[1], eye[2]);
	glVertex3f(eye[0] + 20.0*u[1][0], eye[1] + 20.0*u[1][1], eye[2] + 20.0*u[1][2]);
	glEnd();

	glColor3f(0.0, 1.0, 1.0);          /* Draw Ze *///blue
	glBegin(GL_LINES);
	glVertex3f(eye[0], eye[1], eye[2]);
	glVertex3f(eye[0] + 20.0*u[2][0], eye[1] + 20.0*u[2][1], eye[2] + 20.0*u[2][2]);
	glEnd();

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switch (style) {
	case 0:
		make_view(4);
		make_projection(4);
		glViewport(width / 2, 0, width / 2, height / 2);
		draw_scene();

		make_view(1);
		make_projection(1);
		glViewport(0, height / 2, width / 2, height / 2);
		draw_scene();
		make_view(1);
		draw_view();

		make_view(2);
		glViewport(width / 2, height / 2, width / 2, height / 2);
		draw_scene();
		make_view(2);
		draw_view();

		make_view(3);
		glViewport(0, 0, width / 2, height / 2);
		draw_scene();
		make_view(3);
		draw_view();
		break;

	case 1:
		make_view(1);
		make_projection(1);
		glViewport(0, 0, width, height);
		draw_scene();
		make_view(1);
		draw_view();
		break;

	case 2:
		make_view(2);
		glViewport(0, 0, width, height);
		draw_scene();
		make_view(2);
		draw_view();
		break;

	case 3:
		make_view(3);
		glViewport(0, 0, width, height);
		draw_scene();
		make_view(3);
		draw_view();
		break;
	case 4:
		glViewport(0, 0, width, height);
		make_view(4);
		make_projection(4);
		draw_scene();
		break;
	}
	/*-------Swap the back buffer to the front --------*/
	glutSwapBuffers();
	return;
}



void draw_scene()
{
	static float  ang_self = 0.0;  /*Define the angle of self-rotate */
	static float  angle = 0.0;

	glLightfv(GL_LIGHT4, GL_POSITION, lit4_position);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, lit4_direction);

	glLightfv(GL_LIGHT5, GL_POSITION, lit5_position);
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, lit5_direction);

	glLightfv(GL_LIGHT1, GL_POSITION, lit1_position);  /*fixed position---*/
	glDisable(GL_TEXTURE_2D);
	draw_floor();

	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, 0.0);
	glMatrixMode(GL_MODELVIEW);

	draw_slope();
	draw_axes();
	draw_object();

	/*-------Draw the billboard ----*/
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, textName[2]);
	glEnable(GL_TEXTURE_2D);
	draw_billboard(5.0*4.0, -3.0*4.0, 5.0, 8.0);
	draw_billboard(6.0*4.0, -5.0*4.0, 5.0, 8.0);
	draw_billboard(3.0*4.0, -6.0*4.0, 5.0, 8.0);
	draw_billboard(2.0*4.0, -7.0*4.0, 5.0, 8.0);
	draw_billboard(7.0*4.0, -2.0*4.0, 5.0, 8.0);

	glDisable(GL_TEXTURE_2D);

	/*-------Draw the car body which is a cube----*/
	glTranslatef(position[0], position[1], position[2]);
	glRotatef(-up_ang, 1.0, 0.0, 0.0);
	glRotatef(self_ang, 0.0, 1.0, 0.0);

	glPushMatrix();              /* Save M1 coord. sys */
	draw_cube();
	glPopMatrix();               /* Get M1 back */
								 /*-------Draw the front wheels -----*/
	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();              /* Save M1 coord. sys */
	glTranslatef(-4.0, 0.0, 3.0); /* Go to left wheel position */
	glutSolidTorus(0.5,  /* inner radius */
		1.0,  /* outer radius */
		24,   /* divided into 18 segments */
		12);  /* 12 rings */
	glPopMatrix();

	glPushMatrix();              /* Save M1 coord. sys */
	glTranslatef(-4.0, 0.0, -3.0);/* Go to right wheel position */
	glutSolidTorus(0.5,  /* inner radius */
		1.0,  /* outer radius */
		24,   /* divided into 18 segments */
		12);  /* 12 rings */
	glPopMatrix();

	/*------Draw back wheels ----*/
	glColor3f(1.0, 0.4, 0.0);
	glPushMatrix();              /* Save M1 coord. sys */
	glTranslatef(6.0, 0.0, 0.0); /* Go to left wheel position */
	glutSolidTorus(0.5,  /* inner radius */
		1.0,  /* outer radius */
		24,   /* divided into 18 segments */
		12);  /* 12 rings */
	glPopMatrix();

	glColor3f(1.0, 0.4, 0.4);
	glPushMatrix();              /* Save M1 coord. sys */
	glTranslatef(0.0, 1.5, 7.5); /* Go to left wheel position */
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glutSolidTorus(0.5,  /* inner radius */
		3.0,  /* outer radius */
		24,   /* divided into 18 segments */
		12);  /* 12 rings */
	glPopMatrix();

	glColor3f(1.0, 0.4, 0.4);
	glPushMatrix();              /* Save M1 coord. sys */
	glTranslatef(0.0, 1.5, -7.5); /* Go to left wheel position */
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glutSolidTorus(0.5,  /* inner radius */
		3.0,  /* outer radius */
		24,   /* divided into 18 segments */
		12);  /* 12 rings */
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(0.0, 1.5, 7.5);
	glRotatef(bld_ang, 0.0, 1.0, 0.0);
	draw_blade();/* draw the first blade */
	glRotatef(120, 0.0, 1.0, 0.0);
	draw_blade();/* draw the first blade */
	glRotatef(120, 0.0, 1.0, 0.0);
	draw_blade();/* draw the first blade */
	glPopMatrix();

	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(0.0, 1.5, -7.5);
	glRotatef(bld_ang, 0.0, 1.0, 0.0);
	draw_blade();
	glRotatef(120, 0.0, 1.0, 0.0);
	draw_blade();
	glRotatef(120, 0.0, 1.0, 0.0);
	draw_blade();
	glPopMatrix();

}

void idle_func()
{
	ds += 0.001;
	if (ds>1.0) ds = ds - 1.0;

	if (turn_fog == 1) {
		fog_color[0] += 0.001;
		fog_color[1] += 0.001;
		fog_color[2] += 0.001;

		if (lit4_diffuse[0] > 0.7)
			fog_color[0] *= -1.0;
		else if (lit4_diffuse[0] < 0.1)
			fog_color[0] *= -1.0;

		if (lit4_diffuse[1] > 0.7)
			fog_color[1] *= -1.0;
		else if (lit4_diffuse[1] < 0.1)
			fog_color[1] *= -1.0;

		if (lit4_diffuse[2] > 0.7)
			fog_color[2] *= -1.0;
		else if (lit4_diffuse[2] < 0.1)
			fog_color[2] *= -1.0;
	}
	else {
		fog_color[0] = 0.15;
		fog_color[1] = 0.20;
		fog_color[2] = 0.20; 
	}
	bld_ang += 5.0;
	lit4_diffuse[0] += lit_color4[0];
	lit4_diffuse[1] += lit_color4[1];
	lit4_diffuse[2] += lit_color4[2];
	lit5_diffuse[0] += lit_color5[0];
	lit5_diffuse[1] += lit_color5[1];
	lit5_diffuse[2] += lit_color5[2];

	if (bld_ang>360.0) {

		bld_ang -= 360.0;
	}
	if (lit4_diffuse[0]>1.0)
		lit_color4[0] *= -1.0;
	else if (lit4_diffuse[0]<0.1)
		lit_color4[0] *= -1.0;

	if (lit4_diffuse[1]>1.0)
		lit_color4[1] *= -1.0;
	else if (lit4_diffuse[1]<0.1)
		lit_color4[1] *= -1.0;

	if (lit4_diffuse[2]>1.0)
		lit_color4[2] *= -1.0;
	else if (lit4_diffuse[2]<0.1)
		lit_color4[2] *= -1.0;

	if (lit5_diffuse[0]>1.0)
		lit_color5[0] *= -1.0;
	else if (lit5_diffuse[0]<0.1)
		lit_color5[0] *= -1.0;

	if (lit5_diffuse[1]>1.0)
		lit_color5[1] *= -1.0;
	else if (lit5_diffuse[1]<0.1)
		lit_color5[1] *= -1.0;

	if (lit5_diffuse[2]>1.0)
		lit_color5[2] *= -1.0;
	else if (lit5_diffuse[2]<0.1)
		lit_color5[2] *= -1.0;
	setup_light();
	setup_fog();
	display(); /* show the scene again */
}

void my_reshape(int w, int h)
{

	width = w;
	height = h;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w>h)
		glOrtho(-40.0, 40.0, -40.0*(float)h / (float)w, 40.0*(float)h / (float)w,
			-100.0, 100.0);
	else
		glOrtho(-40.0*(float)w / (float)h, 40.0*(float)w / (float)h, -40.0, 40.0,
			-100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void my_quit(unsigned char key, int ix, int iy)
{
	double m, n;
	int    i;
	float  x[3], y[3], z[3];
	if (key == 'q') {
		style = 0;
		self_ang = -90.0; glob_ang = 0.0; up_ang = 0.0; bld_ang = 10.0;
		float  positions[3] = { 0.0, 0.0, 0.0 };
		float temps[3] = { 0.0,0.0,0.0 };
		double  Eyes[3] = { 0.0, 0.0, 30.0 }, Focuss[3] = { 0.0, 0.0, 0.0 },
			Vups[3] = { 0.0, 1.0, 0.0 };
		float   us[3][3] = { { 1.0,0.0,0.0 },{ 0.0,1.0,0.0 },{ 0.0,0.0,1.0 } };
		float Fog_color[] = { 0.15, 0.20, 0.20, 0.50 };
		for (int i = 0; i < 3; i++) {
			position[i] = positions[i];
			temp[i] = temps[i];
			Eye[i] = Eyes[i];
			Focus[i] = Focuss[i];
			Vup[i] = Vups[i];
			fog_color[i] = Fog_color[i];
			for (int j = 0; j < 3; j++)
				u[i][j] = us[i][j];
		}
		setup_fog();
		zoom = 1.0;
		eyeDx = 0.0; eyeDy = 0.0; eyeDz = 0.0;
		eyeAngx = 0.0; eyeAngy = 0.0; eyeAngz = 0.0;
		eye[0] = Eye[0];
		eye[1] = Eye[1];
		eye[2] = Eye[2];
	}
	if (key == 'Q') exit(0);
	if (key == '0') {
		trun0++;
		if (trun0 % 2 != 0)	glEnable(GL_LIGHT0);
		else	glDisable(GL_LIGHT0);
	}
	if (key == '1') {
		trun1++;
		if (trun1 % 2 != 0)	glEnable(GL_LIGHT1);
		else	glDisable(GL_LIGHT1);
	}
	if (key == '3') {
		trun23++;
		if (trun23 % 2 != 0) {
			glEnable(GL_LIGHT2);
			glEnable(GL_LIGHT3);
		}
		else {
			glDisable(GL_LIGHT2);
			glDisable(GL_LIGHT3);
		}
	}
	if (key == '5') {
		trun45++;
		if (trun45 % 2 != 0) {
			glEnable(GL_LIGHT4);
			glEnable(GL_LIGHT5);
		}
		else {
			glDisable(GL_LIGHT4);
			glDisable(GL_LIGHT5);
		}
	}
	if (key == '9') position[1] += 1.0;
	if (key == '8') {
		position[0] += Step*cos(self_ang*PI / 180.0);
		position[2] -= Step*sin(self_ang*PI / 180.0);
		if (50 >= (position[0] + 5) && (position[0] + 5) >= 10) {
			if (40 >= (position[2] + 8) && (position[2] + 8) >= 17.5) {
				up_ang = 40;
				m = position[2] - 20;
				position[1] = m * 2 / 3;
			}
			else {
				up_ang = 0.0;
				position[1] = 0.0;
			}
		}
		else if (50 >= (position[0] + 5) && (position[0] + 5) >= 10) {
			if (40 >= (position[2] - 8) && (position[2] - 8) >= 17.5) {
				up_ang = 40;
				m = position[2] - 20;
				position[1] = m * 2 / 3;
			}
			else {
				up_ang = 0.0;
				position[1] = 0.0;
			}
		}
		else if (50 >= (position[0] - 5) && (position[0] - 5) >= 10) {
			if (40 >= (position[2] - 8) && (position[2] - 8) >= 17.5) {
				up_ang = 40;
				m = position[2] - 20;
				position[1] = m * 2 / 3;
			}
			else {
				up_ang = 0.0;
				position[1] = 0.0;
			}
		}
		else if (50 >= (position[0] - 5) && (position[0] - 5) >= 10) {
			if (40 >= (position[2] + 8) && (position[2] + 8) >= 17.5) {
				up_ang = 40;
				m = position[2] - 20;
				position[1] = m * 2 / 3;
			}
			else {
				up_ang = 0.0;
				position[1] = 0.0;
			}
		}
		else {
			up_ang = 0.0;
			position[1] = 0.0;
		}

		if (-20.0 >= (position[0] + 5) && (position[0] + 5) >= -35.0) {
			if (-20 >= (position[2] + 8) && (position[2] + 8) >= -35) {
				position[0] = temp[0];
				position[2] = temp[2];
			}
			else {
				temp[0] = position[0];
				temp[2] = position[2];
			}
		}
		else if (-20.0 >= (position[0] + 5) && (position[0] + 5) >= -35.0) {
			if (-20 >= (position[2] - 8) && (position[2] - 8) >= -35) {
				position[0] = temp[0];
				position[2] = temp[2];
			}
			else {
				temp[0] = position[0];
				temp[2] = position[2];
			}
		}
		else if (-20.0 >= (position[0] - 5) && (position[0] - 5) >= -35.0) {
			if (-20 >= (position[2] + 8) && (position[2] + 8) >= -35) {
				position[0] = temp[0];
				position[2] = temp[2];
			}
			else {
				temp[0] = position[0];
				temp[2] = position[2];
			}
		}
		else if (-20.0 >= (position[0] - 5) && (position[0] - 5) >= -35.0) {
			if (-20 >= (position[2] - 8) && (position[2] - 8) >= -35) {
				position[0] = temp[0];
				position[2] = temp[2];
			}
			else {
				temp[0] = position[0];
				temp[2] = position[2];
			}
		}
		else {
			temp[0] = position[0];
			temp[2] = position[2];
		}
	}
	else if (key == '2') {
		position[0] -= Step*cos(self_ang*PI / 180.0);
		position[2] += Step*sin(self_ang*PI / 180.0);
		if (50 >= (position[0] + 5) && (position[0] + 5) >= 10) {
			if (40 >= (position[2] + 8) && (position[2] + 8) >= 17.5) {
				up_ang = 40;
				m = position[2] - 20;
				position[1] = m * 2 / 3;
			}
			else {
				up_ang = 0.0;
				position[1] = 0.0;
			}
		}
		else if (50 >= (position[0] + 5) && (position[0] + 5) >= 10) {
			if (40 >= (position[2] - 8) && (position[2] - 8) >= 17.5) {
				up_ang = 40;
				m = position[2] - 20;
				position[1] = m * 2 / 3;
			}
			else {
				up_ang = 0.0;
				position[1] = 0.0;
			}
		}
		else if (50 >= (position[0] - 5) && (position[0] - 5) >= 10) {
			if (40 >= (position[2] - 8) && (position[2] - 8) >= 17.5) {
				up_ang = 40;
				m = position[2] - 20;
				position[1] = m * 2 / 3;
			}
			else {
				up_ang = 0.0;
				position[1] = 0.0;
			}
		}
		else if (50 >= (position[0] - 5) && (position[0] - 5) >= 10) {
			if (40 >= (position[2] + 8) && (position[2] + 8) >= 17.5) {
				up_ang = 40;
				m = position[2] - 20;
				position[1] = m * 2 / 3;
			}
			else {
				up_ang = 0.0;
				position[1] = 0.0;
			}
		}
		else {
			up_ang = 0.0;
			position[1] = 0.0;
		}


		if (-20.0 >= (position[0] + 5) && (position[0] + 5) >= -35.0) {
			if (-20 >= (position[2] + 8) && (position[2] + 8) >= -35) {
				position[0] = temp[0];
				position[2] = temp[2];
			}
			else {
				temp[0] = position[0];
				temp[2] = position[2];
			}
		}
		else if (-20.0 >= (position[0] + 5) && (position[0] + 5) >= -35.0) {
			if (-20 >= (position[2] - 8) && (position[2] - 8) >= -35) {
				position[0] = temp[0];
				position[2] = temp[2];
			}
			else {
				temp[0] = position[0];
				temp[2] = position[2];
			}
		}
		else if (-20.0 >= (position[0] - 5) && (position[0] - 5) >= -35.0) {
			if (-20 >= (position[2] + 8) && (position[2] + 8) >= -35) {
				position[0] = temp[0];
				position[2] = temp[2];
			}
			else {
				temp[0] = position[0];
				temp[2] = position[2];
			}
		}
		else if (-20.0 >= (position[0] - 5) && (position[0] - 5) >= -35.0) {
			if (-20 >= (position[2] - 8) && (position[2] - 8) >= -35) {
				position[0] = temp[0];
				position[2] = temp[2];
			}
			else {
				temp[0] = position[0];
				temp[2] = position[2];
			}
		}
		else {
			temp[0] = position[0];
			temp[2] = position[2];
		}
	}
	else if (key == '4') {
		self_ang += 10.0;
	}
	else if (key == '6') {
		self_ang -= 10.0;
	}

	/*------transform the EYE coordinate system ------*/

	else if (key == ')') {
		style = 0;
	}

	else if (key == '!') {
		style = 1;
	}

	else if (key == '@') {
		style = 2;
	}

	else if (key == '#') {
		style = 3;
	}

	else if (key == '$') {
		style = 4;
	}

	else if (key == '-') {
		zoom += 0.1;
	}

	else if (key == '+') {
		zoom -= 0.1;
	}

	else if (key == 'u') {
		eyeDy += 0.5;       /* move up */
		for (i = 0; i<3; i++) eye[i] -= 0.5*u[1][i];
	}
	else if (key == 'd') {
		eyeDy += -0.5;       /* move down */
		for (i = 0; i<3; i++) eye[i] += 0.5*u[1][i];
	}
	else if (key == 'w') {
		eyeDx += -0.5;       /* move left */
		for (i = 0; i<3; i++) eye[i] += 0.5*u[0][i];
	}
	else if (key == 'e') {
		eyeDx += 0.5;        /* move right */
		for (i = 0; i<3; i++) eye[i] -= 0.5*u[0][i];
	}
	else if (key == 'n') {
		eyeDz += 0.5;        /* zoom in */
		for (i = 0; i<3; i++) eye[i] -= 0.5*u[2][i];
	}
	else if (key == 'f') {
		eyeDz += -0.5;       /* zoom out */
		for (i = 0; i<3; i++) eye[i] += 0.5*u[2][i];
	}
	else if (key == 'p') {             /* pitching */
		eyeAngx += 5.0;
		if (eyeAngx > 360.0) eyeAngx -= 360.0;
		y[0] = u[1][0] * cv - u[2][0] * sv;
		y[1] = u[1][1] * cv - u[2][1] * sv;
		y[2] = u[1][2] * cv - u[2][2] * sv;

		z[0] = u[2][0] * cv + u[1][0] * sv;
		z[1] = u[2][1] * cv + u[1][1] * sv;
		z[2] = u[2][2] * cv + u[1][2] * sv;

		for (i = 0; i<3; i++) {
			u[1][i] = y[i];
			u[2][i] = z[i];
		}
	}
	else if (key == 'P') {
		eyeAngx += -5.0;
		if (eyeAngx<0.0) eyeAngx += 360.0;
		y[0] = u[1][0] * cv + u[2][0] * sv;
		y[1] = u[1][1] * cv + u[2][1] * sv;
		y[2] = u[1][2] * cv + u[2][2] * sv;

		z[0] = u[2][0] * cv - u[1][0] * sv;
		z[1] = u[2][1] * cv - u[1][1] * sv;
		z[2] = u[2][2] * cv - u[1][2] * sv;

		for (i = 0; i<3; i++) {
			u[1][i] = y[i];
			u[2][i] = z[i];
		}
	}
	else if (key == 'h') {            /* heading */
		eyeAngy += 5.0;
		if (eyeAngy>360.0) eyeAngy -= 360.0;
		for (i = 0; i<3; i++) {
			x[i] = cv*u[0][i] - sv*u[2][i];
			z[i] = sv*u[0][i] + cv*u[2][i];
		}
		for (i = 0; i<3; i++) {
			u[0][i] = x[i];
			u[2][i] = z[i];
		}
	}
	else if (key == 'H') {
		eyeAngy += -5.0;
		if (eyeAngy<0.0) eyeAngy += 360.0;
		for (i = 0; i<3; i++) {
			x[i] = cv*u[0][i] + sv*u[2][i];
			z[i] = -sv*u[0][i] + cv*u[2][i];
		}
		for (i = 0; i<3; i++) {
			u[0][i] = x[i];
			u[2][i] = z[i];
		}
	}
	else if (key == 'r') {            /* rolling */
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
	else if (key == 'R') {
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

	else if (key == 't') 	glDisable(GL_FOG);
	else if (key == 'T') 	glEnable(GL_FOG);
	else if (key == 'o')	turn_fog = 0;
	else if (key == 'O')	turn_fog = 1;
	else if (key == 'l')   glFogi(GL_FOG_MODE, GL_LINEAR);
	else if (key == 'j')	glFogi(GL_FOG_MODE, GL_EXP);
	else if (key == 'J')	glFogi(GL_FOG_MODE, GL_EXP2);
	else if (key == 'i') {
		fog_opacity += 2;
		setup_fog();
	}
	else if (key == 'I') {
		fog_opacity -= 2;
		setup_fog();
	}
	display();
}

void main(int argc, char **argv)
{
	/*-----Initialize the GLUT environment-------*/
	glutInit(&argc, argv);
	/*-----Depth buffer is used, be careful !!!----*/
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow("HW5");
	myinit();      /*---Initialize other state varibales----*/
				   /*----Associate callback func's whith events------*/
	glutDisplayFunc(display);
	/*  glutIdleFunc(display); */
	glutIdleFunc(idle_func);
	glutReshapeFunc(my_reshape);
	glutKeyboardFunc(my_quit);

	glutMainLoop();
}

/*------------------------------------------
* Procedure to compute norm of vector v[]
*/
float norm2(float v[])
{
	double  sum;
	float   temp;

	sum = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
	temp = (float)sqrt(sum);
	return temp;
}
