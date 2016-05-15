#ifndef GL_INTERFACE_H
#define GL_INTERFACE_H
void Init();

void Display();
void Mouse(int button,int state,int x,int y);
void Motion(int x,int y);
void MouseWheel(int wheel, int direction, int x, int y);
void Timer(int c);
void KeyboardDown(unsigned char c,int x,int y);
void KeyboardUp(unsigned char c,int x,int y);
void idle_func();
#endif