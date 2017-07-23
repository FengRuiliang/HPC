#pragma once
#include "SliceCut.h"
#include "QDebug"

#define MAX_FLOAT_VALUE (static_cast<float>(10e10))
#define MIN_FLOAT_VALUE	(static_cast<float>(-10e10))
//CHANGE
// type definitions
typedef std::vector<HE_vert* >::iterator VERTEX_ITER;
typedef std::vector<HE_face* >::iterator FACE_ITER;
typedef std::vector<HE_edge* >::iterator EDGE_ITER;

typedef std::vector<HE_vert* >::reverse_iterator VERTEX_RITER;
typedef std::vector<HE_face* >::reverse_iterator FACE_RITER;
typedef std::vector<HE_edge* >::reverse_iterator EDGE_RITER;
typedef std::pair<HE_vert*, HE_vert* > PAIR_VERTEX;


SliceCut::~SliceCut()
{
	delete[]storage_Face_list_;
	delete[]pieces_list_;
}


std::vector<int> * SliceCut::storeMeshIntoSlice()
{

	//qDebug() << mesh_in_->get_faces_list()->size();
	const std::vector<HE_face *>& faces = *(mesh_in_->get_faces_list());
	const std::vector<HE_vert *>& vertice = *(mesh_in_->get_vertex_list());
	storage_Face_list_ = new std::vector<int>[num_pieces_];// new #2/thickness_
	pieces_list_ = new std::vector<std::vector<cutLine>*>[num_pieces_];
	
	//qDebug() << storage_Face_list_->size()<<2/thickness_+1;
	for (auto iter_Face= faces.begin();iter_Face!= faces.end();iter_Face++ )
	{
		
		//minimal 2 and maximal 0
		float min_z_ = vertice.at(sortVertInFace((*iter_Face)->id()).at(2))->position().z();
		float max_z_ = vertice.at(sortVertInFace((*iter_Face)->id()).at(0))->position().z();
		if (min_z_==max_z_)// 22/01/2017
		{
			continue;
		}
		//the num of layer equal to 
		for (int j= min_z_ / thickness_;j<=max_z_ / thickness_;j++)
		{
			
			storage_Face_list_[j].push_back((*iter_Face)->id());
		}
	}
	return storage_Face_list_;
}

int SliceCut::isEdgeInFace(HE_vert* pvert1, HE_vert* pvert2, HE_face* pface){
	HE_edge* temp_edge = mesh_in_->getedgemap()[PAIR_VERTEX(pvert1, pvert2)];
	if (mesh_in_->getedgemap()[PAIR_VERTEX(pvert1, pvert2)]->pface_ == pface)
	{
		return temp_edge->id();
	}
	temp_edge = temp_edge->ppair_;
	if (temp_edge->pface_ == pface)
	{
		return temp_edge->id();
	}
	return -1;
}

std::vector<int> SliceCut::sortVertInFace(int faceid)
{
	// get sorted vertexes in a face
	HE_face* curFace = mesh_in_->get_face(faceid);
	HE_edge* pedge = curFace->pedge_;
	//each face with 3 vertexes
	std::vector<int> vertex_list;
	double max_height = MIN_FLOAT_VALUE;
	double min_height = MIN_FLOAT_VALUE;
	do
	{
		int vertid = pedge->pvert_->id();
		vertex_list.push_back(vertid);
		pedge = pedge->pnext_;
	} while (pedge != curFace->pedge_);
#define it_rotate_vert(n) mesh_in_->get_vertex_list()->at(vertex_list[n])->position()
#define switch_vert(a,b) tempid = vertex_list[a]; vertex_list[a] = vertex_list[b]; vertex_list[b] = tempid;
	if (vertex_list.size() == 3)
	{
		int tempid;
		if (it_rotate_vert(0).z() < it_rotate_vert(2).z())
		{
			switch_vert(0, 2);
		}
		if (it_rotate_vert(1).z() < it_rotate_vert(2).z())
		{
			switch_vert(1, 2);
		}
		else if (it_rotate_vert(0).z() < it_rotate_vert(1).z())
		{
			switch_vert(0, 1);
		}
	}

	return vertex_list;
}

