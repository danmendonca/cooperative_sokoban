#ifndef CSOKO_SOLVER
#define CSOKO_SOLVER



#include <cstdio>
#include <fstream>
#include <vector>
#include <cmath>
#include <map>
#include <string>
#include <string.h>
#include <algorithm>
#include <iterator>
#include <queue>
#include <stack>
#include <ctime>


namespace csoko_thinker{


char mapa[32][32],bmapa[32][32];
short nummapa[32][32],amp[32][32],dirxy[1024][2];

std::ifstream in;
std::vector<short> boxes,box_dest; std::vector<short>::iterator ite;
std::map<std::vector<short>,bool> ClosedList;
short sokop,Nbox=0,pcont=1,maxx=0,maxy=0;;
short moves[4][2] = { {-1,0},{0,1},{1,0},{0,-1} };
std::vector<path_node> path;



/***************************************
 *
 *node class
 *
 ****************************************/
class node {
public:
	node( short ccost, short ch, int cnp, short cpb, std::vector<short>& cbox ) {
		cost=ccost; h= ch; npath=cnp; config=cbox; placedbox=cpb;
	};
	short cost,h,placedbox;
	int npath;
	std::vector<short> config;
};




/***************************************
 *
 *path_node class
 *
 ****************************************/
class path_node {
public:
	path_node(int cxy, short cdir, int cb) {
		xy=cxy; dir=cdir; back=cb;
	};
	int xy;
	short dir;
	int back;
};



/**
 *
 */
bool operator < (const node &a, const node &b) {
	if(a.placedbox==b.placedbox) return a.h+a.cost>b.h+b.cost;
	return a.placedbox<b.placedbox;
};




/***************************************
 *
 *point_sort class
 *
 ****************************************/
class point_sort {
public:
	bool operator () (const short& a, const short& b) {
		short da=dirxy[a][0]+dirxy[a][1], db=dirxy[b][0]+dirxy[b][1];
		if(da==db) {
			if(dirxy[a][0]==dirxy[b][0]) return dirxy[a][1]<dirxy[b][1];
			return dirxy[a][0]<dirxy[b][0];
		}
		return da<db;
	};
};


#endif
