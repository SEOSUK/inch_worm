#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include "std_msgs/Int16.h"
#include "omni_msgs/OmniState.h"
#include "omni_msgs/OmniFeedback.h"
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/WrenchStamped.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Twist.h>
#include <signal.h>
#include <cmath>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>

#define PI 3.14159256359

ros::Publisher force_feedback;
ros::Publisher haptic_dasom_command_pub_;
ros::Publisher haptic_palletrone_command_pub_;

geometry_msgs::Twist xyzrpy;
geometry_msgs::Twist dasom_xyzrpy;
geometry_msgs::Twist palletrone_xyzrpy;

geometry_msgs::PoseStamped pose_msg;
geometry_msgs::PoseStamped pose_msg_i;

omni_msgs::OmniFeedback ds_feedback;

double core_x = 0;
double core_y = 0;
double threshold_r = 100;
float threshold_force = 2.0;
float threshold_force_x = 0;
float threshold_force_y = 0;

double current_position_r = 0;
double current_position_x = 0;
int current_position_y = 0;

Eigen::Matrix3d RotZ;
Eigen::Vector3d eigen_xyzrpy;
Eigen::Vector3d eigen_palletrone_xyzrpy;

void commandGenerator()
{
	if(abs(pose_msg.pose.position.x - pose_msg_i.pose.position.x) < 0.01) 
    	xyzrpy.linear.x = pose_msg.pose.position.x;
    if(abs(pose_msg.pose.position.y - pose_msg_i.pose.position.y) < 0.01) 
    	xyzrpy.linear.y = pose_msg.pose.position.y;
    if(abs(pose_msg.pose.position.z - pose_msg_i.pose.position.z) < 0.01) 
    	xyzrpy.linear.z = pose_msg.pose.position.z;
    
    xyzrpy.angular.x = 0;
    xyzrpy.angular.y = 0;
    xyzrpy.angular.z = 0;

	eigen_xyzrpy[0] = xyzrpy.linear.x;
	eigen_xyzrpy[1] = xyzrpy.linear.y;
	eigen_xyzrpy[2] = xyzrpy.linear.z;

    pose_msg_i = pose_msg;
}

void stateCallback(const omni_msgs::OmniState::ConstPtr& msg)
{
	current_position_x = msg->pose.position.x;
	current_position_y = (int) -(msg->pose.position.y);
	current_position_r = sqrt(pow(current_position_x,2) + pow(current_position_y,2));

	pose_msg.pose.position.x = msg->pose.position.x / 1000.0 / 2.5;
    pose_msg.pose.position.y = -msg->pose.position.y / 1000.0 / 2.5;
    pose_msg.pose.position.z = msg->pose.position.z / 1000.0 / 1.5;

	commandGenerator();

	RotZ << cos(-PI/4),   -sin(-PI/4),        0,
			sin(-PI/4),    cos(-PI/4),        0,
				 0,             0,            1;

	eigen_palletrone_xyzrpy = RotZ * eigen_xyzrpy;
	eigen_palletrone_xyzrpy[1] = -eigen_palletrone_xyzrpy[1];

	double palletrone_mag = sqrt(eigen_palletrone_xyzrpy[1] * eigen_palletrone_xyzrpy[1] + eigen_palletrone_xyzrpy[0] * eigen_palletrone_xyzrpy[0]);

	eigen_palletrone_xyzrpy[0] /= palletrone_mag;
	eigen_palletrone_xyzrpy[1] /= palletrone_mag;

	if(!(current_position_r < threshold_r))
	{
		// force 넣어주는 부분
		if(current_position_x!=0 || current_position_y!=-88)
		{
			omni_msgs::OmniFeedback feedback;

			threshold_force_x = threshold_force * current_position_x / current_position_r;
			threshold_force_y = -threshold_force * current_position_y / current_position_r;

			feedback.force.x = threshold_force_y;
			feedback.force.y = 0;
			feedback.force.z = threshold_force_x;

			palletrone_xyzrpy.linear.x = eigen_palletrone_xyzrpy[0];
			palletrone_xyzrpy.linear.y = eigen_palletrone_xyzrpy[1];
			palletrone_xyzrpy.linear.z = eigen_palletrone_xyzrpy[2];

			force_feedback.publish(feedback);
		}
		else
		{
			//ROS_INFO("Remove from the stand");
		}
	}
	else
	{
		// force 안 들어가도록
		omni_msgs::OmniFeedback feedback;
		geometry_msgs::Vector3 torque;
		geometry_msgs::Vector3 pos;
		torque.x = 0;
		torque.y = 0;
		torque.z = 0;
		pos.x = 0;
		pos.y = 0;
		pos.z = 0;
		feedback.force = torque;
		feedback.position = pos;
		force_feedback.publish(feedback);

		// force_feedback.publish(ds_feedback); // ds_feedback 힘 넣어줄 거면 이거 주석 풀기!

		dasom_xyzrpy = xyzrpy;

		palletrone_xyzrpy.linear.x = 0;
		palletrone_xyzrpy.linear.y = 0;
		palletrone_xyzrpy.linear.z = 0;
	}

	haptic_dasom_command_pub_.publish(dasom_xyzrpy);
	haptic_palletrone_command_pub_.publish(palletrone_xyzrpy);
}

void dsForceCallback(const geometry_msgs::WrenchStamped &msg)
{
	geometry_msgs::Vector3 torque;

	torque.x = msg.wrench.torque.x;
	torque.y = msg.wrench.torque.y;
	torque.z = msg.wrench.torque.z;

	// 만약에 매핑해 줄 거면 여기서!

	ds_feedback.force = torque;
}

void mySigintHandler(int sig)
{
	omni_msgs::OmniFeedback feedback;
	geometry_msgs::Vector3 torque;
	geometry_msgs::Vector3 pos;
	torque.x = 0;
	torque.y = 0;
	torque.z = 0;
	pos.x = 0;
	pos.y = 0;
	pos.z = 0;
	feedback.force = torque;
	feedback.position = pos;
	force_feedback.publish(feedback);
	ros::shutdown();
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "cylinder");
	ros::NodeHandle n;
	ROS_WARN("DASOM-Palletrone boundary node start!");
	force_feedback = n.advertise<omni_msgs::OmniFeedback>("/phantom/force_feedback", 1);
	haptic_dasom_command_pub_ = n.advertise<geometry_msgs::Twist>("/phantom/xyzrpy/dasom", 1);
	haptic_palletrone_command_pub_ = n.advertise<geometry_msgs::Twist>("/phantom/xyzrpy/palletrone", 1);
	ros::Subscriber sub = n.subscribe("/phantom/state", 1000, stateCallback);
	ros::Subscriber ds_force_sub = n.subscribe("/dasom/external_force", 1000, dsForceCallback);
	signal(SIGINT, mySigintHandler);
	ros::spin();
	return 0;
}
