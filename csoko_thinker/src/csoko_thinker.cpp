#include "csoko_thinker/csoko_thinker.h"

int main(int argc,char **argv)
{
  ros::init(argc, argv, "csoko_thinker_node", ros::init_options::AnonymousName);
  csoko_thinker::CSoko_Thinker obj(argc, argv);
  ros::spin();
  return 0;
}

namespace csoko_thinker{
 
  CSoko_Thinker::CSoko_Thinker(int argc,char **argv)
  {
    if(argc != 3)
    {
      ROS_ERROR(
        "Usage : csoko_thinker csoko_thinker <robot_frame_id> <laser_frame_id>");
      exit(0);
    }
    
    std::string robot_topic = std::string("/") + std::string(argv[1]);
    
    laser_topic_ = robot_topic + std::string("/") + std::string(argv[2]);
    speeds_topic_ = robot_topic + std::string("/cmd_vel");
    odom_topic = robot_topic +  std::string("/odom");

    odom_state = 0;
    
    subscriber_ = n_.subscribe(laser_topic_.c_str(), 1, &CSoko_Thinker::callback, this);
    odom_sub =   n_.subscribe(odom_topic.c_str(), 1, &CSoko_Thinker::odometryCallback, this);
    
    cmd_vel_pub_ = n_.advertise<geometry_msgs::Twist>(speeds_topic_.c_str(), 1);
  }
  

  CSoko_Thinker::~CSoko_Thinker(void)
  {
    
  }
  

  void CSoko_Thinker::callback(const sensor_msgs::LaserScan& msg)
  {
    scan_ = msg;
    float linear = 0, rotational = 0;
    for(unsigned int i = 0 ; i < scan_.ranges.size() ; i++)
    {
      float real_dist = scan_.ranges[i];
      linear -= cos(scan_.angle_min + i * scan_.angle_increment) 
        / (1.0 + real_dist * real_dist);
      rotational -= sin(scan_.angle_min + i * scan_.angle_increment) 
        / (1.0 + real_dist * real_dist);
    }
    geometry_msgs::Twist cmd;
    
    linear /= scan_.ranges.size();
    rotational /= scan_.ranges.size();
    
    //~ ROS_ERROR("%f %f",linear,rotational);
    
    if(linear > 0.3)
    {
      linear = 0.3;
    }
    else if(linear < -0.3)
    {
      linear = -0.3;
    }

    cmd.linear.x = (odom_state < 3) ? 0.3 + linear : 0;
    cmd.angular.z = (odom_state > 1) ? 0.0174532925 * 10 : rotational;
    cmd_vel_pub_.publish(cmd);
  }
  
  
  void CSoko_Thinker::odometryCallback(const nav_msgs::Odometry msg){
    
    
    
    if(odom_state == 0){
      odom_msg = msg;
      odom_state = 1;
      return;
    }
    
    float currentXPos = msg.pose.pose.position.x, 
          currentYPos = msg.pose.pose.position.y,
          oldXPos = odom_msg.pose.pose.position.x, 
          oldYPos = odom_msg.pose.pose.position.y;
    float delta = 0.0000001;
    
    bool stuckX = ((currentXPos - oldXPos) == 0) ? true : false;
    bool stuckY = ((currentYPos - oldYPos) == 0) ? true: false;
    
    float dx = std::abs(currentXPos - oldXPos),
          dy = std::abs(currentYPos - oldYPos);
           
     if(stuckX && stuckY){
        
        if(odom_state == 1) odom_state = 2;
        else if(odom_state == 2)  odom_state = 3;
        else if(odom_state == 3) odom_state = 2;
        
        ROS_INFO("dx = %f ; dy =  %f ; odomState= %i", dx, dy, odom_state);
     }
    else{
      odom_state = 1;
      ROS_INFO("dx = %f ; dy =  %f ; odomState= %i", dx, dy, odom_state);
    }  
    //odom_msg.pose.pose.position.z = 0.0;
      
    //save it 
    odom_msg = msg;  
    
  }
  
}