/*
 * yo_shit.cpp
 *
 *  Created on: Dec 28, 2015
 *      Author: dmendonca
 */
#ifndef CSOKO_SOLVER__H
#define CSOKO_SOLVER__H


#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <tuple>
#include <array>
#include <map>
#include <boost/algorithm/string.hpp>
#include <boost/unordered_set.hpp>

typedef std::vector<char> TableRow;
typedef std::vector<TableRow> Table;
typedef std::tuple<size_t, size_t> T_pos;
typedef std::vector<T_pos> Vec_t_pos;
typedef std::tuple<size_t, std::string> Robot_Move;
typedef std::vector<Robot_Move > Moves_R;

struct Board {
	Table sData, dData;
	int px, py;


	bool setRobot(size_t xx, size_t yy);
	void setMap(Table t, bool toSetRobot=false);
	void setRobotAndMap(int xx, int yy, Table t);

	Board(int xx, int yy, Table t);
	Board(std::string b);

	bool move(int x, int y, int dx, int dy, Table &data);
	bool push(int x, int y, int dx, int dy, Table &data);
	bool isSolved(const Table &data);
	std::string solve();
};


/**
 *
 */
void printBoard(const Table &t);


/**
 *Sets all robots, boxes and delivery points as walls
 */
void allAsWalls(Table &t, const Vec_t_pos &rs, const Vec_t_pos &bs, const Vec_t_pos &ds);


/**
 *
 */
void allAsBefore(Table &t, const Vec_t_pos &rs, const Vec_t_pos &bs, const Vec_t_pos &ds);


/**
 *Set a robot position, a box and a location to where the box should be taken in a Table
 *@t table to add the elements
 *@r robot position in the table
 *@b box position in the table
 *@d destiny position in the table where the box should be taken
 */
void setInterests(Table &t, const T_pos &r, const T_pos &b, const T_pos &d);


/**
 *Sets a box and a delivery point in a Table as walls
 */
void negateInterest(Table &t, const T_pos &b, const T_pos &d);


/**
 *
 */
void performMove(Table &t, T_pos &r, T_pos &b, const std::string &sol, bool isPrint=false);

/**
 *
 */
bool turn(const Table t, const Vec_t_pos rs, const Vec_t_pos bs, const Vec_t_pos ds, Moves_R &moves, size_t robot_nr);


/**
 *
 */
void dynamicSolPrint(Table t);


/**
 *
 *
int main() {
	Table t;
	ifstream ifs("sokobanMap1");
	vector<T_pos > rs; //robots
	vector<T_pos > bs; //boxes
	vector<T_pos > ds; //destinys

	if(ifs.is_open())
	{
		string line;
		while(getline(ifs, line))
		{
			TableRow row;
			for(auto c : line)
			{
				row.push_back(c);
				if(c == '@')
					rs.push_back(make_tuple((row.size() -1),t.size()));
				else if(c == '$')
					bs.push_back(make_tuple((row.size() -1),t.size()));
				else if(c == '.')
					ds.push_back(make_tuple((row.size() -1),t.size()));
			}
			t.push_back(row);
		}

		cout << rs.size() << " " << bs.size() << " " << ds.size() << " " << endl << "rs: " << endl;


		ifs.close();
	}
	vector<vector<string> > r_moves(rs.size());
	bool solvable= turn(t, rs, bs, ds, r_moves, 0);


	if(solvable)
	{
		for(string s : r_moves.at(0))
		{
			cout<<s<<endl;
			//performMove(t, rs.at(0), bs.at(0), s, true);
		}
		for(string s : r_moves.at(1))
		{
			cout<<s<<endl;
			//performMove(t, rs.at(0), bs.at(0), s, true);
		}
	}

	return 0;
}
*/
#endif
