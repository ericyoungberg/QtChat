/*
 * APPLICATION: QtChat
 * A UNIX chat application
 *
 * FILE: main.cpp
 * Where the magic begins
*/

#include <QtGui>
#include <QApplication>
#include <mainwindow.h>


// FUNCTION: main
// Event loops and the application domain
int main(int argc, char *argv[]) {

  // Define the application
  QApplication app(argc, argv);

  // Define and launch the application window
  MainWindow *window = new MainWindow;
  window->show();

  // Start the event loop
  return app.exec();
}
// (END) main
