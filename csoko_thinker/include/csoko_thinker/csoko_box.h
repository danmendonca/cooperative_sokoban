#ifndef CSOKO_BOX
#define CSOKO_BOX

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
	class CSokoBox : public CSokoObject
	{
	public:
		CSokoBox(int x, int y);
	};
}
#endif
