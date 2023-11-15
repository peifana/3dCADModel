#pragma once
#include"Vertex.hpp"
class HalfEdge;
class Edge {
	HalfEdge* half1=nullptr, * half2=nullptr;
	Edge(HalfEdge* half1, HalfEdge* half2) :half1(half1), half2(half2) {};
};