#include "csoko_thinker/csoko_box.h"

#include <QPointF>

namespace csoko_thinker{
	CSokoBox::CSokoBox(int x, int y)
	{
		this->x = x;
		this->y = y;
		string s = ros::package::getPath("csoko_thinker") + "/csoko_images/" + "box";
		QString images_path = QString::fromAscii(s.c_str(), s.length());
		this->icon.load(images_path);
	}
}
