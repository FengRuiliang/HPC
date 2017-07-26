#pragma once
#include <vector>
#include "vector3.h"
#include "vert.h"
#include <omp.h>
class HE_vert;
extern HE_vert * InsertVertex(const Vector3d & v);
extern bool LoadFromTxtFile(const char* fins);
extern void findLooppoint(int ID);
extern bool computeRoot();
extern void Computecutchoice(int* root);
extern bool findsubgraph(int* roots_, int num_roots_, int* cuts, int num_cuts_);
extern void DFS(HE_vert* cur, int flag);
extern void Reset(int roots[],int num);
extern bool computeSubgraph(int graphnum);
extern void collectVector(HE_vert* root,std::vector<Vector3d>& vectors);//对数组的引用
extern void ComputeAngle(int id, Vector3d sum);
extern void resetroots(int* root);




