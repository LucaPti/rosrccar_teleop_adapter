#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include "rosrccar_messages/RCControl.h"

float acceleration;
float steering;
ros::Publisher pub_;

void convertercallback(const geometry_msgs::Twist& tw_msg)
{
  rosrccar_messages::RCControl rc_msg;
  acceleration = (float)tw_msg.linear.x; // usually 0.8
  steering = (float)tw_msg.angular.z;  // between -1 and 1
//   rosrccar_messages::RCControl rc_msg;
  rc_msg.accelerator = acceleration*0.3; // usually 0.8
  rc_msg.steering = -steering;  // between -1 and 1
  rc_msg.valid = true;
  pub_.publish(rc_msg);
}

int main(int argc, char **argv)
{
  //Initiate ROS
  ros::init(argc, argv, "twist2rc");
  ros::NodeHandle n_; 
  ros::Subscriber sub_ = n_.subscribe("cmd_vel", 1, convertercallback);
  pub_ = n_.advertise<rosrccar_messages::RCControl>("rc_output", 1);
  ROS_INFO("Subscribed to cmd_vel and publishing to rc_output");
/*
  ros::Rate loop_rate(100);

  int count = 0;
  while (ros::ok())
  {
    
    rosrccar_messages::RCControl rc_msg;
    rc_msg.accelerator = acceleration*0.25; // usually 0.8
    rc_msg.steering = steering;  // between -1 and 1
    rc_msg.valid = true;
    pub_.publish(rc_msg);

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }
*/
  ros::spin();
  return 0;
}
