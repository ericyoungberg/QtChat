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
#include "mainwindow.h"

using std::cout;

// FUNCTION: main
// Event loops and the application domain
int main(int argc, char *argv[]) {

  // Define the application
  QApplication app(argc, argv);

  int pipe1[2], pipe2[2];

  pipe(pipe1);
  pipe(pipe2);

  if(!fork()) {
    execl("./net/QtServer", "QtServer", (char*)0); 
  } else {
     // Define and launch the application window
    MainWindow *window = new MainWindow;
    window->show();

    // Start the event loop
    return app.exec();
  }
}
// (END) main
