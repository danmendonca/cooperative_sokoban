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
#include <sensor_msgs/LaserScan.h>
#include <sensor_msgs/Range.h>


namespace csoko_thinker
{
 
  class CSoko_Thinker
  {
    private:
    
      int odom_state;
    
      //!< The robot odometry msg
      nav_msgs::Odometry odom_msg;
      
      ros::Subscriber odom_sub;
      
      std::string odom_topic;
      
      //!< The ros laser scan msg
      sensor_msgs::LaserScan scan_;
      
      //!< Subscriber for the ros laser msg
      ros::Subscriber subscriber_;
      
      //!< The ROS node handle
      ros::NodeHandle n_;
      
      //!< The laser topic
      std::string laser_topic_;
      
      //!< The speeds topic
      std::string speeds_topic_;
      
      //!< The twist publisher
      ros::Publisher cmd_vel_pub_;
      
    public:
    
      CSoko_Thinker(int argc,char **argv);
      
      ~CSoko_Thinker(void);
      
      void callback(const sensor_msgs::LaserScan& msg);
      
      void odometryCallback(const nav_msgs::Odometry msg);
      
  };
}

#endif