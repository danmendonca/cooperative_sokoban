#ifndef CSOKO_ROBOT
#define CSOKO_ROBOT

#include <QImage>

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
	class CSoko_Robot : public CSokoObject
	{
	public:
		CSoko_Robot(int x, int y);
	};
}
#endif
