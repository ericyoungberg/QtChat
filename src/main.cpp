/*
 * APPLICATION: QtChat
 * A UNIX chat application
 *
 * FILE: main.cpp
 * Where the magic begins
*/

#include <QtGui>
#include <QApplication>
#include <iostream>
#include <unistd.h>
#include "gui/mainwindow.h"
#include "net/NetworkHandler.h"
#include "net/ApplicationBus.h"

using std::cout;

// FUNCTION: main
// Event loops and the application domain
int main(int argc, char *argv[]) {

  // Define the application
  QApplication *app = new QApplication(argc, argv);

  ApplicationBus *bus = new ApplicationBus(app);

  // Fork into a seperate process
  // The first for the network listener
  // The second for the GUI
  if(!fork()) {
    // Create a listener to accept incoming connections for the GUI
    NetworkHandler *network = new NetworkHandler(bus);
    network->createListener();
  } else {
    // Define and launch the application window
    MainWindow *window = new MainWindow(bus);
    window->show();
  }

  // Start the event loop
  return app->exec();
}
// (END) main
