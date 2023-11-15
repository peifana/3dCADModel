#pragma once
#include"Elements.hpp"
Solid* BuildSolid() {
	//build basic cube
	Point p1(-1, -1,-1), p2(1,-1, -1), p3(1, 1, -1), p4(-1, 1, -1);
	Point p5(-1, -1, 1), p6(1,-1, 1), p7(1, 1, 1), p8(-1, 1, 1);
	//dig a hole
	Point p11(-1.0/2, -1.0/2, -1), p12(1.0/2, -1.0/2, -1), p13(1.0/2, 1.0/2, -1), p14(-1.0/2, 1.0/2, -1);
	Point p15(-1.0/2, -1.0/2, 1), p16(1.0/2, -1.0/2, 1), p17(1.0/2, 1.0/2, 1), p18(-1.0/2, 1.0/2, 1);
	Solid* s = new Solid();
	s->Mvsf(p1);
	s->Mev(p1, p2);
	s->Mev(p2, p3);
	s->Mev(p3, p4);
	s->Mef1(p4, p1);
	
	s->Mev(p2, p6);
	s->Mev(p6, p7);
	s->Mef1(p7, p3);

	s->Mev(p7, p8);
	s->Mef1(p8, p4);
	
	s->Mev(p8, p5);
	s->Mef1(p5, p1);

	s->Mef2(p5, p6);
	
	s->Mev(p1, p11);
	s->Mev(p11, p12);
	s->Mev(p12, p13);
	s->Mev(p13, p14);
	s->Mef1(p14, p11);
	s->Kemr(0);
	s->Mev(p13, p17);
	s->Mev(p17, p16);
	s->Mef1(p16, p12);

	s->Mev(p14, p18);
	s->Mef1(p18, p17);

	s->Mev(p11, p15);
	s->Mef1(p15, p18);

	s->Mef2(p16, p15);
	s->Kfmrh(-1, 5);


	return s;
}