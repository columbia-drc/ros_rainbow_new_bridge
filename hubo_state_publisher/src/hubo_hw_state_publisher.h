/**
 * \file hubo_hw_state_publisher.cpp
 * \brief ROS node that publishes topics for Hubo's joint and IMU states
 * Adapted for rainbow code joint streamer
 * \date May 20, 2013
 * \author Andrew Price
 * \author Jon Weisz
 */


#include <ros/ros.h>
#include <string>


#include <urdf/model.h>

#include <tinyxml.h>
#include <boost/foreach.hpp>
#include <boost/range/adaptor/map.hpp>
#include "RainbowDrcHuboJoints.h"

typedef boost::shared_ptr<urdf::Joint> UrdfJointPtr;



/**
 * \class HuboHWStatePublisher
 * \brief Retrieves data from joint states struct and publishes tfs respecting mimic joint constraints
 */
class HuboHWStatePublisher
{
 public:
  HuboHWStatePublisher();
  bool parseModel();
  bool getMimicJoints(std::vector<UrdfJointPtr> &joint_list);
  bool publishState();
  bool parseDataCallback(char * buff);

protected:
  ros::NodeHandle m_nh;
  
  
  
  
  
  bool m_UseMimic;
  std::string m_RobotDescription;
  urdf::Model m_UrdfModelDescription;
  std::vector<UrdfJointPtr> m_MimicJoints;
  ros::Publisher m_JointPublisher;
  float joint_values[NO_OF_JOINT];
  std::string encoder, encoder_1;

  int getJointIndexFromName(const char * name);

};
