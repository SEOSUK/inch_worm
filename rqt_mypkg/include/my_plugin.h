/*
  Copyright 2018
*/
#ifndef RQT_MYPKG_CPP_MY_PLUGIN_H
#define RQT_MYPKG_CPP_MY_PLUGIN_H

#include <ros/ros.h>
#include <rqt_gui_cpp/plugin.h>
#include <QWidget>
#include <iostream>
#include "rqt_mypkg/ui_my_plugin.h"
#include <std_msgs/UInt16.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/JointState.h>
#include <eigen3/Eigen/Dense>
#include <vector>
#include "std_srvs/Empty.h"
#include "rqt_mypkg/DasomDynamixel.h"
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>


//#include "rqt_mypkg/FAC_HoverService.h"
//#include <QKeyEvent> 
namespace rqt_mypkg_cpp
{

class MyPlugin : public rqt_gui_cpp::Plugin
{    
    Q_OBJECT
public:
    MyPlugin();
    virtual void initPlugin(qt_gui_cpp::PluginContext& context);
    virtual void shutdownPlugin();

    template <class T>
T map(T x, T in_min, T in_max, T out_min, T out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


private slots:

   // void Arm_Callback(bool val);    
    void publisher_set(const ros::TimerEvent&);
    void callback_set(const ros::TimerEvent&);
//    void TextBox_callback(const ros::TimerEvent&);    
    void ping_callback(const ros::TimerEvent&);    
    void qsc_x_callback(int val);
    void qsc_y_callback(int val);
    void btn_Start_Callback(bool val);
    void AngleSubscriber_Callback(const sensor_msgs::JointState &msg);
    void measured_EE_position_Callback(const geometry_msgs::Twist &msg);

    private:
    Ui::MyPluginWidget ui_;
    QWidget* widget_;
    ros::Publisher publisher;         //이건 GUI Shutdown 용이라서 건들면 안 됨.
    ros::Publisher cmd_Publisher;
    ros::Subscriber AngleSubscriber;
    ros::Subscriber measured_EE_position_sub_;
//    ros::ServiceClient ping_client;
    ros::Timer Publisher_set;
    ros::Timer Callback_set;

    geometry_msgs::Twist EE_command;
    //    ros::ServiceServer HoverServer;
};

}  // namespace rqt_mypkg_cpp

#endif  // RQT_MYPKG_CPP_MY_PLUGIN_H
