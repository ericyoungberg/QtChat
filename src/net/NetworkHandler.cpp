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


//----------------------------------------------------------------------
// METHOD: NetworkHandler
// Attaches an D-Bus adaptor to this QObject so we can communicate
// with the GUI process
//----------------------------------------------------------------------
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

  // Number of bytes sent
  int nbytes;

  // Find a connection that matches the IP destination passed to the method
  for(unsigned i=0;i<connections.size();i++) {
    if(strcmp(connections.at(i).IP, IP) == 0) {

      cout << "OUT: " << message << ", " << IP << endl;

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
    cout << "Recreated connection ... \n";
    transmit(IP, message);
  }

}
// (END) transmit


void NetworkHandler::deleteOutwardConnection(char* IP) {

  // Find a connection that matches the IP destination passed to the method
  for(unsigned i=0;i<connections.size();i++) {
    if(strcmp(connections.at(i).IP, IP) == 0) {
      connections.erase(i); // Remove the connection from the list of current connections
    }
  }
}
// (END) deleteOutwardConnection


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
    cout << "Connection unsuccessful\n"; 
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

  fd_set master,    // Master socket set
         read_fds;  // Copy of the master set
  int fdmax;        // Max socket descriptor within the set

  char buf[1024];   // Buffer
  int nbytes;       // Number of bytes received

  char rawIP[24];   // The IP address before it has been wrapped in Qt Classes

  int yes = 1;      // Used as reference to an integer
  int i;            // Iterator count
  int listener,     // Listener descriptor
      newfd,        // New file descriptor that is accepted
      status;       // The error status for setting up the socket/listener

  struct addrinfo hints;      // Struct for storing address hints
  struct addrinfo *servinfo;  // The server/listener info 

  socklen_t addrlen;          // The base length of an address

  struct sockaddr_storage remote_addr;  // Storage placeholder for the client address info

  // Reset the Socket FD set
  FD_ZERO(&master);
  FD_ZERO(&read_fds);

  // Setup the hints for creating the address info structure
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  // Make the address info
  if((status = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
    cout << "Could not create addr\n\n";
    exit(1);
  }

  // Create the listener socket
  if((listener = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {
    cout << "Could not create the socket\n\n";
    exit(1);
  }

  // Make sure that we can bind this socket on restart just incase it didn't flush on exit
  if(setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    cout << "Could not clear the port for reuse\n\n";
    exit(1);
  }

  // Binds the listener socket to the port
  if(bind(listener, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
    cout << "Could not bind the socket\n\n";
    exit(1);
  }

  // Clear the server info struct since we no longer need it
  freeaddrinfo(servinfo);

  // Listen for new connections
  if(listen(listener, 10) == -1) {
    cout << "Can't listen on the socket\n\n";
    exit(1);
  }

  // Pour the accepted connections into the master set
  FD_SET(listener, &master);

  // Set the initial socket max to the listener
  fdmax = listener;

  // Time to listen
  while(true) {

    read_fds = master;  // Copy over the the master set each time so you don't overwrite the currently open connections

    // Look/select sockets to check for state changes
    if(select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
      cout << "Couldn't start select\n\n";
      exit(1);
    }

    // Look through the sockets
    for(i=0;i<=fdmax;i++) {

      // If there is a socket here...
      if(FD_ISSET(i, &read_fds)) {
        
        // If the socket found is the listener...
        if(i == listener) {

          // Accept connection
          addrlen = sizeof(remote_addr);
          newfd = accept(listener, (struct sockaddr*)&remote_addr, &addrlen);

          // Check for error
          if(newfd == -1) {
            cout << "Could not accept this connection\n\n";
          } else {

            // If there was a new connection, add it to the master set for now
            FD_SET(newfd, &master);
            if(newfd > fdmax) fdmax = newfd; // Change the max if necessary
          }
        } else {  // If it was not the listener...

          // Try to receive data on the socket
          if((nbytes = recv(i, buf, sizeof(buf), 0)) <= 0) {
            
            cout << "DELETING " << i << endl;

            // Close the socket if we have lose the connection 
            FD_CLR(i, &master);
            close(i);

          } else {  // If we did receive something from the connection

            cout << "SOCK: " << i << ", MAX: " << fdmax << endl;

            // Grab only the current size of the message from the buffer
            buf[nbytes] = '\0';

            // Get the sender's IP
            status = getpeername(i, (struct sockaddr*)&remote_addr, &addrlen);
            if(status == -1) cout << "Could not retrieve peername\n";
            struct sockaddr_in *s = (struct sockaddr_in*)&remote_addr;
            inet_ntop(AF_INET, &s->sin_addr, rawIP, sizeof(rawIP));

            // Convert to QString
            QString newIP(rawIP);
            QString newMessage(buf);

            emit queueRouter(newIP, newMessage);  // Emit the signal across the IPC with the newly received information
          } 
        }
      } 
    }

  }
}
// (END) createListener
