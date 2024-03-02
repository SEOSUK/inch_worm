#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include "std_msgs/Int16.h"
#include "omni_msgs/OmniState.h"
#include "omni_msgs/OmniFeedback.h"
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Vector3.h>
#include <signal.h>

ros::Publisher force_feedback;

float torquex=0;
float torquey=0;
float torquez=0;

float force_function(float currentPos, float limit)
{
	float force = (currentPos - limit) * 0.2;
	if(force > 2.0)
	{
		force = 2.0;
	}
	else if(force < -2.0)
	{
		force = -2.0;
	}
	return force;
}

float xmax = 110;
float xmin = -110;
float ymax = 110;
float ymin = -110;
float zmax = 35;
float zmin = -35;

void stateCallback(const omni_msgs::OmniState::ConstPtr& msg)
{
	float x = (msg->pose.position.x);
	int y =(int) msg->pose.position.y;
	int z =(int) msg->pose.position.z;

	if(!(x<xmax && x>xmin && y>ymin && y<ymax && z>zmin && z<zmax))
	{
		if(x!=0 || y!=88 || z!=-65)
		{
			omni_msgs::OmniFeedback feedback;
			geometry_msgs::Vector3 torque;
			geometry_msgs::Vector3 pos;
			pos.x = x;
			pos.y = y;
			pos.z = z;
			float xval = 0;
			float yval = 0;
			float zval = 0;
			if(x>=xmax)
			{
				xval = force_function(x, xmax);
				// ROS_WARN("X force = %lf", xval);
			}
			else if(x<=xmin)
			{
				xval = force_function(x, xmin);
				// ROS_WARN("X force = %lf", xval);
			}
			if(y>ymax)
			{
				yval = force_function(y, ymax);
				// ROS_WARN("Y force = %lf", yval);
			}
			else if(y<=ymin)
			{
				yval = force_function(y, ymin);
				// ROS_WARN("Y force = %lf", yval);
			}
			if(z>zmax)
			{
				zval = force_function(z, zmax);
				// ROS_WARN("Z force = %lf", zval);
			}
			else if(z<=zmin)
			{
				zval = force_function(z, zmin);
				// ROS_WARN("Z force = %lf", zval);
			}

			// For our haptic coordinate system
			torque.x = yval;
			torque.y = zval;
			torque.z = xval;
			feedback.force = torque;
			feedback.position = pos;
			force_feedback.publish(feedback);
		}
		else
		{
			//ROS_INFO("Remove from the stand");
		}
		
	}
	else
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
	}
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
	ros::init(argc, argv, "box");
	ros::NodeHandle n;
	force_feedback = n.advertise<omni_msgs::OmniFeedback>("/phantom/force_feedback", 1);
	ros::Subscriber sub = n.subscribe("/phantom/state", 1000, stateCallback);
	signal(SIGINT, mySigintHandler);
	ros::spin();
	return 0;
}
