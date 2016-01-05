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
#include <limits>
#include <boost/algorithm/string.hpp>
#include <boost/unordered_set.hpp>

typedef std::vector<char> TableRow;
typedef std::vector<TableRow> Table;
typedef std::tuple<size_t, size_t> T_pos;
typedef std::vector<T_pos> Vec_t_pos;
typedef std::tuple<size_t, std::string> Robot_Move;
typedef std::vector<Robot_Move > V_Robot_Move;

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
std::string printBoard(const Table &t);


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
 * This method performs all movements given in @sol to a robot @r and box @b in a sokoban map @t
 */
void performAllMoves(Table &t, T_pos &r, T_pos &b, const std::string &sol, bool isPrint=false);


/**
 *
 */
void performOneMove(Table &t, T_pos &r, T_pos &b, const char &c);


/**
 *
 */
void getMovementDelta(char c, int &x, int &y);


/**
 *
 */
bool turn(const Table t, const Vec_t_pos rs, const Vec_t_pos bs, const Vec_t_pos ds, V_Robot_Move &moves, size_t robot_nr);


#endif
