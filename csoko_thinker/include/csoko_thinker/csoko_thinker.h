#ifndef CSOKO_THINKER
#define CSOKO_THINKER


#include <iostream>
#include <cstdlib>
#include <cmath>

#include <ros/package.h>
#include "ros/ros.h"

#include <stdr_msgs/RobotIndexedVectorMsg.h>

#include <geometry_msgs/Pose2D.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/OccupancyGrid.h>
#include <sensor_msgs/LaserScan.h>
#include <sensor_msgs/Range.h>

#include <iostream>
#include <fstream>


namespace csoko_thinker
{

class CSoko_Thinker
{
private:

	const static bool CSOKO_THINKER_DEBUG = true;
	int odom_state;
	int8_t **my_map;
	int map_height, map_width;


	/********************************************
	 * Subscribers
	 *******************************************/

	ros::Subscriber odom_sub;

	ros::Subscriber occ_grid_sub;

	ros::Subscriber laser_sub;


	/********************************************
	 * Publishers
	 *******************************************/

	ros::Publisher cmd_vel_pub;


	/********************************************
	 *Topics
	 *******************************************/

	std::string occ_grid_topic;

	std::string odom_topic;

	std::string laser_topic;

	std::string speeds_topic;


	/********************************************
	 * Messages
	 *******************************************/

	nav_msgs::Odometry odom_msg;

	nav_msgs::OccupancyGrid occ_grid_msg;

	sensor_msgs::LaserScan laser_scan_msg;


	/********************************************
	 * Node Handler
	 *******************************************/
	//!< The ROS node handle
	ros::NodeHandle nh;


public:

	CSoko_Thinker(int argc,char **argv);

	~CSoko_Thinker(void);

	void mapCallback(const nav_msgs::OccupancyGrid& msg);

	void callback(const sensor_msgs::LaserScan& msg);

	void odometryCallback(const nav_msgs::Odometry msg);

};
}

#endif
