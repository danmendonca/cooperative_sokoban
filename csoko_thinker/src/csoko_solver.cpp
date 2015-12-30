/*
 * yo_shit.cpp
 *
 *  Created on: Dec 28, 2015
 *      Author: dmendonca
 */

#include "csoko_thinker/csoko_solver.h"

using namespace std;


bool Board::setRobot(size_t xx, size_t yy){
	bool correctRobotPos = false;
	if(sData.size()> yy && sData.at(yy).size() > xx)
		if(sData.at(yy).at(xx) == '@')
		{
			px = xx;
			py = yy;
			correctRobotPos = true;
		}

	return correctRobotPos;
}

void Board::setMap(Table t, bool toSetRobot)
{

	sData.clear();
	dData.clear();
	map<char,char> maps = {{' ',' '}, {'.','.'}, {'@',' '},
			{'#','#'}, {'$',' '}},
					mapd = {{' ',' '}, {'.',' '}, {'@','@'},
							{'#',' '}, {'$','*'}};

	for(size_t i = 0; i < t.size(); i++)
	{
		TableRow sTemp, dTemp;
		for (size_t c = 0; c < t.at(i).size(); c++) {
			char ch = t.at(i).at(c);
			sTemp.push_back(maps[ch]);
			dTemp.push_back(mapd[ch]);
			if(toSetRobot && ch == '@')
			{
				px=c;
				py=i;
			}
		}
		sData.push_back(sTemp);
		dData.push_back(dTemp);
	}
}

void Board::setRobotAndMap(int xx, int yy, Table t)
{
	bool goodRobot = setRobot(xx, yy);
	setMap(t, !goodRobot);

}


Board::Board(int xx, int yy, Table t)
{
	setRobotAndMap(xx, yy, t);
}


Board::Board(string b) {
	vector<string> data;
	boost::split(data, b, boost::is_any_of("\n"));

	//save higher length in width
	size_t width = 0;
	for (auto &row: data)
		width = max(width, row.size());

	map<char,char> maps = {{' ',' '}, {'.','.'}, {'@',' '},
			{'#','#'}, {'$',' '}},
					mapd = {{' ',' '}, {'.',' '}, {'@','@'},
							{'#',' '}, {'$','*'}};

	for (size_t r = 0; r < data.size(); r++) {
		TableRow sTemp, dTemp;
		for (size_t c = 0; c < width; c++) {
			char ch = c < data[r].size() ? data[r][c] : ' ';
			sTemp.push_back(maps[ch]);
			dTemp.push_back(mapd[ch]);
			if (ch == '@') {
				px = c;
				py = r;
			}
		}
		sData.push_back(sTemp);
		dData.push_back(dTemp);
	}
}



bool Board::move(int x, int y, int dx, int dy, Table &data) {
	if (sData[y+dy][x+dx] == '#' || data[y+dy][x+dx] != ' ')
		return false;

	data[y][x] = ' ';
	data[y+dy][x+dx] = '@';
	return true;
}

bool Board::push(int x, int y, int dx, int dy, Table &data) {
	if (sData[y+2*dy][x+2*dx] == '#' || data[y+2*dy][x+2*dx] != ' ')
		return false;

	data[y][x] = ' ';
	data[y+dy][x+dx] = '@';
	data[y+2*dy][x+2*dx] = '*';
	return true;
}

bool Board::isSolved(const Table &data) {
	for (size_t r = 0; r < data.size(); r++)
		for (size_t c = 0; c < data[r].size(); c++)
			if ((sData[r][c] == '.') != (data[r][c] == '*'))
				return false;
	return true;
}

string Board::solve() {
	boost::unordered_set<Table, boost::hash<Table>> visited;
	visited.insert(dData);

	queue<tuple<Table, string, int, int> > open;
	open.push(make_tuple(dData, "", px, py));

	vector<tuple<int, int, char, char>> dirs = {
			make_tuple( 0, -1, 'u', 'U'),
			make_tuple( 1,  0, 'r', 'R'),
			make_tuple( 0,  1, 'd', 'D'),
			make_tuple(-1,  0, 'l', 'L')
	};

	while (open.size() > 0) {
		Table temp, cur = get<0>(open.front());
		string cSol = get<1>(open.front());
		int x = get<2>(open.front());
		int y = get<3>(open.front());
		open.pop();

		for (int i = 0; i < 4; ++i) {
			temp = cur;
			int dx = get<0>(dirs[i]);
			int dy = get<1>(dirs[i]);

			if (temp[y+dy][x+dx] == '*') {
				if (push(x, y, dx, dy, temp) &&
						visited.find(temp) == visited.end()) {
					if (isSolved(temp))
						return cSol + get<3>(dirs[i]);
					open.push(make_tuple(temp, cSol + get<3>(dirs[i]),
							x+dx, y+dy));
					visited.insert(temp);
				}
			} else if (move(x, y, dx, dy, temp) &&
					visited.find(temp) == visited.end()) {
				if (isSolved(temp))
					return cSol + get<2>(dirs[i]);
				open.push(make_tuple(temp, cSol + get<2>(dirs[i]),
						x+dx, y+dy));
				visited.insert(temp);
			}
		}
	}

	return "No solution";
}



void printBoard(const Table &t)
{
	for(auto row: t)
	{
		for(auto c : row)
			cout<<c;
		cout<<endl;
	}
}



