#include "csoko_thinker/csoko_object.h"

#include <QPointF>

namespace csoko_thinker{
	/***************************************
	*
	*SokobanObject class
	*
	****************************************/
	CSokoObject::CSokoObject(int x, int y, bool isBox)
	{
		this->x = x;
		this->y = y;
		drawX = x;
		drawY = y;
		this->isBox = isBox;

		string s;
		if(!isBox)
			s = ;
		else
			s = "/home/viki/catkin_ws/src/cooperative_sokoban/csoko_images/box";
		QString images_path = QString::fromAscii(s.c_str(), s.length());
		this->icon.load(images_path);
		if (!icon.loadFromFile("/home/viki/catkin_ws/src/cooperative_sokoban/csoko_images/robot.png"))
		{
		    // error...
		}
	}
}
