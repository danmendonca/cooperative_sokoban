#ifndef CSOKO_FRAME
#define CSOKO_FRAME


#include <iostream>
#include <cstdlib>
#include <cmath>

#include <ros/package.h>
#include "ros/ros.h"

#include <stdr_msgs/RobotIndexedVectorMsg.h>

#include "csoko_thinker/csoko_map.h"

#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/OccupancyGrid.h>
#include <sensor_msgs/LaserScan.h>
#include <sensor_msgs/Range.h>

#include <QFrame>
#include <QImage>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include <QColor>
#include <QRgb>

#include <map>

#include <iostream>
#include <fstream>

using namespace std;

namespace csoko_thinker
{

class CSokoFrame : public QFrame
{
	Q_OBJECT
	public:
		CSokoFrame(QWidget* parent = 0, Qt::WindowFlags f = 0);
		uint64_t frame_count_;
		void draw(QImage img, QPointF pos);
};
}
#endif
