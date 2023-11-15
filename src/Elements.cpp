#include"Elements.hpp"
bool operator==(const Vertex* v, const Point p) {
	return v->p == p;
}
Vertex* operator+(const Vertex* v, const Point p) {
	Vertex* v2 = new Vertex(v->p + p);
	return v2;
}
void Loop::Print() {
	HalfEdge* half = firstHalfEdge;
	while (half) {
		half->getBeg()->Print();
		if (!half->getNextHalfEdge())half->getEnd()->Print();
		half = half->getNextHalfEdge();
	}



}
void Loop::Reverse() {
	std::stack<HalfEdge*>sta;
	auto half = firstHalfEdge;
	while (half) {
		auto nexthalf = half->getNextHalfEdge();
		half=half->Reverse();
		sta.push(half);
		half = nexthalf;

	}
	firstHalfEdge = sta.top();
	sta.pop();
	auto half2 = firstHalfEdge;
	while (!sta.empty()) {
		half2->ChangeNextEdge(sta.top());
		half2 = sta.top();
		sta.pop();
	}
	half2->ChangeNextEdge(nullptr);
	lastHalfEdge = half2;
}
void Solid::AddFace(Face* face) {
	Face* temp = lastFace;
	lastFace = face;
	lastFace->ChangePreFace(temp);
	if (temp) {
		temp->ChangeNextFace(face);
	}
	if (!firstFace) {
		firstFace = lastFace;
	}
}
Vertex* Solid::FindVertex(Point p) {
	for (auto v : VertexList) {
		if (v == p)return v;
	}
	return nullptr;
}
Face* Solid::FindFace(int faceId) {
	if (faceId == -1)return lastFace;
	else {
		int i = 0;
		Face* f = firstFace;
		while (i != faceId) {
			f = f->GetnextFace();
			i += 1;
		}
		return f;
	}
}
