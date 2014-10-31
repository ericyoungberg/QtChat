#include "NetworkHandler.h"
#include <vector>
#include <iostream>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include <QtDBus>

using namespace std;

NetworkHandler::NetworkHandler() {
  
}

void NetworkHandler::createConnection(char* IP) {

  // Make a connection to send the message to the recipient
  struct addrinfo hints;
  struct addrinfo *servinfo;

  int sockfd; 

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  getaddrinfo(IP, NULL, &hints, &servinfo);

  sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
  if(sockfd == -1) cout << "Something went wrong with the socket\n";

  connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);

  struct connection newConnection;
  strcpy(newConnection.IP, IP);
  newConnection.sockfd = sockfd;

  connections.push_back(newConnection);
}
