#ifndef CSOKO_OBJECT
#define CSOKO_OBJECT

#include <QImage>

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
}
#endif
