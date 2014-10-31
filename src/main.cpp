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

using std::cout;

// FUNCTION: main
// Event loops and the application domain
int main(int argc, char *argv[]) {

  /*
  int serv_pipe[2], gui_pipe[2];

  // Create the pipes
  pipe(serv_pipe);
  pipe(gui_pipe);

  // Fork off a seperate process for the network server
  // and the GUI
  int cpid = fork();

  if(cpid == 0) {


    // Close pipe ends that aren't needed here
    close(serv_pipe[1]);
    close(gui_pipe[0]);
    NetworkHandler *network = new NetworkHandler(serv_pipe[0], gui_pipe[1]); 

  } else {


    // Close pipe ends that aren't needed here
    close(serv_pipe[1]);
    close(gui_pipe[0]);
*/
    // Define the application
    QApplication app(argc, argv);

    // Define and launch the application window
    MainWindow *window = new MainWindow();
    window->show();

    // Start the event loop
    return app.exec();

 // }

}
// (END) main
