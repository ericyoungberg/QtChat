#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H

#include <vector>
#include <QObject>
#include <QString>
#include "ChatInterface.h"

class ChatInterface;
class QString;

using std::vector;

struct connection {
  int sockfd;
  char IP[24];

  connection(){};
  connection(char* setIP) {
    strcpy(IP, setIP); 
  }
};

class NetworkHandler : public QObject
{
  Q_OBJECT

public:
  NetworkHandler();
  virtual ~NetworkHandler() {};

  void transmit(char*, char*);
  void createListener();

  int createOutwardConnection(char*);

signals:
  void queueRouter(QString, QString);

private:
  vector<connection> connections;

  ChatInterface *ipc;
};

#endif
