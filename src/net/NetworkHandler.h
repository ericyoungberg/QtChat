#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H

#include <vector>

using std::vector;

struct connection {
  int sockfd;
  char IP[24];

  connection(){};
  connection(char* setIP) {
    strcpy(IP, setIP); 
  }
};

class NetworkHandler
{

public:
  NetworkHandler();

  void transmit(char*, char*);
  int createOutwardConnection(char*);
  void createListener();

private:
  vector<connection> connections;

};

#endif
