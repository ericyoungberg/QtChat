#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H

#include <vector>
#include <QObject>
#include <QString>
#include "ChatInterface.h"

class ChatInterface;
class QString;

using std::vector;


// -------------------------------------------------------------
// STRUCT: connection
// Stores reference information for a socket 
// -------------------------------------------------------------
struct connection {
  int sockfd;
  char IP[24];

  // Constructors
  connection(){};
  connection(char* setIP) {
    strcpy(IP, setIP); 
  }
};
// (END) connection


// -------------------------------------------------------------
// CLASS: NetworkHandler
// Handles all network based calls
// -------------------------------------------------------------
class NetworkHandler : public QObject
{
  Q_OBJECT

public:
  NetworkHandler();
  virtual ~NetworkHandler() {};

  void transmit(char*, char*);
  void createListener();

  int createOutwardConnection(char*);
  void deleteOutwardConnection(char*);

signals:
  void queueRouter(QString, QString);

private:
  vector<connection> connections;

  ChatInterface *ipc;
};
// (END) NetworkHandler

#endif
