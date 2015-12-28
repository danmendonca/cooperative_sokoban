#ifndef CSOKO_MAP
#define CSOKO_MAP

#include <QImage>

#include "csoko_thinker/csoko_frame.h"
#include "csoko_thinker/csoko_object.h"
#include "csoko_thinker/csoko_tile.h"
#include "csoko_thinker/csoko_box.h"
#include "csoko_thinker/csoko_robot.h"

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
