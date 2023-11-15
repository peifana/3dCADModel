#pragma once
#include<vector>
#include<stack>
class Point {
public:
	double x, y, z;
public:
	Point(double x, double y, double z) :x(x), y(y), z(z) {};
	Point() :x(0), y(0), z(0) {};
	bool  operator==(const Point p) const {
		return x == p.x && y == p.y && z == p.z;
	}
};


class Vertex {
	Point p;
public:
	Vertex(Point p) :p(p) {};
	friend bool operator==(const Vertex* v, const Point p);
};
bool operator==(const Vertex* v, const Point p) {
	return v->p == p;
}

class Edge;
class Loop;
class HalfEdge {
	Edge* belongEdge=nullptr;
	Vertex* begVertex, * endVertex;
	HalfEdge* nextEdge;
	Loop* belongLoop=nullptr;
public:
	HalfEdge(Vertex* beg, Vertex* end, Edge* e)
		:begVertex(beg), endVertex(end), belongEdge(e){};
	void ChangeNextEdge(HalfEdge* hal) {nextEdge = hal;}
	void MarkBelongLoop(Loop* l) {belongLoop = l;}
	void MarkBelongEdge(Edge* edge) {belongEdge = edge;}
	Vertex* getBeg() {return begVertex;}
	Vertex* getEnd() {return endVertex;}
	HalfEdge* getNextHalfEdge() {return nextEdge;}
	bool HasBelongLoop() { return belongLoop != nullptr; }
	void Reverse() {
		Vertex* temp = begVertex;
		begVertex = endVertex;
		endVertex = temp;
	}
};
class Edge {
public:
	HalfEdge* half1, *half2;
public:
	Edge(Vertex* v1, Vertex* v2) {
		half1 = new HalfEdge(v1, v2,this);
		half2 = new HalfEdge(v2, v1,this);
	}
};
class Face;
class Loop {
	HalfEdge* firstHalfEdge = nullptr,*lastHalfEdge=nullptr;
	Loop* nextloop = nullptr;
	Face* belongFace = nullptr;
	Vertex* onlyVertex=nullptr;
	bool finishedBuild = false;
public:
	Loop(HalfEdge* firstHalfEdge) :firstHalfEdge(firstHalfEdge) {
		firstHalfEdge->MarkBelongLoop(this);
	};
	Loop(Vertex* v) :onlyVertex(v) {};
	void MarkbelongFace(Face* f) {
		belongFace = f;
	}
	Loop() {};
	void AddHalfEdge(HalfEdge* half) {
		if (finishedBuild) {
			std::cout << "the loop has finished building " << std::endl;
			return;
		}
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
		if (firstHalfEdge->getBeg() == lastHalfEdge->getEnd()) {
			finishedBuild = true;
		}
		half->MarkBelongLoop(this);
	}
	bool IsFinished() {
		return finishedBuild;
	}
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
	Loop* GetouterLoop() { return outerLoop; }
	void ChangeouterLoop(Loop* l) {
		if (!l->IsFinished()) {
			std::cout << "do not put an unfinished loop as a outer loop of the face" << std::endl;
			return;
		}
		outerLoop = l;
		l->MarkbelongFace(this);
	}
	void ChangeInnerLoop(Loop* l) {
		if (!l->IsFinished()) {
			std::cout << "do not put an unfinished loop as an inner loop of the face" << std::endl;
			return;
		}
		innerLoop = l;
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
	void AddFace(Face* face) {
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
	void AddVertex(Vertex* v) {
		VertexList.emplace_back(v);
	}
	Vertex* FindVertex(Point p) {
		for (auto v : VertexList) {
			if (v == p)return v;
		}
		return nullptr;
	}
	void AddEdge(Edge* e) {
		edgeList.emplace_back(e);
	}
	Face* FindFace(int faceId) {
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
	void Mvsf(Point p);
	void Mev(Point p1,Point p2);
	void Mef1(Point p1, Point p2);//normal mef
	void Mef2(Point p1, Point p2);//the last one who finish two faces
	void FinishBuildingLoop(HalfEdge* half);
	void Kemr(int FaceId);
	void Kfmrh(int deletefaceId, int outfaceId);
};




void Loop::Reverse() {
	std::stack<HalfEdge*>sta;
	auto half = firstHalfEdge;
	while (half) {
		half->Reverse();
		sta.push(half);
	}
	lastHalfEdge = firstHalfEdge;
	firstHalfEdge = sta.top();
	sta.pop();
	auto half2 = firstHalfEdge;
	while (!sta.empty()) {
		half2->ChangeNextEdge(sta.top());
		half2 = sta.top();
		sta.pop();
	}
}