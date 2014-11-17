#include "ApplicationBus.h"

// Constructor
ApplicationBus::ApplicationBus(QApplication *application) : QDBusAbstractAdaptor(application), app(application) {
  connect(application, SIGNAL(receivedMessage(char*, char*)), SIGNAL(receivedMessage(char*, char*)));
}


void ApplicationBus::newMessage(char* IP, char* message) {
  emit receivedMessage(IP, message);
}
