#ifndef CSOKO_MAP
#define CSOKO_MAP

#include <QImage>

#include "csoko_thinker/csoko_frame.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <cstdlib>
#include <ctime>

#include <QFrame>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <QVector>


using namespace std;

namespace csoko_thinker
{

class CSokoObject
{
	public:
		QImage icon;
		int x,y;
		double drawX, drawY;

		CSokoObject(int x, int y);
};

class CSokoRobot : public CSokoObject
{
	public:
	CSokoRobot(int x, int y);
};

class CSokoBox : public CSokoObject
{
	public:
	CSokoBox(int x, int y);
};

class CSokoTile
{
	public:
		int x,y;
		CSokoObject object;
		bool hasObject, isGoal, isSolid;
		
		CSokoTile(int x, int y, bool isGoal, bool isSolid);
		void setObject(CSokoObject object);
};


class CSokoMap
{
	public:
		string mapName;
		QImage bg, goal;
		vector<vector<CSokoTile> > grid;	
		CSokoMap(string mapName);
		void drawAll(CSokoFrame frame);
};



}
#endif
