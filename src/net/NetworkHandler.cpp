#include <iostream>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstdlib>
#include "NetworkHandler.h"

using namespace std;

const char* PORT = "2255";

NetworkHandler::NetworkHandler() {}

void NetworkHandler::transmit(char* IP, char* message) {
  int nbytes;

  cout << IP << ", " << message << endl;

  for(unsigned i=0;i<connections.size();i++) {
    if(strcmp(connections.at(i).IP, IP) == 0) {
      cout << "Connection found on: " << IP << endl;
      nbytes = send(connections.at(i).sockfd, message, strlen(message), 0);
      cout << "BYTES SENT: " << nbytes << endl; 
    }
  }
}

void NetworkHandler::createOutwardConnection(char* IP) {

  // Create connection structure and copy the IP into it before getaddrinfo screws it all up
  struct connection newConnection(IP);

  // Make a connection to send the message to the recipient
  struct addrinfo hints;
  struct addrinfo *servinfo;

  int sockfd;   // The socket created to be stored in the connection struct

  // Setup the hints
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  // Make the info
  getaddrinfo(IP, NULL, &hints, &servinfo);

  // Retrieve a socket for the connection
  sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
  if(sockfd == -1) cout << "Something went wrong with the socket\n";

  // Connect the socket
  connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);

  // Save the socket now that it has been setup
  newConnection.sockfd = sockfd;

  // Add the new connection to the list of connections for later use
  connections.push_back(newConnection);

}

void NetworkHandler::createListener() {

  /*
  fd_set master,
         read_fds;
  int fdmax;

  char buf[256];
  int nbytes;

  int yes = 1;
  int f, j;
  int listener,
      newfd,
      status;

  struct addrinfo hints;
  struct addrinfo *servinfo;

  socklen_t addr_size;

  struct sockaddr_storage remote_addr;

  FD_ZERO(&master);
  FD_ZERO(&read_fds);

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if((status = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
    cout << "Could not create addr\n\n";
    exit(1);
  }

  if((listener = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
    cout << "Could not create the socket\n\n";
    exit(1);
  }

  if(setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    cout << "Could not clear the port for reuse\n\n";
    exit(1);
  }

  if(bind(listener, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
    cout << "Could not bind the socket\n\n";
    exit(1);
  }

  freeaddrinfo(servinfo);

  if(listen(listener, 10) == -1) {
    cout << "Can't listen on the socket\n\n";
    exit(1);
  }

  FD_SET(listener, &master);

  fdmax = listener;

  while(true) {
    read_fds = master; 
    if(select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
      cout << "Couldn't start select\n\n";
      exit(1);
    }

    for(i=0;i<=fdmax;i++) {
      if(FD_ISSET(i, &read_fds)) {
        if(i == listener) {
          addrlen = sizeof(remote_addr);
          newfd = accept(listener, (struct sockaddr*)&remote_addr, &addrlen);
          if(newfd == -1) {
            cout << "Could not accept this connection\n\n";
          } else {
            FD_SET(newfd, &master);
            if(newfd > fdmax) fdmax = newfd;

          }
        } else {
          if((nbytes = recv(i, &buf, sizeof(buf), 0)) <= 0) {
            if(nbytes == 0) {
              cout << "Connection closed on " << i << endl; 
            }
            close(i);
            FD_CLR(i, &master);
          } else {
            for(j=0;j<=fdmax;j++) {
              
            }  
          } 
        }
      } 
    }

  }
*/
}
