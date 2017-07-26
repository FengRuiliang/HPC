// hpc.cpp : Defines the entry point for the console application.
#include <fstream>
#include "vert.h"
#include <omp.h>
#include <string.h>
#include <algorithm>
#include "Header.h"

std::vector<HE_vert*>* pvertices_list_;
int num_vertex_;
#pragma omp threadprivate(pvertices_list_)
int rootsnumber;
int graphnumber;
int num_of_vertex_list_;
 //#define MEMFIX
int main()
{

	//omp_set_nested(1);
	//omp_set_dynamic(1);
	//omp_set_num_threads(12);
#pragma omp parallel shared(rootsnumber,graphnumber)
	{
		LoadFromTxtFile("model.txt");
	}
#pragma omp barrier
	{
		computeRoot();
	}
	for (int i = 0; i < num_vertex_; i++)
	{
		delete pvertices_list_->at(i);
	}
	delete pvertices_list_;
	return 0;
}
HE_vert * InsertVertex(const Vector3d & v)
{

	HE_vert* pvert = new HE_vert(v);
	pvert->id_ = static_cast<int>(pvertices_list_->size());
	pvertices_list_->push_back(pvert);
	#ifdef MEMFIX
    delete pvert;
    return NULL;
#else
	return pvert;
#endif
}


bool LoadFromTxtFile(const char* fins)
{

	if (fins == NULL)
	{
		return false;
	}
	FILE *pfile = fopen(fins, "r");
	fseek(pfile, 0, SEEK_SET);
	char pLine[512];
	if (pvertices_list_ == NULL)
	{
		pvertices_list_ = new std::vector<HE_vert*>;
		//printf("num of thread,%d,list address,%d,\r\n",omp_get_thread_num(),pvertices_list_);
	}
	while (fgets(pLine, 512, pfile))
	{
		char *p[10] = { NULL };
		char *saveptr = NULL;
		p[0] = strtok_s(pLine, " ", &saveptr);
		Vector3d nvv;
		nvv[0] = (double)atof(p[0]);
		for (int i = 1; p[i] = strtok_s(NULL, " ", &saveptr); i++)
		{
			if (*p[0] == 'R')
			{
				rootsnumber = atof(p[1]);
				break;
			}
			if (*p[0] == 'S')
			{
				graphnumber = atof(p[1]);
				break;
			}
			if (i < 2)
			{
				nvv[i] = (double)atof(p[i]);
			}
			else if (i == 2)
			{
				nvv[i] = (double)atof(p[i]);
				InsertVertex(nvv);
				(*(pvertices_list_->rbegin()))->set_seleted(UNSELECTED);// all point init with 0;
				(*(pvertices_list_->rbegin()))->sumVector = (0.0, 0.0, 0.0);
				(*(pvertices_list_->rbegin()))->truth = true;
				//(*(pvertices_list_->rbegin()))->subgraphflag.clear();
			}
			else if (*p[i] != '\r' &&*p[i] != '\r\n'&&*p[i] != '\n' && (int)atof(p[i]) != 0)
			{
				(*(pvertices_list_->rbegin()))->neighborIdx.push_back((int)atof(p[i]) - 1);
				(*(pvertices_list_->rbegin()))->neighbor_search_.push_back((int)atof(p[i]) - 1);
				(*(pvertices_list_->rbegin()))->degree_++;
			}
		}
	}
	fclose(pfile);
	//return true;
	num_of_vertex_list_ = pvertices_list_->size();
	for (auto iter = pvertices_list_->begin(); iter != pvertices_list_->end(); iter++)
	{
		cout<<(*iter)->id_;
		for(int j=0;j<(*iter)->neighborIdx.size();j++)
		{
			cout<<(*iter)->neighborIdx[j];
		}
		cout<<endl;
		if ((*iter)->degree() == 1)
		{
			(*iter)->set_boundary_flag(BOUNDARY);
			findLooppoint((*iter)->id());
		}
		else
		{
			(*iter)->set_boundary_flag(INNER);
		}

	}
	for (auto iter = pvertices_list_->begin(); iter != pvertices_list_->end(); iter++)
	{
		if ((*iter)->selected() == UNSELECTED)
		{
			(*iter)->set_boundary_flag(IN_LOOP);
		}
		else
			(*iter)->set_seleted(UNSELECTED);
	}
	num_vertex_ = pvertices_list_->size();
	return true;
}

