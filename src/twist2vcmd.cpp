#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include "rosrccar_messages/VehicleCommand.h"

float acceleration;
float steering;
ros::Publisher pub_;
int idletime;

void convertercallback(const geometry_msgs::Twist& tw_msg)
{
  rosrccar_messages::VehicleCommand vcmd_msg;
  acceleration = (float)tw_msg.linear.x; // usually 0.8
  steering = (float)tw_msg.angular.z;  // between -1 and 1
  vcmd_msg.target_lon = acceleration*1e3; // usually 0.8
  vcmd_msg.target_lat = -steering*1e3;  // between -1 and 1
  if((acceleration==0)&&(steering==0)){
    idletime = idletime +1;
  }
  else {
    idletime = 0;
  }
  if(idletime<100){
    vcmd_msg.operationmode_lon = 4; //"automated" mode
    vcmd_msg.operationmode_lat = 4; //"automated" mode
  }
  else {
    vcmd_msg.operationmode_lon = 1; //"manual" mode
    vcmd_msg.operationmode_lat = 1; //"manual" mode
  }
  pub_.publish(vcmd_msg);
}

int main(int argc, char **argv)
{
  //Initiate ROS
  ros::init(argc, argv, "twist2vcmd");
  ros::NodeHandle n_; 
  ros::Subscriber sub_ = n_.subscribe("cmd_vel", 1, convertercallback);
  pub_ = n_.advertise<rosrccar_messages::VehicleCommand>("vehicle_command", 1);
  ROS_INFO("Subscribed to cmd_vel and publishing to vehicle_command");
  idletime = 0;
  ros::spin();
  return 0;
}