void SliceCut:: clearcut()
{
	if (pieces_list_==NULL)
	{
		return;
	}
	for (size_t i=0;i<num_pieces_;i++)
	{
		pieces_list_[i].clear();
		storage_Face_list_[i].clear();
	}
	delete[]pieces_list_;
	delete[] storage_Face_list_;
	pieces_list_ = NULL;
	storage_Face_list_ = NULL;
}

void SliceCut::CutInPieces()
{
	const std::vector<HE_face *>& faces = *(mesh_in_->get_faces_list());
	const std::vector<HE_vert *>& verts = *(mesh_in_->get_vertex_list());
	for (size_t i = 0; i <num_pieces_; i++)
	{
		std::vector<int>&slice_faces_ = storage_Face_list_[i];
		//qDebug() <<i<< slice_faces_.size();
		float cur_height_ = i*thickness_;
		HE_edge * cur_edge_=NULL;
		HE_edge * longest_edge_=NULL;
		HE_edge * cut_edge = NULL;
		std::vector<int> vert_sort;
	
		// find the longest edge which is been acrossed;
		std::vector<HE_edge *> layer_hedge_;
		for (auto iter = slice_faces_.begin(); iter != slice_faces_.end(); iter++)
		{
			//circle_list_ = new std::vector<cutLine>;//new a vector storage
			//pieces_list_[i].push_back(circle_list_);
			if (*iter == -1)
			{
				continue;
			}
			cur_edge_ = faces.at(*iter)->pedge_;
			vert_sort = sortVertInFace(*iter);//sort the three vertex in z position of No *iter face.
			//lowest point shall  not higher than current height
			//qDebug() << *iter;
			if (verts.at(vert_sort[2])->position().z()>cur_height_|| (faces.at(*iter)->normal().x()==0& faces.at(*iter)->normal().y()==0))//in this layer,eliminate the higher one
			{
				*iter=-1;
				continue;
			}
			else
			{
				//if (circle_list_!=NULL)
				{
					circle_list_ = new std::vector<cutLine>;//new a vector storage
					pieces_list_[i].push_back(circle_list_);
				}
				
				if (cur_edge_->pvert_->id()==vert_sort[0])//��ǰ�ߵ�end����ߵ�
				{
					if (cur_edge_->pprev_->pvert_->id() == vert_sort[1])//��ǰ�ߵ�start���м�߶ȵ㡣
					{
						longest_edge_ = cur_edge_->pnext_;
					}
					else
					{
						longest_edge_ = cur_edge_;
					}
						
				}
				else if (cur_edge_->pvert_->id()==vert_sort[1])
				{
					longest_edge_ = cur_edge_->pprev_;
				}
				else
				{
					if (cur_edge_->pprev_->pvert_->id()==vert_sort[1])
					{
						longest_edge_ = cur_edge_->pnext_;
					}
					else
					{
						longest_edge_ = cur_edge_;
					}
						
				}
				cur_edge_ = longest_edge_;



				//cur_edge_ = cut_edge;

				if (cur_edge_ != NULL)
				{
					int index = 0;
					do
					{
						index++;
						Vec3f cur_vector_(cur_edge_->pvert_->position() - cur_edge_->pprev_->pvert_->position());

						point pos1, pos2;
						if (cur_vector_.z() == 0)//it means the face is lay flat in the cut face.
						{
							cur_edge_ = cur_edge_->pnext_->ppair_;//28/11/2016
							continue;
						}
						else
						{
							//if (cur_edge_->pvert_->position().z() > cur_height_)//the cur_edge_ is up vector
							if (cur_vector_.z() > 0 )// modify at 2017/2/28
							{
								cut_edge = cur_edge_;
								pos2 = cur_edge_->pvert_->position() - cur_vector_*(cur_edge_->pvert_->position().z() - cur_height_) / cur_vector_.z();
								if (cur_edge_->pnext_->pvert_->position().z() > cur_height_)
								{
									cur_edge_ = cur_edge_->pprev_;
									cur_vector_ = (cur_edge_->pvert_->position() - cur_edge_->pprev_->pvert_->position());
									pos1 = cur_edge_->pvert_->position() - cur_vector_*(cur_edge_->pvert_->position().z() - cur_height_) / cur_vector_.z();
									
								}
								else if (cur_edge_->pnext_->pvert_->position().z() < cur_height_)
								{
									cur_edge_ = cur_edge_->pnext_;
									cur_vector_ = (cur_edge_->pvert_->position() - cur_edge_->pprev_->pvert_->position());
									pos1 = cur_edge_->pvert_->position() - cur_vector_*(cur_edge_->pvert_->position().z() - cur_height_) / cur_vector_.z();

								}
								else
								{
									pos1 = cur_edge_->pnext_->pvert_->position();
									cur_edge_ = cur_edge_->pnext_;//2016/11/28
								}
								
							}
							else  //(cur_edge_->pvert_->position().z() <=cur_height_),the cur_edge_ is down vector
							{

								pos1 = cur_edge_->pvert_->position() - cur_vector_*(cur_edge_->pvert_->position().z() - cur_height_) / cur_vector_.z();

								if (cur_edge_->pnext_->pvert_->position().z() < cur_height_)
								{
									
									cur_edge_ = cur_edge_->pprev_;
									cur_vector_ = (cur_edge_->pvert_->position() - cur_edge_->pprev_->pvert_->position());
									
									pos2 = cur_edge_->pvert_->position() - cur_vector_*(cur_edge_->pvert_->position().z() - cur_height_) / cur_vector_.z();
									cut_edge = cur_edge_;
								}
								else if (cur_edge_->pnext_->pvert_->position().z() > cur_height_)
								{
									cur_edge_ = cur_edge_->pnext_;
									cur_vector_ = (cur_edge_->pvert_->position() - cur_edge_->pprev_->pvert_->position());
									pos2 = cur_edge_->pvert_->position() - cur_vector_*(cur_edge_->pvert_->position().z() - cur_height_) / cur_vector_.z();
									cut_edge = cur_edge_;
								}
								else// cur_edge_->pnext_->pvert_->position().z()== cur_height_
								{
									pos2 = cur_edge_->pnext_->pvert_->position();
									if (cur_edge_->pvert_->position().z() == cur_height_)
									{
										cur_edge_ = cur_edge_->pprev_;
									}
									else
									{
										cur_edge_ = cur_edge_->pnext_;
									}

									cut_edge = cur_edge_;
								}

							}
							cur_edge_ = cut_edge->ppair_;
						}
						if (cur_edge_->pface_==NULL)//means reach the boundary 28/11/2016
						{
							//qDebug()<<" faces.at(*iter)"<< faces.at(*iter) <<
							break;
						}
						std::vector<int>::iterator p = std::find(slice_faces_.begin(), slice_faces_.end(), cur_edge_->pface_->id());
						if (p == slice_faces_.end())
						{
							break;
							qDebug() << i << index;
						}
						//if (p != slice_faces_.end())
						{
							
							*p = -1;
						}
						
						//qDebug() << "slice" << i;
						if (pos1==pos2)
						{
							continue;
						}
						cutLine new_cutline(pos1, pos2);
 						circle_list_->push_back(new_cutline);
					} while (cur_edge_->id() != longest_edge_->id() && cur_edge_ ->pface_!= NULL);
				}
				
				
				//break;
			}

		}
	}


}
void SliceCut::Exportslice() //shuchu  hanshu
{
	for (int i = 0; i < num_pieces_; i++)
	{
		for (int j = 0; j < pieces_list_[i].size(); j++)
		{
			for (int k = 0; k < (pieces_list_[i])[j]->size(); k++)
			{
				std::cout << ((pieces_list_[i])[j])->at(k).position_vert[0].x() << ((pieces_list_[i])[j])->at(k).position_vert[0].y() << ((pieces_list_[i])[j])->at(k).position_vert[0].z();
				std::cout << ((pieces_list_[i])[j])->at(k).position_vert[1].x() << ((pieces_list_[i])[j])->at(k).position_vert[1].y() << ((pieces_list_[i])[j])->at(k).position_vert[1].z();
			}
		}
	}
}
