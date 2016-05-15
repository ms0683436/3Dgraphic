#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>	// �̭ӤH���|include
#include <GL\glut.h>

#pragma comment(lib, "glew32.lib")


void loadFile(const char* filename, std::string &string)
{
	std::ifstream fp(filename);
	if (!fp.is_open()) {
		std::cout << "Open <" << filename << "> error." << std::endl;
		return;
	}

	char temp[300];
	while (!fp.eof()) {
		fp.getline(temp, 300);
		string += temp;
		string += '\n';
	}

	fp.close();
}

unsigned int loadShader(std::string &source, GLenum type)
{
	// �i�DOpenGL�ڭ̭n�Ъ��O����shader 
	unsigned int ShaderID;
	ShaderID = glCreateShader(type);
	// ��std::string���c�ഫ��const char* 
	const char* csource = source.c_str();
	// ��{���X��i�h���Ыت�shader object�� 
	glShaderSource(ShaderID, 1, &csource, NULL);
	// �sĶshader 
	glCompileShader(ShaderID);
	// �o�O�sĶ�L�{���T��, ���~���򪺧�L���error�̭� 
	char error[1000] = "";
	glGetShaderInfoLog(ShaderID, 1000, NULL, error);
	// �M���X�X�� 
	std::cout << "Complie status: \n" << error << std::endl;

	return ShaderID;
}
// �Ψ��x�sshader�٦�program��id
unsigned int vs, fs, program;

void initShader(const char* vname, const char* fname)
{
	std::string source;

	// ��{���XŪ�isource
	loadFile(vname, source);
	// �sĶshader�åB��id�Ǧ^vs
	vs = loadShader(source, GL_VERTEX_SHADER);
	source = "";
	loadFile(fname, source);
	fs = loadShader(source, GL_FRAGMENT_SHADER);

	// �Ыؤ@��program
	program = glCreateProgram();
	// ��vertex shader��program�s���W
	glAttachShader(program, vs);
	// ��fragment shader��program�s���W
	glAttachShader(program, fs);
	// �ھڳQ�s���W��shader, link�X�U��processor
	glLinkProgram(program);
	// �M��ϥΥ�
	glUseProgram(program);
}

// �^���귽
void clean()
{
	glDetachShader(program, vs);
	glDetachShader(program, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteProgram(program);
}

void glRenderScene()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, 400, 400);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 400 / 400.0, 0.1, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(20, 20, -20, 0, 0, 0, 0, 1, 0);

	glBegin(GL_QUADS);
	glVertex3f(10, 0, -10);
	glVertex3f(-10, 0, -10);
	glVertex3f(-10, 0, 10);
	glVertex3f(10, 0, 10);
	glEnd();

	glutSwapBuffers();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("glsl lesson 01");

	glewInit();

	glutDisplayFunc(glRenderScene);

	initShader("vertex.vs", "fragment.frag");

	glutMainLoop();

	clean();

	return 0;
}