void findLooppoint(int ID)
{
	int Degreee = pvertices_list_->at(ID)->degree();
	for (auto iter1 = pvertices_list_->at(ID)->neighborIdx.begin(); iter1 != pvertices_list_->at(ID)->neighborIdx.end(); iter1++)
	{
		if (pvertices_list_->at(*iter1)->selected() == SELECTED)
		{
			//cout << "the neighbor "<<*iter1<< " has been set SELECTED"<<endl;
			--Degreee;
			//cout << Degreee<< endl;
		}
	}
	if (Degreee <= 1)
	{
		pvertices_list_->at(ID)->set_seleted(SELECTED);
		//cout << ID << endl;
		for (auto iter2 = pvertices_list_->at(ID)->neighborIdx.begin(); iter2 != pvertices_list_->at(ID)->neighborIdx.end(); iter2++)
		{
			if (pvertices_list_->at(*iter2)->selected() == UNSELECTED)
			{
				findLooppoint(pvertices_list_->at(*iter2)->id());
			}
		}
	}
	else
	{
		return;
	}
}

void setLoop(int rootsnumber, int& loop_0_, int& loop_1_, int& loop_2_, int& loop_3_, int& loop_4_, int& loop_5_, int& loop_6_, int& loop_7_, int& loop_8_)
{
	switch (rootsnumber)
	{
	case 1:
		loop_7_ = num_vertex_;
		loop_6_ = num_vertex_;
		loop_5_ = num_vertex_;
		loop_4_ = num_vertex_;
		loop_3_ = num_vertex_;
		loop_2_ = num_vertex_;
		loop_1_ = num_vertex_;
		loop_0_ = num_vertex_;
	case 2:
		loop_6_ = num_vertex_;
		loop_5_ = num_vertex_;
		loop_4_ = num_vertex_;
		loop_3_ = num_vertex_;
		loop_2_ = num_vertex_;
		loop_1_ = num_vertex_;
		loop_0_ = num_vertex_;
	case 3:
		loop_5_ = num_vertex_;
		loop_4_ = num_vertex_;
		loop_3_ = num_vertex_;
		loop_2_ = num_vertex_;
		loop_1_ = num_vertex_;
		loop_0_ = num_vertex_;
	case 4:
		loop_4_ = num_vertex_;
		loop_3_ = num_vertex_;
		loop_2_ = num_vertex_;
		loop_1_ = num_vertex_;
		loop_0_ = num_vertex_;
	case 5:
		loop_3_ = num_vertex_;
		loop_2_ = num_vertex_;
		loop_1_ = num_vertex_;
		loop_0_ = num_vertex_;
	case 6:
		loop_2_ = num_vertex_;
		loop_1_ = num_vertex_;
		loop_0_ = num_vertex_;
	case 7:
		loop_1_ = num_vertex_;
		loop_0_ = num_vertex_;
	case 8:
		loop_0_ = num_vertex_;
	case 9:
	default:
		break;
	}
}

void resetroots(int* root)
{
	for (int i = 0; i < 9; i++)
	{
		if (root[i] < num_vertex_)
		{
			pvertices_list_->at(root[i])->cutchoice.clear();
			pvertices_list_->at(root[i])->unavailiable.clear();
			pvertices_list_->at(root[i])->last_unavailiable_size.clear();
		}
	}
}

