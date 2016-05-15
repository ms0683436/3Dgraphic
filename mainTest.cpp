#include <string>
#include <iostream>
#include <fstream>

#include <GL\glew.h>	// 依個人路徑include
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
	// 告訴OpenGL我們要創的是哪種shader 
	unsigned int ShaderID;
	ShaderID = glCreateShader(type);
	// 把std::string結構轉換成const char* 
	const char* csource = source.c_str();
	// 把程式碼放進去剛剛創建的shader object中 
	glShaderSource(ShaderID, 1, &csource, NULL);
	// 編譯shader 
	glCompileShader(ShaderID);
	// 這是編譯過程的訊息, 錯誤什麼的把他丟到error裡面 
	char error[1000] = "";
	glGetShaderInfoLog(ShaderID, 1000, NULL, error);
	// 然後輸出出來 
	std::cout << "Complie status: \n" << error << std::endl;

	return ShaderID;
}
// 用來儲存shader還有program的id
unsigned int vs, fs, program;

void initShader(const char* vname, const char* fname)
{
	std::string source;

	// 把程式碼讀進source
	loadFile(vname, source);
	// 編譯shader並且把id傳回vs
	vs = loadShader(source, GL_VERTEX_SHADER);
	source = "";
	loadFile(fname, source);
	fs = loadShader(source, GL_FRAGMENT_SHADER);

	// 創建一個program
	program = glCreateProgram();
	// 把vertex shader跟program連結上
	glAttachShader(program, vs);
	// 把fragment shader跟program連結上
	glAttachShader(program, fs);
	// 根據被連結上的shader, link出各種processor
	glLinkProgram(program);
	// 然後使用它
	glUseProgram(program);
}

// 回收資源
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