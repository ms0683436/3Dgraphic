/*------------------------------------------------------------
 * The main program of Phong shading rendering.
 * See http://glew.sourceforge.net/basic.html for the details.
 *   Initialize glut and glew.
 *   Load & setup the shaders.
 *   Initialize the program.
 */
#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include<GL/freeglut.h>
#include "GLInterface.h"
#include "GLSLSetup.h"


//The C-language main procedure
int main(int argc,char** argv)
{
 
	//Initialize glut context.
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitWindowSize(1200,1200);
	glutCreateWindow("GLSL PhongShading");

		glutDisplayFunc(Display);
		glutKeyboardFunc(KeyboardDown);
		glutKeyboardUpFunc(KeyboardUp);
		glutMotionFunc(Motion);
		glutMouseFunc(Mouse);
		glutMouseWheelFunc(MouseWheel);
		glutTimerFunc(50,Timer,0);
	/*Initialize GL glew extension*/
	glewInit();
    //Check the global variables to make sure glew is supported.
	Init();
	glutMainLoop();
}