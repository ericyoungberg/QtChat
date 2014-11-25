#include <iostream>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include "NetworkHandler.h"
#include "ChatAdaptor.h"
#include "ChatInterface.h"

using namespace std;

// Universal application port
const char* PORT = "2255";

// Blank constructor
NetworkHandler::NetworkHandler() {

  // Add the Network Handler to the the D-Bus
  new ChatAdaptor(this); 
  QDBusConnection::sessionBus().registerObject("/", this);

  // Connect the interface to the Network Handler
  ipc = new ChatInterface(QString(), QString(),  QDBusConnection::sessionBus(), this);

}


//----------------------------------------------------------------------
// METHOD: transmit
// Finds an existing socket and sends a message across it
//----------------------------------------------------------------------
void NetworkHandler::transmit(char* IP, char* message) {


  cout << "OUT: " << message << ", IP: " << IP << endl;

  // Number of bytes sent
  int nbytes;

  // Find a connection that matches the IP destination passed to the method
  for(unsigned i=0;i<connections.size();i++) {
    if(strcmp(connections.at(i).IP, IP) == 0) {

      // Try to send the message over the socket
      if((nbytes = send(connections.at(i).sockfd, message, strlen(message), 0)) == -1) {
        cout << "Could not send the message to " << IP << endl;
        return; // Leave if we couldn't send anything
      }

      // Exit this function calmly
      return;
    }
  }

  // If we didn't find a connection with this user, let's try building one
  // then re-transmit the message
  if(createOutwardConnection(IP) == 0) {
    cout << "TRANSMIT RETRIED... AND SUCCEEDED! SHREKT M8!\n";
    transmit(IP, message);
  }

}
// (END) transmit


//----------------------------------------------------------------------
// METHOD: createOutwardConnection
// Creates a new socket to send messages over and stores it in the
// connections vector.
//----------------------------------------------------------------------
int NetworkHandler::createOutwardConnection(char* IP) {

  // Create connection structure and copy the IP into it before getaddrinfo screws it all up
  struct connection newConnection(IP);

  // Make a connection to send the message to the recipient
  struct addrinfo hints;
  struct addrinfo *servinfo;

  int sockfd;   // The socket created to be stored in the connection struct

  // Setup the hints
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  // Make the info
  int error = getaddrinfo(IP, PORT, &hints, &servinfo);
  if(error) cout << "Could not get the addrinfo\n";

  // Retrieve a socket for the connection
  sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
  if(sockfd == -1) cout << "Something went wrong with the socket\n";

  // Connect the socket
  if(::connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
    cout << "Connection is futile!\n"; 
    return -1;
  }

  // Save the socket now that it has been setup
  newConnection.sockfd = sockfd;

  // Add the new connection to the list of connections for later use
  connections.push_back(newConnection);

  // Everything is good
  return 0;
}
// (END) makeOutwardConnection


//----------------------------------------------------------------------
// METHOD: createListener
// Creates a listener that accepts incoming connections and their 
// respsective transmissions
//----------------------------------------------------------------------
void NetworkHandler::createListener() {

  fd_set master,
         read_fds;
  int fdmax;

  char buf[1024];
  int nbytes;

  char rawIP[24];

  int yes = 1;
  int i;
  int listener,
      newfd,
      status;

  struct addrinfo hints;
  struct addrinfo *servinfo;

  socklen_t addrlen;

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
          if((nbytes = recv(i, buf, sizeof(buf), 0)) <= 0) {
            close(i);
            FD_CLR(i, &master);
          } else {

            // Grab only the current size of the message from the buffer
            buf[nbytes] = '\0';

            cout << "IN: " << buf << endl;

            // Get the sender's IP
            status = getpeername(i, (struct sockaddr*)&remote_addr, &addrlen);
            if(status == -1) cout << "Could not retrieve peername\n";
            struct sockaddr_in *s = (struct sockaddr_in*)&remote_addr;
            inet_ntop(AF_INET, &s->sin_addr, rawIP, sizeof(rawIP));

            // Convert to QString
            QString newIP(rawIP);
            QString newMessage(buf);

            emit queueRouter(newIP, newMessage);
          } 
        }
      } 
    }

  }
}
// (END) createListener
