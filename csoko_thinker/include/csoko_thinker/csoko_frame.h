#ifndef CSOKO_FRAME
#define CSOKO_FRAME


#include <iostream>
#include <cstdlib>
#include <cmath>

#include <ros/package.h>
#ifndef Q_MOC_RUN // See: https://bugreports.qt-project.org/browse/QTBUG-22829
#include <ros/ros.h>

#include "csoko_thinker/csoko_object.h"
#include "csoko_thinker/csoko_tile.h"

#include <stdr_msgs/RobotIndexedVectorMsg.h>

#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/OccupancyGrid.h>
#include <sensor_msgs/LaserScan.h>
#include <sensor_msgs/Range.h>

#include <QFrame>
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include <QColor>
#include <QRgb>
#include <QTimer>
#include <QPaintEvent>

#include <map>

#include <iostream>
#include <fstream>

using namespace std;

namespace csoko_thinker
{
class CSokoFrame : public QFrame
{
	public:
		CSokoFrame();
		CSokoFrame(QWidget * parent);
		virtual ~CSokoFrame() {};

		void draw(QImage img, QPointF pos);
		void paintEvent(QPaintEvent * e);
		void signalUpdate(vector<vector<CSokoTile> > grid, vector<CSokoObject> objects);
		void loadMap(string mapName);

		void closeWindow();
	protected:
	private slots:
		void onUpdate();
	private:
		void clear();
		uint64_t frame_count_;
		QTimer* update_timer_;
		QImage bg;
		QImage curImg;
		QPointF curPos;

		vector<CSokoObject> objects;
		vector<vector<CSokoTile> > grid;
		QImage goal;
};
}
#endif
#endif
