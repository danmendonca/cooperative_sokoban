#include "csoko_thinker/csoko_robot.h"

#include <QPointF>

namespace csoko_thinker{
	CSoko_Robot::CSoko_Robot(int x, int y)
	{
		this->x = x;
		this->y = y;
		string s = ros::package::getPath("csoko_thinker") + "/csoko_images/" + "robot";
		QString images_path = QString::fromAscii(s.c_str(), s.length());
		this->icon.load(images_path);
	}
}
