#include "csoko_thinker/csoko_frame.h"

#include <QPointF>

/**
 * Load map, create frame, start timer
 */
namespace csoko_thinker{

CSokoFrame::CSokoFrame() : QFrame(0, 0)
, frame_count_(0)
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "CSOKO");

	srand(time(NULL));
	update_timer_ = new QTimer(this);
	update_timer_->setInterval(16);
	update_timer_->start();
	connect(update_timer_, SIGNAL(timeout()), this, SLOT(&CSokoFrame::onUpdate()));

	string goalPath = "/home/viki/catkin_ws/src/cooperative_sokoban/csoko_images/goal";
	QString images_path = QString::fromAscii(goalPath.c_str(), goalPath.length());
	this->goal.load(images_path);
	ROS_ERROR("FRAME IS LOADED");
	update();
}

CSokoFrame::CSokoFrame(QWidget * parent) : QFrame(0, 0)
, frame_count_(0)
{
}

void CSokoFrame::clear()
{
}

void CSokoFrame::closeWindow()
{
	window.close();
}

void CSokoFrame::loadMap(string mapName)
{
	string bgPath = "/home/viki/catkin_ws/src/cooperative_sokoban/csoko_resources/maps/"+mapName;
	QString images_path = QString::fromAscii(bgPath.c_str(), bgPath.length());
	this->bg.load(images_path);

	if(bg.isNull())
		ROS_ERROR("FUCK");
	else
		ROS_ERROR("COOL");

	update();
}


void CSokoFrame::onUpdate()
{
	ROS_ERROR("ONUPDTAE CALL");
}

void CSokoFrame::paintEvent(QPaintEvent*e)
{
	QPainter painter(this);

	//Draw Background
	if(!bg.isNull())
		painter.drawImage(QPointF(0,0),bg);

	//Draw Goal
	for(int i=0;i<grid.size();i++)
	{
		for(int j=0;j<grid[i].size();j++)
		{
			CSokoTile tile = grid[i][j];
			if(tile.isGoal)
			{
				if(!goal.isNull())
					painter.drawImage(QPointF(j*16,i*16),goal);
			}
		}
	}

	//Draw Robots and Boxes
	for(int j=0;j<objects.size();j++)
	{
		painter.drawImage(QPointF(objects[j].drawX*16,objects[j].drawY*16),objects[j].icon);
	}
}

void CSokoFrame::signalUpdate(vector<vector<CSokoTile> > grid, vector<CSokoObject> objects)
{
	this->grid = grid;
	this->objects = objects;
	update();
}

void CSokoFrame::draw(QImage img, QPointF pos)
{
	ROS_ERROR("ASDFGHJHHGGFDS");
//	viewport()->update();
	//QPainter painter(0);
//	QPainter painter(this);
//	painter.begin( this );
//	QPointF p = pos* img.height();
//	painter.drawImage(p,img);
//	painter.end( );
}

}
