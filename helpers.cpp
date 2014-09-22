/*
 * FILE: helpers.cpp
 * A couple of helper functions of mine that don't belong in any class
 */

#include <QtWidgets>

using namespace std;

// FUNCTION: stripQ
// Filters all of the Qt specific stuff from a string
char* stripQ(QString str) {
  QByteArray ba = str.toLocal8Bit();
  return ba.data();
}
// (end) stripQ


// FUNCTION: appendChars
// Adds two character arrays together
char* appendSuffix(char* ch1, const char* ch2) {
  char* ch3 = new char[strlen(ch1)+strlen(ch2)+1];
  strcpy(ch3, ch1);
  strcat(ch3, ch2);
  return ch3;
}
