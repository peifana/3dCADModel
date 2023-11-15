#include<iostream>
#include"BuildSolid.hpp"
#include"Elements.hpp"
#include"Draw.hpp"
int main(int argc, char** argv) {
	Solid* example = BuildSolid();
	example->Print();
	Draw(argc, argv, example);
}