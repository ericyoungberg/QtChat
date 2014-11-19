#include "ChatInterface.h"

ChatInterface::ChatInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent) :
  QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent) {}

ChatInterface::~ChatInterface(){}
