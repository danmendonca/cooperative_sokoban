#ifndef CSOKO_THINKER__H
#define CSOKO_THINKER__H


#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <tuple>
#include <SFML/Graphics.hpp>


#include <ros/package.h>
#include <ros/console.h>
#include <stdr_msgs/RobotIndexedVectorMsg.h>
#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/OccupancyGrid.h>
#include <sensor_msgs/LaserScan.h>
#include <sensor_msgs/Range.h>
#include "ros/ros.h"
#include "csoko_thinker/csoko_frame.h"
#include "csoko_thinker/csoko_object.h"
#include "csoko_thinker/csoko_tile.h"
#include "csoko_thinker/csoko_solver.h"
namespace csoko_thinker
{


typedef std::tuple<size_t, size_t> index_matcher;
typedef std::tuple<size_t, size_t> XY_COORD;


class CSoko_Thinker
{
private:
	void updateMap();
	void update();
	void onUpdate(const ros::TimerEvent& e);


	std::vector<CSokoObject> objects;
	std::vector<index_matcher> objPos_rob_index;
	V_Robot_Move moves, current_moves;
	std::vector<XY_COORD> robots_pos, boxes_pos, deliverys_pos;
	Table map_table;
	//int odom_state;
	int8_t **my_map;
	int map_height, map_width;


	/********************************************
	 * Node Handler
	 *******************************************/
	//!< The ROS node handle
	ros::NodeHandle nh;


public:
	const static bool CSOKO_THINKER_DEBUG = false;
	CSoko_Thinker();
	CSoko_Thinker(int argc,char **argv);

	~CSoko_Thinker(void);

	void timerCallback(const ros::TimerEvent& e);

	CSokoFrame frame;
	std::string mapName;
	std::vector<std::vector<CSokoTile> > grid;
	void loadMap(std::string mapName);
	void drawAll();
	int getBoxPosByCoord(int x, int y);
	int getRobotPosByNo(int number);
	bool robotInUse(const std::vector<std::tuple<size_t, std::string> > &mvs, const std::tuple<size_t, std::string> &mv);
	void moveRobotOnce(size_t r_index);
	size_t matchRobotObj(size_t r_index);
	size_t matchObjRobot(size_t obj_index);
	std::vector<size_t> robotsToMoveNow();
	void updateCurrentMovements();
	bool lockPath(const Robot_Move &r_mv);
	bool lockPath(const size_t &r_nr, const XY_COORD &xy, std::string &movement);

};
}

#endif
