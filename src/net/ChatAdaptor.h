#ifndef CHATADAPTOR_H
#define CHATADAPTOR_H

#include <QtDBus>
#include "NetworkHandler.h"

class ChatAdaptor : public QDBusAbstractAdaptor
{
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "QtChat.ChatAdaptor");

public:
  ChatAdaptor(QObject *parent);
  ~ChatAdaptor();

signals:
  void queueRouter(QString, QString);

};

#endif
