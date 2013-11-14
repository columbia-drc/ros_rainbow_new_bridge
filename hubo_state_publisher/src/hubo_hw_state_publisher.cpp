/**
 * \file hubo_hw_state_publisher.cpp
 * \brief ROS node that publishes topics for Hubo's joint and IMU states
 *
 * \date May 20, 2013
 * \author Andrew Price
 */

#include "hubo_hw_state_publisher.h"
#include <sensor_msgs/JointState.h>

HuboHWStatePublisher::HuboHWStatePublisher()
  
{		
  std::string robot_description_param;
  ros::param::param<std::string>( "robot_description_param", robot_description_param, "robot_description");
  ros::param::param<bool>( "use_mimic", m_UseMimic, true);
  ros::param::get( robot_description_param, m_RobotDescription );
  parseModel();
  m_JointPublisher = m_nh.advertise<sensor_msgs::JointState>("joint_states", 1);
}

bool 
HuboHWStatePublisher::parseModel()
{
  TiXmlDocument doc;
  doc.Parse( m_RobotDescription.c_str() );
  if( !doc.RootElement() )
    {
      
      ROS_WARN("Invalid xml in robot description");
      return false;                                
    }
  if( !m_UrdfModelDescription.initXml( doc.RootElement() ))
    {
      ROS_WARN("Robot model failed to parse");
    }
  getMimicJoints(m_MimicJoints);
}

bool 
HuboHWStatePublisher::getMimicJoints(std::vector<UrdfJointPtr> &joint_list)
{
  joint_list.clear();
  BOOST_FOREACH(UrdfJointPtr & joint, m_UrdfModelDescription.joints_ | boost::adaptors::map_values)
    {
      if(joint->mimic)
	joint_list.push_back(joint);
    }
  return joint_list.size();
}


int
HuboHWStatePublisher::getJointIndexFromName(const char * name)
{
  for(unsigned int i = 0; i < NO_OF_JOINT; ++i)
    {
      if(!strcmp(name, ConvertedRainbowJointNames[i]))
	return i;	
    }
  return -1;
}

bool 
HuboHWStatePublisher::publishState()
{

  sensor_msgs::JointState jState;
    
  jState.header.stamp= ros::Time::now();
  for(unsigned int joint_ind = 0; joint_ind < NO_OF_JOINT; ++joint_ind)
    {
      jState.name.push_back(ConvertedRainbowJointNames[joint_ind]);
      jState.position.push_back(joint_values[joint_ind]);
      jState.velocity.push_back(0);
    }
  
  BOOST_FOREACH(UrdfJointPtr joint, m_MimicJoints)
    {
      jState.name.push_back(joint->name);
      std::string mimic_name = joint->mimic->joint_name;
      int mimicJointIndex = getJointIndexFromName(mimic_name.c_str());
      if (mimicJointIndex < 0)
	continue;
      jState.position.push_back(joint_values[mimicJointIndex]);
      //For now the joint state is set as 0
      jState.velocity.push_back(0);      
    }
  m_JointPublisher.publish(jState);
}


bool 
HuboHWStatePublisher::parseDataCallback(char * buff)
{
  int init=1;
  int n_0=0;
  int n_1=0;
  //check if they are empty packets
  if(encoder.length()!=0)
    {
      //Parse the recieved encoder data.
      for(int i=0;i<NO_OF_JOINT;i++)
	{
	  n_1=encoder.find('*',n_0);
	  int diff=n_1-init;
	  encoder_1=encoder.substr(init,diff);
	  init=n_1+1;
	  joint_values[i]=std::atof(encoder_1.c_str());
	  n_0=n_1+1;
	  encoder_1=" ";
	  
	}
    }
  return true;
}
