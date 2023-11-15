#pragma once
#include"Elements.hpp"
#define CALLBACK    __stdcall
void Draw(int argc, char** argv, Solid* firstSolid);
void KeyBoards(unsigned char key, int x, int y);
void MouseFunc(int button, int state, int x, int y);
void ActiveMotionFunc(int x, int y);
void Display();
class Camera {
	float eyex = 0, eyey = 0, eyez = 10;
	float centerx = 0, centery = 0, centerz = 0;
	float upx = 1, upy = 0, upz = 0;
	friend void Draw(int argc, char** argv, Solid* firstSolid);
	friend void MouseFunc(int button, int state, int x, int y);
};