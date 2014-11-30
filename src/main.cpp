/*
 * APPLICATION: QtChat
 * A P2P UNIX chat application built with Qt
 *
 * FILE: main.cpp
 * Where the magic begins
*/

#include <QtGui>
#include <QApplication>
#include <QtDBus>
#include <iostream>
#include <unistd.h>
#include <sys/prctl.h>
#include <signal.h>

#include "gui/MainWindow.h"
#include "net/NetworkHandler.h"

using std::cout;

// FUNCTION: main
// Event loops and the application domain
int main(int argc, char *argv[]) {

  // Define the application
  QApplication *app = new QApplication(argc, argv);

  // Create the DBus session
  if(!QDBusConnection::sessionBus().isConnected()) {
    cout << "CANNOT CONNECT\n";
    return 1;
  }

  // Fork into a seperate process
  // The first for the network listener
  // The second for the GUI
  if(!fork()) {

    prctl(PR_SET_PDEATHSIG, SIGHUP);  // Make sure that the child process gets killed when the parent does

    // Create a listener to accept incoming connections for the GUI
    NetworkHandler *network = new NetworkHandler();
    network->createListener();
  } else {
    // Define and launch the application window
    MainWindow *window = new MainWindow();
    window->show();
  }

  // Start the event loop
  return app->exec();
}
// (END) main
