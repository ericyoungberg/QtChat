#ifndef SOCKETHANDLER_H
#define SOCKETHANDLER_H


#include <vector>

class vector;

const char* PORT;

class SocketHandler
{
public:
  SocketHandler();

  int createConnection(char*);
  int closeConnection(char*);

private:
  vector<char*> _connections;

  struct addrinfo hints;
  struct addrinfo *servinfo;
  struct addrinfo *clientinfo;

};

#endif
