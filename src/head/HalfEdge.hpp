#pragma once
#include"Vertex.hpp"
class Edge;
class Loop;
class HalfEdge {
	Edge* belongEdge;
	Vertex* begVertex, * endVertex;
	HalfEdge* nextEdge;
	Loop* belongLoop;
public:
	HalfEdge(Vertex* beg, Vertex* end, HalfEdge* next, Loop* loop,Edge* edge)
		:begVertex(beg), endVertex(end), nextEdge(next), belongLoop(loop),belongEdge(belongEdge) {};
	void ChangeNextEdge(HalfEdge* hal) {
		nextEdge = hal;
	}
};


class Edge {
	HalfEdge* half1 = nullptr, * half2 = nullptr;
	Edge(HalfEdge* half1, HalfEdge* half2) :half1(half1), half2(half2) {};
};