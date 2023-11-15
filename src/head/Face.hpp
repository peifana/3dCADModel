#pragma once
#include"Loop.hpp"
class Solid;
class Face {
	Loop* outerLoop=nullptr, * innerLoop=nullptr;
	Solid* BelongSolid=nullptr;
	Face* nextFace=nullptr,*preFace=nullptr;
	Face(Loop* outerloop) :outerLoop(outerLoop) {};
};