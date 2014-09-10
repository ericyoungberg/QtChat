/*
 * FILE: mainwindow.h
 * Defines the MainWindow class
 * Further class info in the mainwindow.cpp file
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QHBoxLayout;
class QVBoxLayout;
class QListWidget;
class QPushButton;
class QTextEdit;
class QTextBrowser;
class QLabel;
class NewContactDialog;
class ChatInput;

class MainWindow : public QMainWindow 
{
  Q_OBJECT

public:
    MainWindow();
private slots:
  void showAddContact();
  void sendMessage();
  void addUserToContacts(QString userAddress);
private:
  bool inputIsEmpty();

   QHBoxLayout *mainLayout; 
   QHBoxLayout *inputLayout;
   QHBoxLayout *upperContactLayout;

   QVBoxLayout *chatLayout;
   QVBoxLayout *contactLayout;

   QListWidget *contactList;

   ChatInput *userInput;

   QPushButton *sendMessageButton;
   QPushButton *addContactButton;

   QTextBrowser *messageBox;

   QLabel *contactLabel;

   NewContactDialog *addContactDialog;
};

#endif
