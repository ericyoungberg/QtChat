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
class QAction;
class ChatSettings;
class QString;

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
  void startConversation();
  void deleteHistory();
  void closeConversation();
  void quitApp();
  void openSettings();
  void setEnvironment();
private:
  bool inputIsEmpty();

   QString userName;
   QString realName;

   QHBoxLayout *mainLayout; 
   QHBoxLayout *inputLayout;
   QHBoxLayout *upperContactLayout;

   QVBoxLayout *chatLayout;
   QVBoxLayout *contactLayout;

   QListWidget *contactList;

   ChatInput *userInput;

   ChatSettings *settingsDialog;

   QPushButton *sendMessageButton;
   QPushButton *addContactButton;

   QTextBrowser *messageBox;

   QLabel *contactLabel;

   NewContactDialog *addContactDialog;

   QMenu *fileMenu;
   QMenu *editMenu;

   QAction *addAction;
   QAction *removeAction;
   QAction *startAction;
   QAction *deleteHistoryAction;
   QAction *closeAction;
   QAction *quitAction;
   QAction *settingsAction;
};

#endif
