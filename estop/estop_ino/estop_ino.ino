#include <ros.h>
#include <ArduinoHardware.h>
#include <kobuki_msgs/MotorPower.h>
#include <std_msgs/String.h>

ros::NodeHandle nh;
kobuki_msgs::MotorPower motor_cmd;
ros::Publisher publish_emergency_cmd("/estop_enabled",&motor_cmd);

void setup(){
  nh.initNode();
  nh.advertise(publish_emergency_cmd);
}

void loop(){
  motor_cmd.state = kobuki_msgs::MotorPower::ON;
  publish_emergency_cmd.publish(&motor_cmd);
  motor_cmd.state = kobuki_msgs::MotorPower::OFF;
  publish_emergency_cmd.publish(&motor_cmd);
  nh.spinOnce();
}
