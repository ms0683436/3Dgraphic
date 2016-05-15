/***************************************************
 * Example program: Draw a cube and clip it with a 
 * user-defined clip plane.
 *   Author: S. K. Ueng
 *   National Taiwan Ocean Univ. Comp. Sci. Dept.
 */
#include <stdio.h>
#include <math.h>

#include <GL/glut.h>


#define   PI   3.1415927

float  points[][3] = {{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, 
                      {1.0, 0.0, 1.0}, {0.0, 0.0, 1.0},
                      {0.0, 1.0, 0.0}, {1.0, 1.0, 0.0},
{1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}};

int    face[][4] = {{0, 1, 2, 3}, {7, 6, 5, 4}, {0, 4, 5, 1}, 
                    {1, 5, 6, 2}, {3, 2, 6, 7}, {0, 3, 7, 4}};

int    cube[6] = {0, 1, 2, 3, 4, 5};

float  color[][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0},
                     {1, 1, 0}, {1, 0, 1}, {0, 1, 1}};


int    width=500, height=500;    /* window shape */

float  pos[3] = {4.0, 0.0, 4.0};
float  angle = 0.0;

//Define the coef. of the clip plane {A, B, C, D}
//The clip plane = A*x + B*y + C*z + D = 0.
double clip_equ[4] = {1.0, 1.0, 1.0, 5.0};
float  dz=0.0;
int    clipCond=0;
/*----------------------------------------------------------
 * Procedure to initialize the working environment.
 */
void  myinit()
{
  glClearColor(0.0, 0.0, 0.0, 1.0);      /*set the background color BLACK */
                   /*Clear the Depth & Color Buffers */
  glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

  glViewport(0, 0, 499, 499);

  /*-----Set a parallel projection mode-----*/
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-8.0, 8.0 , -8.0, 8.0, -10.0, 20.0);
  
  glEnable(GL_DEPTH_TEST);  /*Enable depth buffer for shading computing */
   
  /*----Set a clip plane. -----*/
  glClipPlane(GL_CLIP_PLANE0, clip_equ);
}


/*--------------------------------------------------------
 * Procedure to draw the cube. The geometrical data of the cube
 * are defined above.
 */
void draw_cube()
{
  int    i;


  for(i=0;i<6;i++){     /* draw the six faces one by one */
    glColor3fv(color[i]);  /* Set the current color */

    glBegin(GL_POLYGON);  /* Draw the face */
      glVertex3fv(points[face[i][0]]);
      glVertex3fv(points[face[i][1]]);
      glVertex3fv(points[face[i][2]]);
      glVertex3fv(points[face[i][3]]);
    glEnd();
  }
}

/*------------------------------------------------------------
 * Procedure to draw a floor of black-and-white tiles.
  */
void draw_floor()
{
  int   i,  j;


  for(i=0;i<10;i++)
    for(j=0;j<10;j++){
      if((i+j)%2==0) glColor3f(0.9, 0.9, 0.9);
      else glColor3f(0.1, 0.1, 0.1);
      glBegin(GL_POLYGON);
	glVertex3f(i, 0.0, j);
	glVertex3f(i, 0.0, j+1);
	glVertex3f(i+1, 0.0, j+1);
	glVertex3f(i+1, 0.0, j);
      glEnd();
    }
}



/*-------------------------------------------------------
 * The  Display callback function, drawing a cube and a floor
 * and clip the cube using a clip plane.
 */
void display()
{

  /*Clear previous frame and the depth buffer */
  glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

  /*----Define the current eye position and the eye-coordinate system---*/
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(6.0, 6.0, 6.0, 3.0, 0.0, 3.0, 0.0, 1.0, 0.0);

  /*-------Draw the floor ----*/
  glDisable(GL_CLIP_PLANE0);
  draw_floor();

  /*-------Draw the cube ----*/
  if(clipCond) 
    glEnable(GL_CLIP_PLANE0);
  else 
    glDisable(GL_CLIP_PLANE0);

  glTranslatef(pos[0],pos[1], pos[2]);
  glPushMatrix();
  glScalef(4.0, 4.0, 4.0);
  draw_cube();
  glPopMatrix();

  /*-------Swap the back buffer to the front --------*/
  glutSwapBuffers();
  glFlush(); /*--Display the results----*/
}


/*--------------------------------------------------
 * Reshape callback function which defines the size
 * of the main window when a reshape event occurrs.
 */
void my_reshape(int w, int h)
{


  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if(w>h)
    glOrtho(-10.0, 10.0, -10.0*(float)h/(float)w, 10.0*(float)h/(float)w, 
             -10.0, 30.0);
  else
    glOrtho(-10.0*(float)w/(float)h, 10.0*(float)w/(float)h, -10.0, 10.0, 
            -10.0, 30.0);
  width = w; height = h;
}


/*--------------------------------------------------
 * Keyboard callback func. When a 'q' key is pressed,
 * the program is aborted.
 */
void my_quit(unsigned char key, int x, int y)
{
  if(key=='c'){
    clipCond = 1;
  }else if(key=='d'){
    clipCond = 0;
  }else if(key=='q') exit(0);
  else if(key=='f'){ //move clip_plane forward
    dz += 0.1;
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0, 0, dz);
   /*----Redefine the clip plane. -----*/
    glClipPlane(GL_CLIP_PLANE0, clip_equ);
    glEnable(GL_CLIP_PLANE0);
    glPopMatrix();
  }else if(key=='b'){ //move clip_plane backward
    dz -= 0.1;
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0, 0, dz);
   /*----Redefine the clip plane. -----*/
    glClipPlane(GL_CLIP_PLANE0, clip_equ);
    glEnable(GL_CLIP_PLANE0);
    glPopMatrix();
  }
  display();
}




/*---------------------------------------------------
 * Main procedure which defines the graphics environment,
 * the operation style, and the callback func's.
 */
void main(int argc, char **argv)
{
  /*-----Initialize the GLUT environment-------*/
  glutInit(&argc, argv);

  /*-----Depth buffer is used, be careful !!!----*/
  glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB| GLUT_DEPTH);

  glutInitWindowSize(500, 500);
  glutCreateWindow("cube");

  myinit();      /*---Initialize other state varibales----*/
  
  /*----Associate callback func's whith events------*/
  glutDisplayFunc(display);

  glutReshapeFunc(my_reshape);
  glutKeyboardFunc(my_quit);

  glutMainLoop();
}

