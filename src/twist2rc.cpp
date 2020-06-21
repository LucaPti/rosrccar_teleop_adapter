#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include "rosrccar_messages/RCControl.h"

/*
class SubscribeAndPublish
{
public:
  SubscribeAndPublish()
  {
    ros::Subscriber sub_ = n_.subscribe("key_vel", 1, &SubscribeAndPublish::callback, this);
    ros::Publisher pub_ = n_.advertise<rosrccar_messages::RCControl>("rc_output", 1);
    ROS_INFO("Subscribed to key_vel and publishing to rc_output");
  }

  void callback(const geometry_msgs::Twist& tw_msg)
  {
    rosrccar_messages::RCControl rc_msg;
    rc_msg.accelerator = tw_msg.linear.x; // usually 0.8
    rc_msg.steering = tw_msg.angular.z;  // between -1 and 1
    rc_msg.valid = true;
    pub_.publish(rc_msg);
    ROS_INFO("Relayed a message.");
  }

private:
  ros::NodeHandle n_; 
  ros::Publisher pub_;
  ros::Subscriber sub_;

};//End of class SubscribeAndPublish

int main(int argc, char **argv)
{
  //Initiate ROS
  ros::init(argc, argv, "twist2rc");

  //Create an object of class SubscribeAndPublish that will take care of everything
  SubscribeAndPublish SAPObject;

  ros::spin();

  return 0;
}
*/

float acceleration;
float steering;


void convertercallback(const geometry_msgs::Twist& tw_msg)
{
  rosrccar_messages::RCControl rc_msg;
  acceleration = (float)tw_msg.linear.x; // usually 0.8
  steering = (float)tw_msg.angular.z;  // between -1 and 1
}

int main(int argc, char **argv)
{
  //Initiate ROS
  ros::init(argc, argv, "twist2rc");
  ros::NodeHandle n_; 
  ros::Subscriber sub_ = n_.subscribe("key_vel", 1, convertercallback);
  ros::Publisher pub_ = n_.advertise<rosrccar_messages::RCControl>("rc_output", 1);
  ROS_INFO("Subscribed to key_vel and publishing to rc_output");

  ros::Rate loop_rate(100);

  int count = 0;
  while (ros::ok())
  {
    
    rosrccar_messages::RCControl rc_msg;
    rc_msg.accelerator = acceleration*0.125; // usually 0.8
    rc_msg.steering = steering;  // between -1 and 1
    rc_msg.valid = true;
    pub_.publish(rc_msg);

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }

  return 0;
}
