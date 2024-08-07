#ifndef INCH_MANIPULATOR_CONTROL_H_
#define INCH_MANIPULATOR_CONTROL_H_

#include <ros/ros.h>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>
#include <iostream>
#include <std_msgs/String.h>
#include <sensor_msgs/JointState.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseStamped.h>
#include <std_msgs/Float64MultiArray.h>
#include <cmath>

#include <tf/transform_datatypes.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_msgs/TFMessage.h>
#include <inch_toolbox/inch_workbench.h>
#include <inch_toolbox/inch_joint.h>
#include <inch_toolbox/inch_misc.h>
#include <std_srvs/Empty.h>
#define PI 3.141592

using namespace inch;

class InchControl : public inch::InchWorkbench
{
 public:
  InchControl();
  ~InchControl();

  /*****************************************************************************
  ** Define variables
  *****************************************************************************/
  // For time loop (count)
  double time_i = 0;
  double time_f = 0;
  double time_loop = 0;
  double time_real = 0;

  // From Launch File
    //Link Param
  double Link1_length;
  double Link2_length;

  double Link1_COM;
  double Link2_COM;

  double Link1_mass;
  double Link2_mass;

  double Link1_spring;
  double Link2_spring;

  double init_X;
  double init_Y;

  double Gravity;
  double N1;
  double N2;
  double N3;
  
  bool gimbal_Flag;
  bool initPoseFlag;


  std::string robot_name_;

  /*****************************************************************************
  ** Define functions
  *****************************************************************************/
  void PublishData();
  void deleteToolbox();
  void SolveInverseForwardKinematics();
  
  void TimeCount();
  void Test_trajectory_generator_2dof();
  void Trajectory_mode();
  void trajectory_gimbaling();
  void Experiment_0623_1Link();
  Eigen::Vector2d F_ext_processing();
  void init_pose_function();
  //ROS
  void inch_gimbal_EE_cmd_callback(const geometry_msgs::Twist& msg);
  bool gimbal_callback(std_srvs::Empty::Request& request, std_srvs::Empty::Response& response);

  //TeamWork!
  void YujinWhile();
  void HanryungWhile();
  void SeukWhile();

  void YujinInit();
  void HanryungInit();
  void SeukInit();


 private:
  /*****************************************************************************
  ** ROS NodeHandle
  *****************************************************************************/
  ros::NodeHandle node_handle_;
  ros::NodeHandle priv_node_handle_;

  /*****************************************************************************
  ** ROS Parameters
  *****************************************************************************/

  /*****************************************************************************
  ** Inchtoolbox
  *****************************************************************************/
  // Inch
  InchJoint *inch_joint;

  InchMisc *inch_q_meas_butterworth;
  InchMisc *inch_link1_PID;
  InchMisc *inch_link2_PID;

  /*****************************************************************************
  ** Init Functions
  *****************************************************************************/
  void initPublisher();
  void initSubscriber();
  void initServer();
  

  /*****************************************************************************
  ** ROS Publishers
  *****************************************************************************/
  ros::Publisher theta_command_pub_;
  ros::Publisher EE_meas_pub_;
  ros::Publisher EE_ref_pub_;
  ros::Publisher test_pub_;

  /*****************************************************************************
  ** ROS Subscribers
  *****************************************************************************/
  ros::Subscriber dynamixel_workbench_sub_;
  ros::Subscriber Optitrack_sub_;
  ros::Subscriber gimbal_EE_cmd_sub_;
  ros::ServiceServer inch_gimbal_Flag_server_;
  /*****************************************************************************
  ** ROS Services Clients
  *****************************************************************************/


  /*****************************************************************************
  ** Define variables
  *****************************************************************************/
  Eigen::Vector2d q_ref;
  Eigen::Vector2d q_cmd;
  Eigen::Vector2d q_des;
  Eigen::Vector2d theta_ref; // theta 레퍼런스
  Eigen::Vector2d theta_des; // theta 레퍼런스 (속도리미트 걸림)
  Eigen::Vector2d theta_cmd; // theta 커맨드 (제어기 통과)
  Eigen::Vector2d EE_meas;
  Eigen::Vector2d EE_cmd;
  Eigen::Vector2d EE_gimbal_cmd;
  Eigen::Vector2d EE_ref;
  Eigen::Vector2d F_ext;
  Eigen::Vector2d tau_ext;
  Eigen::Vector2d init_pose;
  Eigen::Vector2d init_theta;
  //Experiment_0623_1Link

  std_msgs::Float64MultiArray test_msg;
  geometry_msgs::Twist EE_meas_msg;
  geometry_msgs::Twist EE_ref_msg;
  sensor_msgs::JointState theta_command_msg;

};

#endif //INCH_MANIPULATOR_CONTROL_H_
