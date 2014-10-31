/*
 * FILE: mainwindow.h
 * Defines the MainWindow class
 * Further class info in the mainwindow.cpp file
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "NetworkHandler.h"


class QHBoxLayout;
class QVBoxLayout;
class QListWidget;
class QListWidgetItem;
class QPushButton;
class QTextEdit;
class QTextBrowser;
class QLabel;
class NewContactDialog;
class ChatInput;
class QAction;
class ChatSettings;
class ConversationBox;
class QStackedWidget;


class MainWindow : public QMainWindow 
{
  Q_OBJECT

public:
    MainWindow();
private slots:
  void showAddContact();
  void sendMessage();
  void addUserToContacts(QString userAddress);
  void removeSelectedContact();
  void startConversation(QListWidgetItem*);
  void deleteHistory();
  void closeConversation();
  void quitApp();
  void openSettings();
  void setEnvironment();
private:
  bool inputIsEmpty();

  int servProc;
  int pipeIn;
  int pipeOut;

  NetworkHandler *network;

   QString userName;
   QString realName;

   char currentConversation[160];
   bool hadPreviousConversation;

   QHBoxLayout *mainLayout; 
   QHBoxLayout *inputLayout;
   QHBoxLayout *upperContactLayout;

   QVBoxLayout *chatLayout;
   QVBoxLayout *contactLayout;

   QListWidget *contactList;

   QStackedWidget *conversations;
   ConversationBox *defaultConversation;

   ChatInput *userInput;

   ChatSettings *settingsDialog;

   QPushButton *sendMessageButton;
   QPushButton *addContactButton;

   QLabel *contactLabel;

   NewContactDialog *addContactDialog;

   QMenu *fileMenu;
   QMenu *editMenu;

   QAction *addAction;
   QAction *removeAction;
   QAction *deleteHistoryAction;
   QAction *closeAction;
   QAction *quitAction;
   QAction *settingsAction;
};

#endif