bool computeRoot()
{

// 	for (int i = 0; i < num_vertex_; i++)
// 	{
// 		if (i < 2)
// 		{
// 			pvertices_list_->at(i)->subgraphflag = 0;
// 		}
// 		else if (i < 10)
// 		{
// 			pvertices_list_->at(i)->subgraphflag = 1;
// 		}
// 		else
// 		{
// 			pvertices_list_->at(i)->subgraphflag = 2;
// 		}
// 	}
// 	cout << computeSubgraph(3);

	int truth = 0;
	#pragma omp parallel for shared(truth)

	int firstLoopNo = 9 - rootsnumber;
#define judgeLoop(loopNo) if (firstLoopNo > loopNo) loop_##loopNo##_ = num_vertex_;
    printf("rootsnumber = %d, num_vertex_ = %d\n", rootsnumber, num_vertex_);

	for (int loop_0_ = 0; loop_0_ < pvertices_list_->size(); loop_0_++)
	{
		if (pvertices_list_->at(loop_0_)->boundary_flag() == BOUNDARY)continue;
				judgeLoop(0)
for (int loop_1_ = 0; loop_1_ < pvertices_list_->size(); loop_1_++)
		{
			if (pvertices_list_->at(loop_1_)->boundary_flag() == BOUNDARY)continue;
judgeLoop(1)
			for (int loop_2_ = 0; loop_2_ < pvertices_list_->size(); loop_2_++)
			{
				if (pvertices_list_->at(loop_2_)->boundary_flag() == BOUNDARY)continue;
judgeLoop(2)
				for (int loop_3_ = 0; loop_3_ < pvertices_list_->size(); loop_3_++)
				{
					if (pvertices_list_->at(loop_3_)->boundary_flag() == BOUNDARY)continue;
judgeLoop(3)
					for (int loop_4_ = 0; loop_4_ < pvertices_list_->size(); loop_4_++)
					{
						if (pvertices_list_->at(loop_4_)->boundary_flag() == BOUNDARY)continue;

judgeLoop(4)
						for (int loop_5_ = 0; loop_5_ < pvertices_list_->size(); loop_5_++)
						{
							if (pvertices_list_->at(loop_5_)->boundary_flag() == BOUNDARY)continue;
judgeLoop(5)

							for (int loop_6_ = 2; loop_6_ < pvertices_list_->size(); loop_6_++)
							{
								if (pvertices_list_->at(loop_6_)->boundary_flag() == BOUNDARY)continue;
	judgeLoop(6)
								for (int loop_7_ =7; loop_7_ < pvertices_list_->size(); loop_7_++)
								{
									if (pvertices_list_->at(loop_7_)->boundary_flag() == BOUNDARY)continue;
	judgeLoop(7)
#pragma omp parallel for shared(truth) 
									for (int loop_8_ = 11; loop_8_ < pvertices_list_->size(); loop_8_++)
									{
#pragma omp  critical(success)
										if (pvertices_list_->at(loop_8_)->boundary_flag() == BOUNDARY)continue;

										setLoop(rootsnumber+3, loop_0_, loop_1_, loop_2_, loop_3_, loop_4_, loop_5_, loop_6_, loop_7_, loop_8_);
										int roots[9] = { loop_0_,loop_1_,loop_2_,loop_3_,loop_4_ ,loop_5_ ,loop_6_ ,loop_7_ ,loop_8_ };
										Computecutchoice(roots);
										resetroots(roots);// reset all vertex information
										int cutsize[9], cutid[9];
										for (int j = 0; j < 9; j++)
										{ 
											if (roots[j] >= num_vertex_)
											{
												cutid[j] = -1;
												cutsize[j] = 0;
											}
											else
											{
												cutid[j] = 0;
												cutsize[j] = pvertices_list_->at(roots[j])->cutedpoints.size();
											}
										}
										for (; cutid[0] < cutsize[0]; cutid[0]++)
										{
											if (cutid[0] != -1 && pvertices_list_->at(roots[0])->set_cutchoice(cutid[0]) == false)continue;
											for (; cutid[1] < cutsize[1]; cutid[1]++)
											{
												if (cutid[1] != -1 && pvertices_list_->at(roots[1])->set_cutchoice(cutid[1]) == false)continue;
												for (; cutid[2] < cutsize[2]; cutid[2]++)
												{
													if (cutid[2] != -1 && pvertices_list_->at(roots[2])->set_cutchoice(cutid[2]) == false)continue;
													for (; cutid[3] < cutsize[3]; cutid[3]++)
													{
														if (cutid[3] != -1 && pvertices_list_->at(roots[3])->set_cutchoice(cutid[3]) == false)continue;
														for (; cutid[4] < cutsize[4]; cutid[4]++)
														{
															if (cutid[4] != -1 && pvertices_list_->at(roots[4])->set_cutchoice(cutid[4]) == false)continue;
															for (; cutid[5] < cutsize[5]; cutid[5]++)
															{
																if (cutid[5] != -1 && pvertices_list_->at(roots[5])->set_cutchoice(cutid[5]) == false)continue;
																for (; cutid[6] < cutsize[6]; cutid[6]++)
																{
																	if (cutid[6] != -1 && pvertices_list_->at(roots[6])->set_cutchoice(cutid[6]) == false)continue;
																	for (; cutid[7] < cutsize[7]; cutid[7]++)
																	{
																		if (cutid[7] != -1 && pvertices_list_->at(roots[7])->set_cutchoice(cutid[7]) == false)continue;
																		for (; cutid[8] < cutsize[8]; cutid[8]++)
																		{
																			
																			if (cutid[8] != -1 && pvertices_list_->at(roots[8])->set_cutchoice(cutid[8]) == false)continue;
																			/*core code*/
																		
																			if (findsubgraph(roots, rootsnumber, cutid, rootsnumber))
																			{
																				if (computeSubgraph(rootsnumber + 1))
																				{
#pragma omp critical(second)
																					{
																						printf(": compute successfully,root,%d,%d,%d,%d,%d,%d,%d, %d, %d,\r\n", roots[0], roots[1], roots[2], roots[3], roots[4], roots[5], roots[6], roots[7], roots[8]);
																						printf(": compute successfully,cuts,%d,%d,%d,%d,%d,%d,%d, %d, %d,,\r\n", cutid[0], cutid[1], cutid[2], cutid[3], cutid[4], cutid[5], cutid[6], cutid[7], cutid[8]);
#pragma omp atomic
																						truth++;

																						//findsubgraph(roots, rootsnumber, cutid, rootsnumber);
																						for (size_t i = 0; i < pvertices_list_->size(); i++)
																						{
																							if (i<num_vertex_)
																							{	cout << "subgraph:"<<i+29<<" "<< pvertices_list_->at(i)->subgraphflag<<endl;
																							}
																							else
																							{
																								cout << "subgraph:" << pvertices_list_->at(i)->vert_pair_+29 << " " << pvertices_list_->at(i)->subgraphflag << endl;
																							}

																						}
																					}
																				}
																				if (truth)
																				{	// end the for loop
																					for (int p = 0; p < 9; p++)
																					{
																						cutid[p] = cutsize[p];
																						roots[p] = num_vertex_;
																					}
																				}
																			}
																			Reset(roots, rootsnumber);
																			/*core code*/
																			if (cutid[8] == -1)
																			{
																				cutid[8] = cutsize[8];
																			}
																			else
																			{
																				//cout<<cutid[5] <<" "<< cutid[6] << " " << cutid[7] << " " << cutid[8]<<endl;
																				pvertices_list_->at(roots[8])->reset_cutchoice();
																			}
																		}
																		if (cutid[7] == -1)
																		{
																			cutid[7] = cutsize[7];
																		}
																		else
																		{
																			cutid[8] = 0;
																			pvertices_list_->at(roots[7])->reset_cutchoice();
																		}
																	}
																	if (cutid[6] == -1)
																	{
																		cutid[6] = cutsize[6];
																	}
																	else
																	{
																		cutid[7] = 0;
																		cutid[8] = 0;
																		pvertices_list_->at(roots[6])->reset_cutchoice();
																	}
																}
																if (cutid[5] == -1)
																{
																	cutid[5] = cutsize[5];
																}
																else
																{
																	cutid[6] = 0;
																	cutid[7] = 0;
																	cutid[8] = 0;
																	pvertices_list_->at(roots[5])->reset_cutchoice();
																}
															}
															if (cutid[4] == -1)
															{
																cutid[4] = cutsize[4];
															}
															else
															{
																cutid[5] = 0;
																cutid[6] = 0;
																cutid[7] = 0;
																cutid[8] = 0;
																pvertices_list_->at(roots[4])->reset_cutchoice();
															}
														}
														if (cutid[3] == -1)
														{
															cutid[3] = cutsize[3];
														}
														else
														{
															cutid[4] = 0;
															cutid[5] = 0;
															cutid[6] = 0;
															cutid[7] = 0;
															cutid[8] = 0;
															pvertices_list_->at(roots[3])->reset_cutchoice();
														}
													}
													if (cutid[2] == -1)
													{
														cutid[2] = cutsize[2];
													}
													else
													{
														cutid[3] = 0;
														cutid[4] = 0;
														cutid[5] = 0;
														cutid[6] = 0;
														cutid[7] = 0;
														cutid[8] = 0;
														pvertices_list_->at(roots[2])->reset_cutchoice();
													}
												}
												if (cutid[1] == -1)
												{
													cutid[1] = cutsize[1];
												}
												else
												{
													cutid[2] = 0;
													cutid[3] = 0;
													cutid[4] = 0;
													cutid[5] = 0;
													cutid[6] = 0;
													cutid[7] = 0;
													cutid[8] = 0;
													pvertices_list_->at(roots[1])->reset_cutchoice();
												}
											}
											if (cutid[0] == -1)
											{
												cutid[0] = cutsize[0];
											}
											else
											{
												cutid[1] = 0;
												cutid[2] = 0;
												cutid[3] = 0;
												cutid[4] = 0;
												cutid[5] = 0;
												cutid[6] = 0;
												cutid[7] = 0;
												cutid[8] = 0;
												pvertices_list_->at(roots[0])->reset_cutchoice();
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
#if 0
	if (!truth)
	{
		printf("split failed, cannot find enough roots to fit the needs\n");
	}
#endif

	return false;
}

void Computecutchoice(int* rootid)
{
	for (int i = 0; i < 9; i++)
	{
		if (rootid[i] < num_vertex_)
		{
			HE_vert* root = pvertices_list_->at((rootid[i]));
			//root.cutedpoints.clear();//for the usecompute
			if (root->cutedpoints.size() > 0 || root->neighborIdx.size() == 0)
			{
				continue;
				//return root.cutedpoints.size();
			}
			for (int k = 0; k < root->neighborIdx.size() - 1; k++)
			{
				std::vector<int> cutedpoint;
				if (k == 0)
				{
					for (unsigned int j = 0; j < root->neighborIdx.size(); j++)
					{
						cutedpoint.clear();
						//cutedpoint.push_back(root.neighbor_search_.at(j));
						cutedpoint.push_back(j);//store the index of neighbor array
						root->cutedpoints.push_back(cutedpoint);
						cutedpoint.clear();
					}
				}
				else
				{
					std::vector<vector<int>> p = root->cutedpoints;
					for (auto iter = p.begin(); iter != p.end(); iter++)//ѡ\D4\F1һ\D6\D6\D2Ѿ\AD\BC\C6\CB\E3\BAõ\C4cut·\BE\B6
					{
						if (iter->size() == k)//ѡ\D4\F1\C6\E4\D6б\DF\CA\FD\B5\C8\D3\DA
						{
							for (int j = 0; j < root->neighborIdx.size(); j++)
							{
								for (int index = 0; index < iter->size(); index++)
								{

									unsigned int s2 = 0;
									for (; s2 < root->neighborIdx.size(); s2++)
									{
										if (iter->at(index) == s2)
											break;

									}
									if (j > s2 && index == iter->size() - 1)
									{
										cutedpoint = *iter;
										//cutedpoint.push_back(root.neighborIdx.at(j));
										cutedpoint.push_back(j);//store the index of neighbor array
										root->cutedpoints.push_back(cutedpoint);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}


bool constructCone(std::vector<Vector3d> vectors)
{
	if (vectors.size() < 1)
	{
		cout << " error: has no neighbor" << endl;
		return false;
	}
	else if (vectors.size() == 1)
	{
		return true;
	}
	else
	{
		Vector3d cirpoints[2] = { vectors[0],vectors[1] };
		Vector3d cij_ = (cirpoints[0] + cirpoints[1]) / ((cirpoints[0] + cirpoints[1]).L2Norm());
		double angle_ = acos(cirpoints[0].Dot(cij_)) * 180 / 3.1415926;
		if (angle_ > 70)
		{
			return false;
		}
		else
		{
			for (size_t i = 2; i < vectors.size(); i++)
			{
				if (angle_ > 70)
				{
					return false;
				}
				else
				{
					double angle_new_ = acos(vectors[i].Dot(cij_)) * 180 / 3.1415926;
					if (angle_new_+angle_-180>-0.05)
					{
						return false;
					}
					if (angle_ < angle_new_)//need to update angle
					{
						Vector3d cir_centre_ = (cirpoints[0] + cirpoints[1]) / 2;
						double r_ = (cirpoints[0] - cir_centre_).L2Norm();
						Vector3d v_centre_q_ = (vectors[i].Dot(cij_)*cij_ - vectors[i]) / ((vectors[i].Dot(cij_)*cij_ - vectors[i]).L2Norm());
						Vector3d q_ = cir_centre_ + v_centre_q_*r_;
						cirpoints[0] = vectors[i];
						cirpoints[1] = q_;
						cij_ = (cirpoints[0] + cirpoints[1]) / ((cirpoints[0] + cirpoints[1]).L2Norm());
						angle_ = acos(cirpoints[0].Dot(cij_)) * 180 / 3.1415926;
						if (angle_ > 70)
						{
							return false;
						}
					}
				}
			}
			return true;
		}
	}
}

bool computeSubgraph(int graphnum)
{
	int subgraph_count_ =0;
	for (int i = 0; i < pvertices_list_->size(); i++)
	{

		if (pvertices_list_->at(i)->subgraphflag >subgraph_count_)
		{
			subgraph_count_ = pvertices_list_->at(i)->subgraphflag;
		}
	}
	if (subgraph_count_+1!= graphnumber)
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	int trueNum_ = 0;
	for (int flag = 0; flag < graphnumber; flag++)
	{

		std::vector<int> subgraphvertice_;
		for (int j = 0; j < pvertices_list_->size(); j++)
		{
			if (pvertices_list_->at(j)->subgraphflag == flag)
			{
				subgraphvertice_.push_back(j);
			}
		}
		for (int k = 0; k < subgraphvertice_.size(); k++)
		{
			for (int k1 = 0; k1 < pvertices_list_->size(); k1++)
			{
				pvertices_list_->at(k1)->set_seleted(UNSELECTED);
			}
			std::vector<Vector3d> vectors_;
			collectVector(pvertices_list_->at(subgraphvertice_[k]), vectors_);
			if (vectors_.size() == 0)
			{
				cout << "flag:"<<flag << "vertex:" << subgraphvertice_[k];
			}
			if (constructCone(vectors_))
			{
				//cout << "flag " << flag << "is  true" << endl;
				trueNum_++;
				break;
			}
			else
			{
				//cout << "flag " << flag << " is false" << endl;
			}
		}
	}
	if (trueNum_ == graphnumber)
	{


		return true;
	}
	else
	{
		return false;
	}
}

void collectVector(HE_vert* root, std::vector<Vector3d>& vectors)
{

	root->set_seleted(SELECTED);
	for (auto iter = root->neighbor_search_.begin(); iter != root->neighbor_search_.end(); iter++)
	{
		HE_vert* neighbor_ = pvertices_list_->at(*iter);
		if (neighbor_->selected() == SELECTED)
		{
			continue;
		}
		else
		{
			Vector3d vector_ = neighbor_->position() - root->position();
			vector_ /= vector_.L2Norm();
			vectors.push_back(vector_);
			collectVector(neighbor_, vectors);
		}
	}
	return;
}

bool findsubgraph(int * roots_, int num_roots_, int * cuts, int num_cuts_)
{
	for (int i = 0; i < 9; i++)
	{
		if (roots_[i] >= num_vertex_)
		{
			continue;// ignore unneed root
		}
		//cout << "root: " << roots_[i] + 1;
		HE_vert* vertex = pvertices_list_->at(roots_[i]);
		InsertVertex(vertex->position());
		(*(pvertices_list_->rbegin()))->vert_pair_ = roots_[i];
		//cout << " cutchoice is:";
		for (auto iter = vertex->cutedpoints[cuts[i]].begin(); iter != vertex->cutedpoints[cuts[i]].end(); iter++)
		{
			//cout << *iter + 1;
			HE_vert* vert_neighbor_ = (pvertices_list_->at(vertex->neighborIdx[*iter]));
			if (find(vertex->neighbor_search_.begin(), vertex->neighbor_search_.end(), vert_neighbor_->id()) == vertex->neighbor_search_.end())//mei you zhaodao, xuyao zhao pair
			{
				for (auto iterNR = vertex->neighbor_search_.begin(); iterNR != vertex->neighbor_search_.end(); iterNR++)
				{
					if (pvertices_list_->at(*iterNR)->vert_pair_ == vert_neighbor_->id())
					{
						vert_neighbor_ = (pvertices_list_->at(*iterNR));
					}
				}
			}
			vertex->neighbor_search_.erase(find(vertex->neighbor_search_.begin(), vertex->neighbor_search_.end(), vert_neighbor_->id()));
			//vert_neighbor_.neighbor_search_.erase(find(vert_neighbor_.neighbor_search_.begin(), vert_neighbor_.neighbor_search_.end(), vertex.id()));

			(*(pvertices_list_->rbegin()))->neighbor_search_.push_back(vert_neighbor_->id());//add neighbor to new vertex
																							 //vert_neighbor_.neighbor_search_.push_back((*(pvertices_list_->rbegin())).id());

			auto find_ = find(vert_neighbor_->neighbor_search_.begin(), vert_neighbor_->neighbor_search_.end(), vertex->id());
			if (find_ != vert_neighbor_->neighbor_search_.end())
			{
				*find_ = (*(pvertices_list_->rbegin()))->id();//modify neighbor vertex's neighbor
			}

			//vert_neighbor_.neighbor_search_;
		}
		if (vertex->neighbor_search_.size() == 0)
		{
			return false;
		}
		//cout << endl;
	}
	for (int i = 0; i < pvertices_list_->size(); i++)
	{
		pvertices_list_->at(i)->set_seleted(UNSELECTED);
		pvertices_list_->at(i)->subgraphflag=-1;
	}
	int flag = -1;
	for (int j = 0; j < pvertices_list_->size(); j++)
	{
		if (pvertices_list_->at(j)->neighborIdx.size()==0)
		{
			continue;
		}
		if (pvertices_list_->at(j)->selected() == UNSELECTED)
		{
			flag++;
			DFS(pvertices_list_->at(j), flag);
		}
	}
	return true;
}

void DFS(HE_vert * cur, int flag)
{
	if (cur->selected() == UNSELECTED)
	{
		cur->set_seleted(SELECTED);
		cur->subgraphflag = flag;
		for (int i = 0; i < cur->neighbor_search_.size(); i++)
		{
			DFS(pvertices_list_->at(cur->neighbor_search_[i]), flag);
		}
	}
}

void Reset(int roots[], int num)
{
	for (int i = 0; i < num; i++)
	{
		pvertices_list_->at(roots[8 - i])->neighbor_search_ = pvertices_list_->at(roots[8 - i])->neighborIdx;
		if (pvertices_list_->size() > num_vertex_)
		{
			pvertices_list_->back()->clearVertex();
			delete pvertices_list_->back();
			pvertices_list_->pop_back();
		}

	}
	//restore 
	for (int j = 0; j < pvertices_list_->size(); j++)
	{
		pvertices_list_->at(j)->sumVector = (0.0, 0.0, 0.0);
		pvertices_list_->at(j)->truth = true;
		pvertices_list_->at(j)->set_seleted(UNSELECTED);
		pvertices_list_->at(j)->subgraphflag=-1;
		pvertices_list_->at(j)->neighbor_search_ = pvertices_list_->at(j)->neighborIdx;
	}
}
