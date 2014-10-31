/*
 * FILE: helpers.h
 * A couple of helper functions of mine that don't belong in any class
 */

#include <QtWidgets>

class QString;

char* stripQ(QString); 
char* createFilePath(const char*, char*, const char*);
