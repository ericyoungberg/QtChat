/*
 * FILE: Helpers.h
 * A couple of helper functions that don't belong in any class
 */

#ifndef GUIHELPERS_H
#define GUIHELPERS_H

#include <QtWidgets>

class QString;

char* stripQ(QString); 
char* createFilePath(const char*, char*, const char*);

QIcon onlineStatusIcon(bool);

#endif
