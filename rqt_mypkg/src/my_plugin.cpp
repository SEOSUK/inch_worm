/*
  Copyright 2018
*/
#include "my_plugin.h"
#include <pluginlib/class_list_macros.h>
#include <QStringList>

double PI = 3.141592;

bool isCallback = false;
float value_x = 0;
float value_y = 0;
float value_z = 0;

Eigen::Vector2d angle_meas;
Eigen::Vector2d End_Effector_Position_cmd;
Eigen::Vector2d End_Effector_Position_meas;


namespace rqt_mypkg_cpp
{



MyPlugin::MyPlugin()
    : rqt_gui_cpp::Plugin()
    , widget_(0)
{
    setObjectName("C++PluginT");
}

void MyPlugin::initPlugin(qt_gui_cpp::PluginContext& context)
{
    // access standalone command line arguments
    QStringList argv = context.argv();
    // create QWidget
    widget_ = new QWidget();
    // extend the widget with all attributes and children from UI file
    ui_.setupUi(widget_);
    // add widget to the user interface
    context.addWidget(widget_);
////////////////////////////////////////////////////////////////////////////////////////
  ros::start();
  ros::NodeHandle n;


  QObject::connect(ui_.qsc_x, SIGNAL(valueChanged(int)), this, SLOT(qsc_x_callback(int)));
  QObject::connect(ui_.qsc_y, SIGNAL(valueChanged(int)), this, SLOT(qsc_y_callback(int)));
  QObject::connect(ui_.btn_Start, SIGNAL(toggled(bool)), this, SLOT(btn_Start_Callback(bool))); 

  Publisher_set = n.createTimer(ros::Duration(0.01), &MyPlugin::publisher_set, this);
  Callback_set = n.createTimer(ros::Duration(0.1), &MyPlugin::callback_set, this);

  cmd_Publisher = n.advertise<geometry_msgs::Twist>("/inch/EE_cmd_gui", 100);
  AngleSubscriber = n.subscribe("/joint_states", 100, &MyPlugin::AngleSubscriber_Callback, this);
  measured_EE_position_sub_ = n.subscribe("/inch/EE_meas", 100, &MyPlugin::measured_EE_position_Callback, this);
}




void MyPlugin::shutdownPlugin()
{

    publisher.shutdown();
    cmd_Publisher.shutdown();
    AngleSubscriber.shutdown();
    ros::shutdown();
}



void MyPlugin::callback_set(const ros::TimerEvent&) 
{
  if(isCallback)
  {
      ui_.txt_joint1->setText((QString::number(angle_meas[0])));
      ui_.txt_joint2->setText((QString::number(angle_meas[1])));
      
      ui_.txt_FK_x->setText((QString::number(End_Effector_Position_meas[0])));
      ui_.txt_FK_y->setText((QString::number(End_Effector_Position_meas[1])));


    ui_.txt_Error_x->setText((QString::number(value_x - End_Effector_Position_meas[0], 'f', 3)));
    ui_.txt_Error_y->setText((QString::number(value_y - End_Effector_Position_meas[1], 'f', 3)));  
  }   
}


void MyPlugin::publisher_set(const ros::TimerEvent&) 
{
    ui_.lbl_cmd_x->setText((QString::number(value_x, 'f', 4)));
    ui_.lbl_cmd_y->setText((QString::number(value_y, 'f', 4)));
    ui_.lbl_cmd_z->setText((QString::number(value_z, 'f', 4)));

  if(ui_.chk_Publish->isChecked() && isCallback) 
  {
    EE_command.linear.x = value_x;
    EE_command.linear.y = value_y;

    cmd_Publisher.publish(EE_command);
    ros::spinOnce();
  }

}


void MyPlugin::qsc_x_callback(int val)
{
	value_x = map<double>(val, 0, 10000000, 0.0,0.5);
}

void MyPlugin::qsc_y_callback(int val)
{
	value_y = map<double>(val, 0, 10000000, 0.1,0.5);
}


void MyPlugin::btn_Start_Callback(bool val)
{
  if(ui_.btn_Start->isChecked()) 
  {
  isCallback = true;
  ui_.btn_Start->setText("Starting..");
  }
  else 
  {
  isCallback = false;
  ui_.btn_Start->setText("Start!");

  ui_.txt_joint1->setText("0");
  ui_.txt_joint2->setText("0");
  ui_.txt_joint3->setText("0");

  ui_.txt_FK_x->setText("0");
  ui_.txt_FK_y->setText("0");
  ui_.txt_FK_z->setText("0");

  ui_.txt_Error_x->setText("0");
  ui_.txt_Error_y->setText("0");
  ui_.txt_Error_z->setText("0");

  }
  return ;
}

void MyPlugin::AngleSubscriber_Callback(const sensor_msgs::JointState &msg)
{
  angle_meas[0] = msg.position.at(0);
//  angle_meas[1] = msg.position.at(1);
}

void MyPlugin::measured_EE_position_Callback(const geometry_msgs::Twist &msg)
{
  End_Effector_Position_meas[0] = msg.linear.x;
//  End_Effector_Position_meas[1] = msg.linear.y;
}

}  // namespace rqt_mypkg_cpp

// #define PLUGINLIB_DECLARE_CLASS(pkg, class_name, class_type, base_class_type)
PLUGINLIB_EXPORT_CLASS(rqt_mypkg_cpp::MyPlugin, rqt_gui_cpp::Plugin)


