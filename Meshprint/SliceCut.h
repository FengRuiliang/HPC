#pragma once
#include "HE_mesh\Mesh3D.h"
#include "globalFunctions.h"
class Mesh3D;
class vector;
class Field;
#define DEFAULT_T 0.02f
typedef trimesh::point point;
typedef trimesh::vec3  Vec3f;
typedef trimesh::vec4  Vec4f;

class cutLine
{
public:
	cutLine() {};
	cutLine(point p1, point p2)
	{
		position_vert[0] = p1; position_vert[1] = p2;
		sweep_point_ = p1;
		sweep_point_Last_ = p1;
	/*	edgeid_vert[0] = edge1; edgeid_vert[1] = edge2;*/
	}
	cutLine(point p1, point p2,int xID,int yID)
	{
		position_vert[0] = p1; position_vert[1] = p2;
		x_field_ = xID;
		y_field_ = yID;
		/*	edgeid_vert[0] = edge1; edgeid_vert[1] = edge2;*/
	}
	~cutLine()
	{}

	point position_vert[2];
	int edgeid_vert[2];

public:
	Vec3f sweep_point_;
	Vec3f sweep_point_Last_;
	int x_field_;
	int y_field_;
};



class SliceCut
{
public:
	SliceCut()
	{}
	SliceCut(Mesh3D* ptr_in, float tn = DEFAULT_T) :mesh_in_(ptr_in)
		{
		//thickness_=0.5;//���
		num_pieces_= mesh_in_->getBoundingBox().at(0).at(2) /thickness_+1;//����
		
		storage_Face_list_ = NULL;// new #2/thickness_������Ƭ�ֲ�
		pieces_list_ = NULL;//�������
		circle_list_ = NULL;//meiyiceng �ж�������ͨ��Ԫ
	};
	~SliceCut();

	void SetThickness(float tn = DEFAULT_T){
		if (tn != 0)
		{
			thickness_ = tn;
		}
	};

	void clearcut();
	void CutInPieces();
	void Exportslice();
	float getThickness() { return thickness_; };
	std::vector < std::vector<cutLine>* >* GetPieces(){ return pieces_list_; }
	int GetNumPieces() { return num_pieces_; }
	 std::vector<int> * storeMeshIntoSlice();
	 std::vector<int>  *storage_Face_list_;

	int  num_pieces_;
	
private:
	//float thickness_;
	Mesh3D* mesh_in_;
	 std::vector<cutLine>* circle_list_;
	 std::vector < std::vector<cutLine>* >*pieces_list_;
	int isEdgeInFace(HE_vert* pvert1, HE_vert* pvert2, HE_face* pface);
	std::vector<int> sortVertInFace(int faceid);
};

