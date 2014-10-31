#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H

#include <vector>
using std::vector;

struct connection {
  int sockfd;
  char* IP;
};

class NetworkHandler
{
public:
  NetworkHandler();

  void createConnection(char*);
  
  vector<connection> connections; 

};

#endif
