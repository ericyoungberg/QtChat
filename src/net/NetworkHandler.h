#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H

#include <vector>
#include "ApplicationBus.h"

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
  NetworkHandler(ApplicationBus*);

  void transmit(char*, char*);
  int createOutwardConnection(char*);
  void createListener();

private:
  vector<connection> connections;

  ApplicationBus *ipc;

};

#endif
