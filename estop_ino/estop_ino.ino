#include <ros.h>
#include <ArduinoHardware.h>
#include <kobuki_msgs/MotorPower.h>
#include <std_msgs/String.h>


int estop_switch_pin = 7;
int estop_status_pin = 13;
static  int estop_switch_status = 0; // 0- estop disengaged (motors running)  1- estop engaged (motors stopped)

ros::NodeHandle nh;
kobuki_msgs::MotorPower sent_cmd;
kobuki_msgs::MotorPower user_cmd;
ros::Publisher publish_emergency_cmd("/mobile_base/commands/motor_power",&motor_cmd);

void callback_keyop(const kobuki_msgs::MotorPower &teleop_msg){
  user_cmd.state = teleop_msg.state;
  return;
}

ros::Subscriber<kobuki_msgs::MotorPower>subscribe_user_cmd("/ninja_pandavs/teleop/motor_power",&callback_keyop);

void setup(){
  nh.initNode();
  nh.advertise(publish_emergency_cmd);
  nh.subscribe(subscribe_user_cmd);
  pinMode(estop_switch_pin,INPUT);
  pinMode(estop_status_pin,OUTPUT);
}  

void loop(){
  estop_switch_status = digitalRead(estop_switch_pin);
  
  if (estop_switch_status){
      sent_cmd.state = kobuki_msgs::MotorPower::OFF;
  }
  else {
      sent_cmd.state = user_cmd.state;
  }
  
  digitalWrite(estop_status_pin,estop_switch_status);  
  publish_emergency_cmd.publish(&sent_cmd);
  nh.spinOnce();
}
