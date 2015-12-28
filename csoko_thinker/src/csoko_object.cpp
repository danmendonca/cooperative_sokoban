#include "csoko_thinker/csoko_object.h"

#include <QPointF>

namespace csoko_thinker{

	/***************************************
	*
	*SokobanObject class
	*
	****************************************/
	CSokoObject::CSokoObject(int x, int y)
	{
		this->x = x;
		this->y = y;
		drawX = x;
		drawY = y;
	}
	
	CSokoBox::CSokoBox(int x, int y) : CSokoObject(x,y)
	{
		string s = ros::package::getPath("csoko_thinker") + "/csoko_images/" + "box";
		QString images_path = QString::fromAscii(s.c_str(), s.length());
		this->icon.load(images_path);
	}
}
