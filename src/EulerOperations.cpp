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
     if(!(buildingLoop->GetFirstHalfEdge()->getEnd()== v2) )
        FinishBuildingLoop();
     if (buildingLoop->IsFinished()) {
         lastFace->ChangeouterLoop(buildingLoop);
         buildingLoop = new Loop();
     }
      Face* f = new Face();
      AddFace(f);
      AddEdge(e);
 }
 void Solid::Mef2(Point p1, Point p2) {
     Vertex* v1 = FindVertex(p1), * v2 = FindVertex(p2);
     Edge* e = new Edge(v1, v2);
     buildingLoop->AddHalfEdge(e->half1);
     FinishBuildingLoop();
     lastFace->ChangeouterLoop(buildingLoop);
     buildingLoop = new Loop();
     Face* f = new Face();
     AddFace(f);
     buildingLoop->AddHalfEdge(e->half2);
     FinishBuildingLoop();
     lastFace->ChangeouterLoop(buildingLoop);
     buildingLoop = new Loop();
     AddEdge(e);
 }
 void Solid::FinishBuildingLoop() {
     Vertex* firstVertex = buildingLoop->GetFirstHalfEdge()->getBeg();
     Vertex* lastVertex = buildingLoop->GetLastHalfEdge()->getEnd();
     Vertex* lastLastVertex = nullptr;
         while (firstVertex!=lastVertex) {
             lastLastVertex = lastVertex;
             for (auto edge : edgeList) {
                 if (lastVertex == firstVertex)break;
                 auto half1 = edge->half1, half2 = edge->half2;
                 if (half2->getBeg() == lastVertex && (!half2->HasBelongLoop())) {
                     if(!half2->HasBelongLoop())
                     buildingLoop->AddHalfEdge(half2);
                     lastVertex = half2->getEnd();
                 }
                 else if (half1->getBeg() == lastVertex && (!half1->HasBelongLoop())) {
                     buildingLoop->AddHalfEdge(half1);
                     lastVertex = half1->getEnd();
                 }
             }
             if (lastLastVertex == lastVertex) {
                 buildingLoop->Print();
                 std::cout << "can't finish building loop,something goes wrong " << std::endl;
                 break;
             }
         }
 }
 void Solid::Kemr(int faceId) {
     int i = 0;
     Face* f = FindFace(faceId);
     auto deleteEdge = buildingLoop->GetFirstHalfEdge()->GetBelongEdge();
     for (auto ite = edgeList.begin(); ite != edgeList.end(); ite++) {
         if (*ite == deleteEdge) {
             edgeList.erase(ite);
             break;
         }
     }
     buildingLoop->Kemr();
     f->ChangeInnerLoop(buildingLoop);
     buildingLoop = new Loop();
 }
 void Loop::Kemr() {
     auto temp = firstHalfEdge;
     firstHalfEdge = firstHalfEdge->getNextHalfEdge();
     delete temp->GetBelongEdge();
     Reverse();
 }
 void Solid::Kfmrh(int deletefaceId,int outfaceId) {
     Face* f1 = FindFace(deletefaceId);
     auto f = firstFace;
     while (f->GetnextFace() != f1) f = f->GetnextFace();
     f->ChangeNextFace(f1->GetnextFace());
     Face* f2 = FindFace(outfaceId);
     Loop* l = f1->GetouterLoop();
     f2->ChangeInnerLoop(l);
 }
