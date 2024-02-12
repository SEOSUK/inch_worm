#include "ros/ros.h"
#include <std_msgs/UInt16.h>
#include "rqt_mypkg/DockService.h"
#include <cstdlib> //atoll 쓰려고 넣어놈

//클라이언트로부터 값 받아서 그 값 퍼블리쉬로 서보 돌리기
int angle = 0;


bool Docking_Callback(rqt_mypkg::DockService::Request &req, rqt_mypkg::DockService::Response &res){
	if(req.Dock_Do){
		angle = 90;
		ROS_INFO("90");
	}
	else{
		angle = 0;
		ROS_INFO("0");
	}
	return true;
}

int main(int argc, char **argv)
 {
  ros::init(argc, argv, "turtle_client");


   ros::NodeHandle n;
  rqt_mypkg::DockService Service;
  std_msgs::UInt16 cmd_msg;

	ros::ServiceServer Server = n.advertiseService("/DockService", Docking_Callback); //ASDF
	ros::Publisher publisher = n.advertise<std_msgs::UInt16>("cmd_msg", 100);   //퍼블리셔 이름은 pubname

  ros::Rate rate(100);   

	while(ros::ok())
	{
    cmd_msg.data = angle;

	publisher.publish(cmd_msg);
	ros::spinOnce();
	rate.sleep();
	}



 return 0;
 }
