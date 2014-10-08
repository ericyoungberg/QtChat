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

  fd_set master,    // The main set of socket descriptors
         read_fds;  // The copy of main set that will be used for actual work
  int    fdmax;     // The largest socket descriptor

  // Make sure the sets have been cleared
  FD_ZERO(&master);
  FD_ZERO(&read_fds);


  int status,     // Will tell us if the info was made properly
      sockfd,     // Socket file descriptor for listener
      newfd;      // Socket file descriptor for new client  

  struct sockaddr_storage remoteaddr;   // Client address
  socklen_t addrlen;



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

  // No longer need to server info since we have a working descriptor
  freeaddrinfo(servinfo);

  // Listen on the bound port
  if(listen(sockfd, 10) < 0)
    return 1;

  // Add the listening socket to the master set of sockets
  FD_SET(sockfd, &master);

  // Keep track to the largest descriptor
  fdmax = sockfd;


  int pid;  // Process ID for the child process to be forked

  // Fork the listener to a different process so the application can continue to run
  if((pid =fork()) == 0) {

    // Main listener loop
    for(;;) {

      // Copy the master set of file descriptors over to a working copy
      read_fds = master; 

      for(int i=0;i<fdmax; i++) {
        if(FD_ISSET(i, &read_fs)) {

          // If it's the listener, check for new clients
          // If not, then it must be a client
          if(i == listener) {
            addrlen = sizeof remoteaddr; 
            newfd = accept(listener, (struct sockaddr*)&remoteaddr, &addrlen);

            if(newfd == -1) {
              return 1;
            } else {
              FD_SET(newfd, &master);   // add to master set 
              if (newfd > fdmax) fdmax = newfd;   // Keep track of the max fd

              cout << "New connection!\n";
            }
          } else {

            // Get data from the client 
            for(int f=0;f<=fdmax;f++) {
              if(FD_ISSET(j, &master)) {
                if (j != listener && j != i) {
                  if(send(j, buf, nbytes, 0) == -1) {
                    return 1; 
                  } 
                } 
              }
            }
          } // (END) listener/client router 
        }
      } // (END) socket set iterator


    } // (END) Main loop 
  } // (END) fork
   
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
