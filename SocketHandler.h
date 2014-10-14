#ifndef SOCKETHANDLER_H
#define SOCKETHANDLER_H


class SocketHandler
{
public:
  SocketHandler();

  int createListener();
  int createConnection(char*);
  int closeConnection(char*);

private:
//  struct addrinfo hints;
//  struct addrinfo *servinfo;
//  struct addrinfo *clientinfo;

};

#endif
