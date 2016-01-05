#include "csoko_thinker/csoko_solver.h"

using namespace std;


bool Board::setRobot(size_t xx, size_t yy)
{
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


Board::Board(string b)
{
	vector<string> data;
	boost::split(data, b, boost::is_any_of("\n"));

	//save higher length in width
	size_t width = 0;
	for (auto &row: data)
		width = max(width, row.size());

	map<char,char> maps = {{' ',' '}, {'.','.'}, {'@',' '},	{'#','#'}, {'$',' '}},
			mapd = {{' ',' '}, {'.',' '}, {'@','@'}, {'#',' '}, {'$','*'}};

	for (size_t r = 0; r < data.size(); r++)
	{
		TableRow sTemp, dTemp;
		for (size_t c = 0; c < width; c++)
		{
			char ch = c < data[r].size() ? data[r][c] : ' ';
			sTemp.push_back(maps[ch]);
			dTemp.push_back(mapd[ch]);
			if (ch == '@')
			{
				px = c;
				py = r;
			}
		}
		sData.push_back(sTemp);
		dData.push_back(dTemp);
	}
}



bool Board::move(int x, int y, int dx, int dy, Table &data)
{
	if (sData[y+dy][x+dx] == '#' || data[y+dy][x+dx] != ' ')
		return false;

	data[y][x] = ' ';
	data[y+dy][x+dx] = '@';
	return true;
}

bool Board::push(int x, int y, int dx, int dy, Table &data)
{
	if (sData[y+2*dy][x+2*dx] == '#' || data[y+2*dy][x+2*dx] != ' ')
		return false;

	data[y][x] = ' ';
	data[y+dy][x+dx] = '@';
	data[y+2*dy][x+2*dx] = '*';
	return true;
}

bool Board::isSolved(const Table &data)
{
	for (size_t r = 0; r < data.size(); r++)
		for (size_t c = 0; c < data[r].size(); c++)
			if ((sData[r][c] == '.') != (data[r][c] == '*'))
				return false;
	return true;
}

string Board::solve()
{
	boost::unordered_set<Table, boost::hash<Table>> visited;
	visited.insert(dData);

	queue<tuple<Table, string, int, int> > open;
	open.push(make_tuple(dData, "", px, py));

	vector<tuple<int, int, char, char>> dirs =
	{
			make_tuple( 0, -1, 'u', 'U'),
			make_tuple( 1,  0, 'r', 'R'),
			make_tuple( 0,  1, 'd', 'D'),
			make_tuple(-1,  0, 'l', 'L')
	};

	while (open.size() > 0)
	{

		//extracts open head tuple elements and then pop that tuple
		Table temp, cur = get<0>(open.front());
		string cSol = get<1>(open.front());
		int x = get<2>(open.front());
		int y = get<3>(open.front());
		open.pop();

		//for each possible movement
		for (int i = 0; i < 4; ++i)
		{
			temp = cur;
			int dx = get<0>(dirs[i]);
			int dy = get<1>(dirs[i]);

			if (temp[y+dy][x+dx] == '*')
			{
				if (push(x, y, dx, dy, temp) &&	visited.find(temp) == visited.end())
				{
					if (isSolved(temp))
						return cSol + get<3>(dirs[i]);
					open.push(make_tuple(temp, cSol + get<3>(dirs[i]), x+dx, y+dy));
					visited.insert(temp);
				}
			} else if (move(x, y, dx, dy, temp) && visited.find(temp) == visited.end())
			{
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



string printBoard(const Table &t)
{
	string board = "";
	for(auto row: t)
	{
		for(auto c : row){
			cout<<c;
			board += c;
		}
		cout<<endl;
		board+='\n';
	}
	board+='\0';
	return board;
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



void performAllMoves(Table &t, T_pos &r, T_pos &b, const string &sol, bool isPrint)
{
	//Vec_t_pos bPoses;
	for(auto c: sol)
	{
		performOneMove(t, r, b, c);

		if(isPrint)
			printBoard(t);
	}
	//bPoses.push_back(b);

	//return bPoses;
}

void performOneMove(Table &t, T_pos &r, T_pos &b, const char &c)
{
	int r_x = get<0>(r), r_y = get<1>(r);
	int newX = 0, newY = 0;

	if(c == 'd' || c == 'D') newY = 1;
	else if(c == 'u' || c == 'U') newY= -1;
	else newY=0;

	if(c == 'l' || c == 'L') newX= -1;
	else if(c == 'r' || c == 'R') newX = 1;
	else newX=0;

	//clean robot previous position
	t.at(r_y).at(r_x) = ' ';

	//update robot coord to next pos
	r_y += newY;
	r_x += newX;

	//verify if next robot pos has box, and move it if has
	if(t.at(r_y).at(r_x) == '$')
	{
		//bPoses.push_back(make_tuple(r_x,r_y));
		t.at(r_y +newY).at(r_x + newX) = '$';
		get<0>(b) = r_x + newX;
		get<1>(b) = r_y + newY;
	}

	//place robot in new coords
	t.at(r_y).at(r_x) = '@';

	get<0>(r) = r_x;
	get<1>(r) = r_y;
}

void getMovementDelta(char c, int &dx, int &dy)
{
	if(c == 'd' || c == 'D') dy = 1;
	else if(c == 'u' || c == 'U') dy= -1;
	else dy=0;

	if(c == 'l' || c == 'L') dx= -1;
	else if(c == 'r' || c == 'R') dx = 1;
	else dx=0;
}


bool turn(const Table t, const Vec_t_pos rs, const Vec_t_pos bs, const Vec_t_pos ds, V_Robot_Move &moves, size_t robot_nr)
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
			for(size_t j = 0; j < ds.size(); j++)
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
				performAllMoves(t2, rs2.at(robot_nr_now), bs2.at(i), sol);
				negateInterest(t2, bs2.at(i), ds2.at(j));

				//printBoard(t2);

				//save to restore
				bs2.erase(bs2.begin() + i);
				ds2.erase(ds2.begin() + j);
				Robot_Move a_move = make_tuple(robot_nr_now, sol);
				moves.push_back(a_move);

				allSolved = turn(t2, rs2, bs2, ds2, moves, robot_nr_now);

				if(allSolved)
					return true;

				moves.pop_back();
			}
		}

		robot_nr_now++;
		robot_nr_now = (robot_nr_now == rs.size()) ? 0 : robot_nr_now;

	} while( robot_nr_now != starting_robot);

	return false;
}