void allAsWalls(Table &t, const Vec_t_pos &rs, const Vec_t_pos &bs, const Vec_t_pos &ds)
{
	for(T_pos r_pos : rs)
	{
		t.at(get<1>(r_pos)).at(get<0>(r_pos)) = '#';
	}
	for(T_pos b_pos : bs)
	{
		t.at(get<1>(b_pos)).at(get<0>(b_pos)) = '#';
	}
	for(T_pos d_pos : ds)
	{
		t.at(get<1>(d_pos)).at(get<0>(d_pos)) = '#';
	}
}



void allAsBefore(Table &t, const Vec_t_pos &rs, const Vec_t_pos &bs, const Vec_t_pos &ds)
{
	for(T_pos r_pos : rs)
	{
		t.at(get<1>(r_pos)).at(get<0>(r_pos)) = '@';
	}
	for(T_pos b_pos : bs)
	{
		t.at(get<1>(b_pos)).at(get<0>(b_pos)) = '$';
	}
	for(T_pos d_pos : ds)
	{
		t.at(get<1>(d_pos)).at(get<0>(d_pos)) = '.';
	}
}

void setInterests(Table &t, const T_pos &r, const T_pos &b, const T_pos &d)
{
	t.at(get<1>(r)).at(get<0>(r)) = '@';
	t.at(get<1>(b)).at(get<0>(b)) = '$';
	t.at(get<1>(d)).at(get<0>(d)) = '.';
}


void negateInterest(Table &t, const T_pos &b, const T_pos &d)
{
	t.at(get<1>(b)).at(get<0>(b)) = '#';
	t.at(get<1>(d)).at(get<0>(d)) = '#';
}



void performMove(Table &t, T_pos &r, T_pos &b, const string &sol, bool isPrint)
{
	int r_x = get<0>(r), r_y = get<1>(r);
	int newX = 0, newY = 0;
	for(auto c: sol)
	{
		if(c == 'd' || c == 'D')
			newY = 1;
		else if(c == 'u' || c == 'U')
			newY= -1;
		else newY=0;

		if(c == 'l' || c == 'L')
			newX= -1;
		else if(c == 'r' || c == 'R')
			newX = 1;
		else newX=0;

		r_y += newY;
		r_x += newX;

		if(t.at(r_y).at(r_x) == '$')
			t.at(r_y +newY).at(r_x + newX) = '$';

		t.at(r_y).at(r_x) = '@';
		t.at(r_y).at(r_x) = ' ';

		if(isPrint)
			printBoard(t);
	}
	get<0>(r) = r_x;
	get<1>(r) = r_y;
	get<0>(b) = r_x + newX;
	get<1>(b) = r_y + newY;
}


bool turn(const Table t, const Vec_t_pos rs, const Vec_t_pos bs, const Vec_t_pos ds, vector<vector<string> > &moves, size_t robot_nr)
{
	if(bs.size() == 0)
		return true;

	bool allSolved = false;

	size_t robot_nr_now = robot_nr + 1;
	if(robot_nr_now == rs.size()) robot_nr_now = 0;

	size_t starting_robot = robot_nr_now;

	do
	{
		for(size_t i = 0; i < bs.size(); i++)
		{
			for(size_t j = 0; j < ds.size(); i++)
			{
				Table t2(t);
				Vec_t_pos rs2(rs);
				Vec_t_pos bs2(bs);
				Vec_t_pos ds2(ds);
				size_t rx = get<0>(rs2.at(robot_nr_now)), ry = get<1>(rs2.at(robot_nr_now));
				allAsWalls(t2, rs2, bs2, ds2);
				setInterests(t2, rs2.at(robot_nr_now), bs2.at(i), ds2.at(j));

				Board b(rx, ry, t2);
				string sol = b.solve();

				if(sol.at(0) == 'N')
					continue; //can't solve

				allAsBefore(t2, rs2, bs2, ds2);
				performMove(t2, rs2.at(robot_nr_now), bs2.at(i), sol);
				negateInterest(t2, bs2.at(i), ds2.at(j));

				//printBoard(t2);

				//save to restore
				bs2.erase(bs2.begin() + i);
				ds2.erase(ds2.begin() + j);
				moves.at(robot_nr_now).push_back(sol);

				allSolved = turn(t2, rs2, bs2, ds2, moves, robot_nr_now);

				if(allSolved)
					return true;

				moves.at(robot_nr_now).pop_back();
			}
		}

		robot_nr_now++;
		robot_nr_now = (robot_nr_now == rs.size()) ? 0 : robot_nr_now;

	} while( robot_nr_now != starting_robot);

	return false;
}


void dynamicSolPrint(Table t){
	printBoard(t);
	Board b(0, 0, t);
	int r_x = b.px;
	int r_y = b.py;
	string solution = b.solve();

	if(solution.at(0) != 'N')
		for(auto c: solution){
			int newX = 0, newY=0;
			if(c == 'd' || c == 'D')
				newY = 1;
			else if(c == 'u' || c == 'U')
				newY= -1;
			else if(c == 'l' || c == 'L')
				newX= -1;
			else if(c == 'r' || c == 'R')
				newX = 1;

			if(t.at(r_y +newY).at(r_x + newX) == '$')
				t.at(r_y +newY + newY).at(r_x + newX + newX) = '$';

			t.at(r_y +newY).at(r_x + newX) = '@';
			t.at(r_y).at(r_x) = ' ';
			r_y += newY;
			r_x += newX;

			printBoard(t);
		}
	cout<<endl<<endl<<solution<<endl<<endl;
}




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

