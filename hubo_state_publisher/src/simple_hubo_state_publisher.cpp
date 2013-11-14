//ros-based header files
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <tf/transform_broadcaster.h>

#include "RainbowSocketConnection.h"
#include "hubo_hw_state_publisher.h"

//THE DEFAULT PORT TO RECEIVE THE ENCODER VALUES
#define SERVER_PORT     8088



using namespace std;



char buffer[850];





int main(int argc, char** argv) {

//define your ros node,publisher,subscriber etc.
ros::init(argc, argv, "hubo_state_publisher");

ros::Rate loop_rate(100);

 RainbowSocketConnection con(SERVER_PORT);
 HuboHWStatePublisher pub;

 while (ros::ok()) {
   con.getData(buffer);
   pub.parseDataCallback(buffer);
   pub.publishState();
   ros::spinOnce();
   loop_rate.sleep();
}


    return 0;
}
