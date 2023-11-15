	#pragma once
#include"Face.hpp"
class Solid {
	Face* firstFace = nullptr;
	Solid* nextSolid = nullptr;
	Solid(Face* firstFace) :firstFace(firstFace) {};
};