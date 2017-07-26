#ifndef _VERT_H_
#define  _VERT_H_
#include "vector3.h"
#include <vector>
enum SelectTag
{
	UNSELECTED = 0,		//
	SELECTED,			//
	OTHER				//
};
enum BoundaryTag
{
	BOUNDARY,
	INNER,
	TO_SPLIT,
	IN_LOOP,
	LOOP_TO_SPLIT
};
class HE_vert
{
public:

	int			id_;
	Vector3d 	position_;		//!< vertex position
	int			degree_;
	 BoundaryTag	boundary_flag_;	//!< boundary flag
	int			selected_;		//!< a tag: whether the vertex is selected
	std::vector<int> neighborIdx;
	std::vector<int> neighbor_search_;//use to do BFS of DFS 
	std::vector<std::vector<int> > cutedpoints;//a vector store vector pointer ,store choice of cutting
	std::vector <int> cutchoice;
	std::vector <int> unavailiable;
	int subgraphflag;
	Vector3d sumVector;
	int vert_pair_;
	bool truth;
	/*--------------------add by wang kang at 2013-10-12----------------------*/


public:

	HE_vert(){}
	HE_vert(const Vector3d& v)
		: id_(-1), position_(v), degree_(0), boundary_flag_(INNER), selected_(UNSELECTED),
		neighborIdx(), vert_pair_(-1),subgraphflag(-1){}
	~HE_vert(void) {
	}
	void clearVertex() {
		neighborIdx.clear();
		neighbor_search_.clear();
		cutedpoints.clear();
		cutchoice.clear();
		unavailiable.clear();
		//subgraphflag.clear();
		last_unavailiable_size.clear();
	}
	Vector3d& position(void) { return position_; }
	bool		isOnBoundary(void) { return boundary_flag_ == BOUNDARY; }
	int			id(void) { return id_; }
	int			degree(void) { return degree_; }
	int			selected(void) { return selected_; }
	BoundaryTag boundary_flag(void) { return boundary_flag_; }
	void		set_position(const Vector3d& p) { position_ = p; }
	void		set_id(int id) { id_ = id; }
	void		set_seleted(int tag) { selected_ = tag; }
	void		set_boundary_flag(BoundaryTag bt) { boundary_flag_ = bt; }
	 bool		set_cutchoice(int cutchoiceID);
	bool		reset_cutchoice();
	std::vector<int>	last_unavailiable_size;
private:
};
#endif
