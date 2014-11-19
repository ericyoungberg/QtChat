#include "ChatAdaptor.h"
#include "NetworkHandler.h"

ChatAdaptor::ChatAdaptor(QObject *parent) : QDBusAbstractAdaptor(parent) {
  setAutoRelaySignals(true);
}

ChatAdaptor::~ChatAdaptor() {}
