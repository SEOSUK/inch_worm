

#include <ros/ros.h>
#include <std_msgs/UInt16.h>
#include <iostream>

void switch_callback(const std_msgs::UInt16 &str_msg)
{
		ROS_INFO("%d", str_msg.data);
}


// rostopic pub -1 /servo std_msgs/UInt16 "90" //각도       
//servo std_msgs : publish 할 토픽.          geometry_msgs/Twist : 메세지 타입
int main(int argc, char **argv)
{
	ros::init(argc, argv, "turtle_Pose");    

	ros::NodeHandle n;


	ros::Subscriber subscriber = n.subscribe("message", 1000, switch_callback);  

	ros::spin();

	return 0;
}









// rostopic pub -1 /servo std_msgs/UInt16 "90" //각도
//rostopic echo /message //도킹상태

/*
#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include <Servo.h>
#include <ros.h>
#include <std_msgs/UInt16.h>

ros::NodeHandle nh;

Servo servo1;
Servo servo2;

int btn = 8;

void servo_cb(const std_msgs::UInt16 &cmd_msg)
{
servo1.write(cmd_msg.data); // set servo angle, should be from 0-180
servo2.write(cmd_msg.data);
// digitalWrite(13, HIGH-digitalRead(13)); //toggle led
}

ros::Subscriber<std_msgs::UInt16> sub("servo", servo_cb); :: 서보 돌리기
std_msgs::UInt16 isdock
ros::Publisher message("message", &isdock);


void setup()
{
// pinMode(13, OUTPUT);
pinMode(btn, INPUT);

nh.initNode();
nh.subscribe(sub);
nh.advertise(message);

servo1.attach(9); // attach it to pin 9
servo2.attach(6);
}


void loop()
{
if (digitalRead(btn) == LOW)
{
isdock = 0;
message.publish(&isdock);
}
else
{
isdock = 1;
message.publish(&isdock);
}
nh.spinOnce();
delay(1);
}

// alias adrun='rosrun rosserial_server serial_node _port:=/dev/ttyACM0'*/


//이거 돌려서 스위치상태 0 과 1로 출력되는지 확인.