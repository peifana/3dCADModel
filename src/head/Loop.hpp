#pragma once
#include"HalfEdge.hpp"
class Face;
class Loop {
	HalfEdge* firstHalfEdge=nullptr;
	Loop* nextloop=nullptr;
	Face* face=nullptr;
	Vertex* onlyVertex = nullptr;
	Loop(HalfEdge* firstHalfEdge) :firstHalfEdge(firstHalfEdge) {};
	Loop(Vertex* v) :onlyVertex(v) {};
};