#include <SocketHandler.h>
#include <vector>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

using namespace std;

const char* PORT = "2238";

SocketHandler::SocketHandler() {
}

int SocketHandler::createListener() {

  int status,     // Will tell us if the info was made properly
      sockfd;     // Socket file descriptor

  // Setup the hints to generate the information to connect
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  
  // Generate the server info structure
  if(status = getaddrinfo(NULL, PORT, &hints, &servinfo) != 0)
    return 1;
   
  // Create a socket to listen on 
  if(sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol) == -1)
    return 1;

  // Bind the socket to a port
  if(bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) < 0)
    return 1;

  // Listen on the bound port
  if(listen(sockfd, 10) < 0)
    return 1;

   
}


int SocketHandler::createConnection(char* IP) {

  int status,     // Will tell us if the info was made properly
      sockfd;     // Socket file descriptor 

  // Setup the hints to generate the information to connect
  memset(&hints, 0, sizeof hints);  // Empty the hints
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  
  // Generate the client info structure
  if(status = getaddrinfo(IP, PORT, &hints, &clientinfo) != 0)
    return 1; 

  // Get the socket descriptor based upon the client info generated above 
  if(sockfd = socket(clientinfo->ai_family, clientinfo->ai_socktype, clientinfo->ai_protocol) == -1)
    return 1;

  // Connect to the client
  status = 0; // reset the status flag
  if(status = connect(sockfd, clientinfo->ai_addr, clientinfo->ai_addrlen) == -1)
    return 1;

  // Free up the info for the next attempted connection
  freeaddrinfo(clientinfo);

  // Return 0 if the connection process succeeded
  return 0;
}


int SocketHandler::closeConnection(char* IP) {

} 
