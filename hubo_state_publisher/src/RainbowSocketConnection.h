//Network based header files. 
//Please do not change this order of loading the header  files.
#include <stdio.h>      
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h> 
#include<iostream>
#include<string>
#include <unistd.h>
#include<iomanip>
#include <sys/ioctl.h>
#include <sstream>


/**

 **/
class RainbowSocketConnection
{
 public:
  RainbowSocketConnection(unsigned int server_port)
    {
      bool nonblocking = true;
      serveraddr.sin_family      = AF_INET;
      serveraddr.sin_port        = htons(server_port);
      serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); //make sure to provide this rather than a particular ip-address(which never works).
      //define your socket
      sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
      ioctl(sd,FIONBIO,&nonblocking);
      int size=sizeof(serveraddr);   
      int rc = bind(sd, (struct sockaddr *)&serveraddr, size);
      serveraddrlen =sizeof(serveraddr);

    }
  bool getData(char * buffer)
    {
      //recieve the UDP packets from the hubo computer
      int rc = recvfrom(sd, buffer, sizeof(buffer), 0,
                    (struct sockaddr *)&serveraddr, 
                    &serveraddrlen);
      return true;
    }
  
 protected:
  //define your socket related objects
  struct sockaddr_in serveraddr;
  socklen_t serveraddrlen;
  int sd;
};
