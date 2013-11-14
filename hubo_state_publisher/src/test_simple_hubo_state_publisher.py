#!/usr/bin/env python
import socket
import roslib
import rospy
import numpy
import IPython
 
UDP_IP = "127.0.0.1"
UDP_PORT = 8088

rainbow_defines_h='RainbowDrcHuboJoints.h'

rainbow_defines = {}


with open(rainbow_defines_h,'r') as f:
    for l in f:
        datalist=l.split('\t').split(' ')
        if datalist[0] == '#define' and len(datalist)>4:
            print datalist
            try:
                rainbow_defines[datalist[2]]=int(datalist[4])
            except:
                pass




class SocketTester:
    def __init__(self):
        self.sock = socket.socket(socket.AF_INET, # Internet
                             socket.SOCK_DGRAM) # UDP
        self.joint_values = numpy.zeros(rainbow_defines['NO_OF_JOINT'])
        

    def send_joint_values(self):
        joint_value_string = '*'.join([str(j) for j in self.joint_values])
        sock.sendto(joint_value_string, (UDP_IP, UDP_PORT))
    


def main():
    rospy.init_node("test_simple_hubo_state_planner")
    st = SocketTester()
    ipshell = IPShellEmbed(banner = 'Dropping into IPython',
                           exit_msg = 'Leaving Interpreter, back to program.')
    
    ipshell(local_ns = locals())
    loop = rospy.Rate(10)
    
    while not rospy.is_shutdown():
        loop.sleep()
        st.send_joint_values()

if __name__ == "__main__":
    main()

