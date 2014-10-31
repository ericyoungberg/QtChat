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

NetworkHandler::NetworkHandler() {

}

void NetworkHandler::createListener() {

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
    cout << "Could not bind the socket\n\n\";
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

}
