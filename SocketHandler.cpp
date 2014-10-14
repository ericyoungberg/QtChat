#include <SocketHandler.h>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

using namespace std;

const char* PORT = "2238";

SocketHandler::SocketHandler() {
  
}

int SocketHandler::createListener() {
  if(pid = fork()) {
    cout << "PID: " << pid << endl;
  }

  return 0; 
}


int SocketHandler::createConnection(char* IP) {
  return 0;
}


int SocketHandler::closeConnection(char* IP) {
  return 0;
} 
