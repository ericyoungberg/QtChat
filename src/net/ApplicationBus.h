#ifndef APPLICATIONBUS_H
#define APPLICATIONBUS_H

#include <QDBusAbstractAdaptor>
#include <QApplication>

class ApplicationBus : public QDBusAbstractAdaptor
{
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "QtChat.ApplicationBus")

private:
  QApplication *app;

public:
  ApplicationBus(QApplication *app);

  void newMessage(char*, char*);

signals:
  Q_NOREPLY void receivedMessage(char*, char*);

};

#endif
