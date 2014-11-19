#ifndef CHATINTERFACE_H
#define CHATINTERFACE_H

#include <QtDBus>
#include <QString>
#include "NetworkHandler.h"

class ChatInterface : public QDBusAbstractInterface
{
  Q_OBJECT

public:
  static inline const char *staticInterfaceName() {
    return "QtChat.ChatAdaptor";
  }

  ChatInterface(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);
  ~ChatInterface();

signals:
  void queueRouter(QString, QString);

};

#endif
