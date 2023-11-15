#pragma once
#include<vector>
#include<stack>
#include<iostream>
class Point {
public:
	double x, y, z;
public:
	Point(double x, double y, double z) :x(x), y(y), z(z) {};
	Point() :x(0), y(0), z(0) {};
	bool  operator==(const Point p) const {
		return x == p.x && y == p.y && z == p.z;
	}
	Point operator+(const Point p)const {
		return Point(p.x + x, p.y + y, p.z + z);
	}
};


class Vertex {
	Point p;
public:
	Vertex(Point p) :p(p) {}
	Vertex() {}
	Point Getpos(){return p;}
	void Print() {
		std::cout << "(" << p.x << ',' << p.y << ',' << p.z << ")" << std::endl;
	}
	friend bool operator==(const Vertex* v, const Point p);
	friend Vertex* operator+(const Vertex* v, const Point p);
	bool operator==(const Vertex* v1) {
		return v1 == p;
	}
};
class Edge;
class Loop;
class HalfEdge {
	Edge* belongEdge=nullptr;
	HalfEdge* brotherHalfEdge = nullptr;
	Vertex* begVertex, * endVertex;
	HalfEdge* nextEdge=nullptr;
	Loop* belongLoop=nullptr;
public:
	HalfEdge(Vertex* beg, Vertex* end, Edge* e)
		:begVertex(beg), endVertex(end), belongEdge(e){};
	void ChangeNextEdge(HalfEdge* hal) {nextEdge = hal;}
	void MarkBelongLoop(Loop* l) {belongLoop = l;}
	void MarkBelongEdge(Edge* edge) {belongEdge = edge;}
	Edge* GetBelongEdge() { return belongEdge; }
	void MarkBrotherHalfEdge(HalfEdge* h) { brotherHalfEdge = h;}
	Vertex* getBeg() {return begVertex;}
	Vertex* getEnd() {return endVertex;}
	HalfEdge* getNextHalfEdge() {return nextEdge;}
	bool HasBelongLoop() { return belongLoop != nullptr; }
	HalfEdge* Reverse() {
		brotherHalfEdge->ChangeNextEdge(nextEdge);
		brotherHalfEdge->MarkBelongLoop(belongLoop);
		ChangeNextEdge(nullptr);
		MarkBelongLoop(nullptr);
		return brotherHalfEdge;
	}
};
class Edge {
public:
	HalfEdge* half1, * half2;
public:
	Edge(Vertex* v1, Vertex* v2) {
		half1 = new HalfEdge(v1, v2, this);
		half2 = new HalfEdge(v2, v1, this);
		half1->MarkBrotherHalfEdge(half2);
		half2->MarkBrotherHalfEdge(half1);
	}
	~Edge() {
		delete half1;
		delete half2;
	}
};
class Face;
class Loop {
	HalfEdge* firstHalfEdge = nullptr,*lastHalfEdge=nullptr;
	Loop* nextloop = nullptr;
	Face* belongFace = nullptr;
	Vertex* onlyVertex=nullptr;
	Loop* nextInnerLoop=nullptr;
public:
	Loop(HalfEdge* firstHalfEdge) :firstHalfEdge(firstHalfEdge) {
		firstHalfEdge->MarkBelongLoop(this);
	};
	Loop(Vertex* v) :onlyVertex(v) {};
	void MarkbelongFace(Face* f) {
		belongFace = f;
	}
	Loop() {};
	HalfEdge* GetFirstHalfEdge() { return firstHalfEdge; }
	HalfEdge* GetLastHalfEdge() { return lastHalfEdge; }
	Loop* GetNextInnerLoop() { return nextInnerLoop; }
	void AddNextInnerLoop(Loop* loop) {
		if (nextInnerLoop) {
			nextInnerLoop->AddNextInnerLoop(loop);
		}
		else nextInnerLoop = loop;
	}
	void AddHalfEdge(HalfEdge* half) {
		if (!firstHalfEdge) {
			firstHalfEdge = half;
			lastHalfEdge = half;
		}
		else if (lastHalfEdge->getEnd() != half->getBeg())
			std::cout << "Wrong in AddHalfEdge() of Loop,you may don't follow the order" << std::endl;
		else {
			lastHalfEdge->ChangeNextEdge(half);
			lastHalfEdge = half;
		}
		half->MarkBelongLoop(this);
	}
	void Print();
	bool IsFinished() {return firstHalfEdge->getBeg()==lastHalfEdge->getEnd();}
	void Kemr();
	void Reverse();
};


class Solid;
class Face {
	Loop* outerLoop = nullptr, * innerLoop = nullptr;
	Solid* belongSolid = nullptr;
	Face* preFace = nullptr,*nextFace=nullptr;
public:
	Face(Loop* outerloop) :outerLoop(outerLoop) {
		outerLoop->MarkbelongFace(this);
	};
	Face() {};
	void MarkbelongSolid(Solid* s) {
		belongSolid = s;
	}
	void ChangePreFace(Face *face) {preFace = face;}
	void ChangeNextFace(Face* face) { nextFace = face; }
	Face* GetnextFace() { return nextFace; }
	Face* GetPreFace() { return preFace; }
	Loop* GetouterLoop() { return outerLoop; }
	Loop* GetInnerLoop() { return innerLoop; }
	void ChangeouterLoop(Loop* l) {
		if (!l->IsFinished()) {
			l->Print();
			std::cout << "do not put an unfinished loop as a outer loop of the face" << std::endl;
			return;
		}
		outerLoop = l;
		l->MarkbelongFace(this);
	}
	void ChangeInnerLoop(Loop* l) {
		if (!l->IsFinished()) {
			l->Print();
			std::cout << "do not put an unfinished loop as an inner loop of the face" << std::endl;
			return;
		}
		if (innerLoop)innerLoop->AddNextInnerLoop(l);
		else innerLoop = l;
		l->MarkbelongFace(this);
	}
};


class Solid {
	Face* firstFace = nullptr,*lastFace=nullptr;
	Solid* nextSolid = nullptr;
	Loop* buildingLoop = nullptr;

	std::vector<Vertex*> VertexList;
	std::vector<Edge*>edgeList;
public:
	Solid(Face* firstFace) :firstFace(firstFace) {
		firstFace->MarkbelongSolid(this);
	};
	Solid() {};

	Face* GetFirstFace() { return firstFace; }
	Solid* GetNextSolid() { return nextSolid; }

	void AddVertex(Vertex* v) { VertexList.emplace_back(v); }
	void AddEdge(Edge* e) { edgeList.emplace_back(e); }
	void AddFace(Face* face);

	Vertex* FindVertex(Point p);
	Face* FindFace(int faceId);

	void Mvsf(Point p);
	void Mev(Point p1,Point p2);
	void Mef1(Point p1, Point p2);//normal mef
	void Mef2(Point p1, Point p2);//the last one who finish two faces
	void FinishBuildingLoop();
	void Kemr(int FaceId);
	void Kfmrh(int deletefaceId, int outfaceId);
	void Print() {
		int i = 0;
		auto f = firstFace;
		while (f) {
			if (f->GetouterLoop()) {
				printf("outer loop of face %d :\n", i);
				f->GetouterLoop()->Print();
			}
			if (f->GetInnerLoop()) {
				printf("inner loop of face %d :\n", i);
				f->GetInnerLoop()->Print();
			}
			i++;
			f = f->GetnextFace();
		}
	}
	void Sweep(Point p);
};