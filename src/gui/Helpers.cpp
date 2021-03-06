/*
 * FILE: Helpers.cpp
 * A couple of helper functions that don't belong in any class
 */

#include <QtWidgets>
#include <iostream>

using namespace std;


//----------------------------------------------------------------------
// FUNCTION: stripQ
// Filters all of the Qt specific stuff from a string
//----------------------------------------------------------------------
char* stripQ(QString str) {
  QByteArray ba = str.toLocal8Bit();
  return ba.data();
}
// (end) stripQ


//----------------------------------------------------------------------
// FUNCTION: createFilePath
// Adds three character arrays together
//----------------------------------------------------------------------
char* createFilePath(const char* ch1, char* ch2, const char* ch3) {
  char* ch4 = new char[strlen(ch1)+strlen(ch2)+strlen(ch3)+1];
  strcpy(ch4, ch1);
  strcat(ch4, ch2);
  strcat(ch4, ch3);
  return ch4;
}
// (END) createFilePath


//----------------------------------------------------------------------
// FUNCTION: onlineStatusIcon
// Builds an online status QIcon
//----------------------------------------------------------------------
QIcon onlineStatusIcon(bool online) {
  QString path = (online) ? "src/gui/icons/online.png" : "src/gui/icons/offline.png";
  QIcon icon;
  icon.addFile(path); 

  return icon;
}
// (END) onlineStatusIcon
