/*
 * yo_shit.cpp
 *
 *  Created on: Dec 28, 2015
 *      Author: dmendonca
 */

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

using namespace std;

typedef vector<char> TableRow;
typedef vector<TableRow> Table;

struct Board {
	Table sData, dData;
	int px, py;


	bool setRobot(int xx, int yy){
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

	void setMap(Table t, bool toSetRobot=false)
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

	void setRobotAndMap(int xx, int yy, Table t)
	{
		bool goodRobot = setRobot(xx, yy);
		setMap(t, !goodRobot);

	}


	Board(int xx, int yy, Table t)
	{
		setRobotAndMap(xx, yy, t);
	}


	Board(string b) {
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



	bool move(int x, int y, int dx, int dy, Table &data) {
		if (sData[y+dy][x+dx] == '#' || data[y+dy][x+dx] != ' ')
			return false;

		data[y][x] = ' ';
		data[y+dy][x+dx] = '@';
		return true;
	}

	bool push(int x, int y, int dx, int dy, Table &data) {
		if (sData[y+2*dy][x+2*dx] == '#' || data[y+2*dy][x+2*dx] != ' ')
			return false;

		data[y][x] = ' ';
		data[y+dy][x+dx] = '@';
		data[y+2*dy][x+2*dx] = '*';
		return true;
	}

	bool isSolved(const Table &data) {
		for (size_t r = 0; r < data.size(); r++)
			for (size_t c = 0; c < data[r].size(); c++)
				if ((sData[r][c] == '.') != (data[r][c] == '*'))
					return false;
		return true;
	}

	string solve() {
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
};

void printBoard(Table t)
{
	for(auto row: t)
	{
		for(auto c : row)
			cout<<c;
		cout<<endl;
	}
}

void dynamicSolPrint(Table t){
	printBoard(t);
			Board b(0, 0, t);
			int r_x = b.px;
			int r_y = b.py;
			string solution = b.solve();

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
			cout<<endl<<endl<<solution;
}
/*
int main() {
	Table t;
	ifstream ifs("sokobanMap1");
	if(ifs.is_open())
	{
		string line;
		while(getline(ifs, line))
		{
			TableRow row;
			for(auto c : line)
			{
				row.push_back(c);
			}
			t.push_back(row);
		}

		ifs.close();
		dynamicSolPrint(t);

	}

	return 0;
}
*/

