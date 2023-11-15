#include"Elements.hpp"
void Solid::Sweep(Point p) {
	Face* f = lastFace->GetPreFace();//the last builded face
	Loop * l = f->GetouterLoop();
	//outer
	HalfEdge* half = l->GetFirstHalfEdge();
	while (half) {
		Vertex* v1 = half->getBeg(), * v2 = half->getEnd();
		Point p1 = v1->Getpos(), p2 = v2->Getpos();
		Mev(p1, p1+p);
		Mev(p1+p, p2+p);
		if (half->getNextHalfEdge()) {
			Mef1(p2+p, p2);
		}
		else Mef2(p2+p, p2);
	}
	//inner


}