#pragma once
#include"Elements.hpp"
#include<iostream>
 void Solid::Mvsf(Point p) {
    Vertex* v = new Vertex(p);
    Face* f = new Face();
    buildingLoop = new Loop();
    AddVertex(v);
    AddFace(f);
}
 void Solid::Mev(const Point p1, const Point p2) {//p1 is the old one,p2 is the new one
     Vertex* v1 =FindVertex(p1), * v2 = new Vertex(p2);
     AddVertex(v2);
     if (!v1) {
         std::cout << "Warning in Mev:the Point(" << p1.x << "," << p1.y << "," << p1.z << ")has not mentioned before" << std::endl;
     }
     Edge* e = new Edge(v1, v2);
     AddEdge(e);
     buildingLoop->AddHalfEdge(e->half1);
 }
 void Solid::Mef1(Point p1, Point p2) {
     Vertex* v1 = FindVertex(p1), * v2 = FindVertex(p2);
     Edge* e = new Edge(v1, v2);
     buildingLoop->AddHalfEdge(e->half1);
     lastFace->ChangeouterLoop(buildingLoop);
     buildingLoop = new Loop();
     Face* f = new Face();
     AddFace(f);
     AddEdge(e);
 }
 void Solid::Mef2(Point p1, Point p2) {
     Vertex* v1 = FindVertex(p1), * v2 = FindVertex(p2);
     Edge* e = new Edge(v1, v2);
     buildingLoop->AddHalfEdge(e->half1);
     lastFace->ChangeouterLoop(buildingLoop);
     buildingLoop = new Loop();
     Face* f = new Face();
     AddFace(f);
     FinishBuildingLoop(e->half2);
     lastFace->ChangeouterLoop(buildingLoop);
     buildingLoop = new Loop();
     AddEdge(e);
 }
 void Solid::FinishBuildingLoop(HalfEdge* half) {
     buildingLoop->AddHalfEdge(half);
     Vertex* firstVertex = half->getBeg();
     Vertex* lastVertex = half->getEnd();
     Vertex* lastLastVertex = nullptr;
         while (firstVertex!=lastVertex) {
             lastLastVertex = lastVertex;
             for (auto edge : edgeList) {
                 auto half1 = edge->half1, half2=edge->half2;
                 if (half1->getBeg() == lastVertex && (!half1->HasBelongLoop())) {
                     buildingLoop->AddHalfEdge(half1);
                     lastVertex = half1->getEnd();
                 
                 }
                 else if (half2->getBeg() == lastVertex && (!half1->HasBelongLoop())) {
                     buildingLoop->AddHalfEdge (half2);
                     lastVertex = half2->getEnd();
                 }
             }
             if (lastLastVertex == lastVertex) {
                 std::cout << "can't finish BuildLastFace(),something goes wrong " << std::endl;
                 break;
             }
         }
 }
 void Solid::Kemr(int faceId) {
     int i = 0;
     Face* f = FindFace(faceId);
     buildingLoop->Kemr();
     f->ChangeInnerLoop(buildingLoop);
     buildingLoop = new Loop();
 }
 void Loop::Kemr() {
     auto temp = firstHalfEdge;
     firstHalfEdge = firstHalfEdge->getNextHalfEdge();
     delete temp;
     Reverse();
 }
 void Solid::Kfmrh(int deletefaceId,int outfaceId) {
     int i = 0;
     Face* f1 = FindFace(deletefaceId);
     Face* f2 = FindFace(outfaceId);
     Loop* l = f2->GetouterLoop();
     l->Reverse();
     f2->ChangeInnerLoop(l);


 }
