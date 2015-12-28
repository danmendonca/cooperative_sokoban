#ifndef CSOKO_TILE
#define CSOKO_TILE

#include <QImage>

#include "csoko_thinker/csoko_frame.h"
#include "csoko_thinker/csoko_object.h"

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
	class CSokoTile
	{
		public:
			int x,y;
			CSokoObject object;
			bool hasObject, isGoal, isSolid;
		
			CSokoTile(int x, int y, bool isGoal, bool isSolid);
			void setObject(CSokoObject object);
	};
}
#endif
