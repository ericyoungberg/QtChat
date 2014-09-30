/*
 * FILE: mainwindow.cpp
 * Declares the MainWindow class along with all of its methods.
 */


#include <QtGui>
#include <QtWidgets>
#include <mainwindow.h>
#include <newcontactdialog.h>
#include <chatinput.h>
#include <chatsettings.h>
#include <conversationbox.h>
#include <fstream>
#include <iostream>
#include "helpers.h"

using namespace std;


// ENVIRONMENT

#define CONTACTS_FILE "resources/contacts"
#define SETTINGS_FILE "resources/settings.ini"
#define HIST_SUFFIX ".hist"
#define HIST_PREFIX "resources/hist/"

// (end) ENVIRONMENT



//----------------------------------------------------------------------
// CLASS: MainWindow
// Holds everything in the application
//----------------------------------------------------------------------
MainWindow::MainWindow() {

  // ACTIONS
  // ================================

  // Format
  //--------------------------|
  // INITIALIZE               |
  // SHORTCUT                 |
  // EVENT                    |
  //--------------------------|

  // Add Contact
  addAction = new QAction(tr("Add Contact"), this);
  addAction->setShortcut(tr("Ctrl+n"));
  connect(addAction, SIGNAL(triggered()), this, SLOT(showAddContact()));

  // Remove Contact
  removeAction = new QAction(tr("Remove Contact"), this);
  removeAction->setShortcut(tr("Ctrl+r"));
  connect(removeAction, SIGNAL(triggered()), this, SLOT(removeSelectedContact()));

  // Delete Conversation History
  deleteHistoryAction = new QAction(tr("Delete Conversation History"), this);
  // No shortcut, too dangerous
  connect(deleteHistoryAction, SIGNAL(triggered()), this, SLOT(deleteHistory()));

  // Close Conversation
  closeAction = new QAction(tr("Close Conversation"), this);
  closeAction->setShortcut(tr("Ctrl+w"));
  connect(closeAction, SIGNAL(triggered()), this, SLOT(closeConversation()));

  // Quit Application
  quitAction = new QAction(tr("Quit"), this);
  quitAction->setShortcut(tr("Ctrl+q"));
  connect(quitAction, SIGNAL(triggered()), this, SLOT(quitApp()));

  // Open Settings
  settingsAction = new QAction(tr("Settings"), this);
  settingsAction->setShortcut(tr("Ctrl+,"));
  connect(settingsAction, SIGNAL(triggered()), this, SLOT(openSettings()));



  // TOOLBARS
  // ================================

  // File
  fileMenu = menuBar()->addMenu(tr("File"));
  fileMenu->addAction(settingsAction);
  fileMenu->addSeparator();
  fileMenu->addAction(quitAction);
  
  // Edit
  editMenu = menuBar()->addMenu(tr("Edit"));
  editMenu->addAction(addAction);
  editMenu->addAction(removeAction);
  editMenu->addSeparator();
  editMenu->addAction(closeAction);
  editMenu->addAction(deleteHistoryAction);



  // LAYOUT
  // ================================

  // User Input Section

  inputLayout = new QHBoxLayout;                          // init
  userInput = new ChatInput;
  sendMessageButton = new QPushButton(tr("Send"));
  sendMessageButton->setToolTip(tr("Send your message")); // define
  sendMessageButton->setMinimumHeight(100);
  sendMessageButton->setMaximumWidth(50);
  sendMessageButton->setDefault(true);
  inputLayout->addWidget(userInput, 1);                   // setup
  inputLayout->addWidget(sendMessageButton);
  

  // Chat Window Section
  chatLayout = new QVBoxLayout;                           // init
  conversations = new QStackedWidget;
  char defaultTitle[16] = "Default";
  defaultConversation = new ConversationBox(defaultTitle);
  conversations->addWidget(defaultConversation);
  chatLayout->addWidget(conversations, 1);                   // setup
  chatLayout->addLayout(inputLayout);
  

  // Contact Window Section
  contactLayout = new QVBoxLayout;                        // init
  upperContactLayout = new QHBoxLayout;
  contactList = new QListWidget;
  contactList->setMaximumWidth(150);                      // define
  contactLabel = new QLabel(tr("Contacts"));
  addContactButton = new QPushButton("+");
  addContactButton->setMaximumWidth(20);
  addContactButton->setToolTip(tr("Add a new contact"));
  upperContactLayout->addWidget(contactLabel, 1);         // setup
  upperContactLayout->addWidget(addContactButton);
  contactLayout->addLayout(upperContactLayout);
  contactLayout->addWidget(contactList);

  // Main Section
  mainLayout = new QHBoxLayout;                           // init
  mainLayout->addLayout(chatLayout, 1);                   // setup
  mainLayout->addLayout(contactLayout);
  

  // EVENTS
  // =======================================

  connect(sendMessageButton, SIGNAL(clicked()), this, SLOT(sendMessage()));
  connect(addContactButton, SIGNAL(clicked()), this, SLOT(showAddContact()));
  connect(userInput, SIGNAL(enterKeyPressed()), this, SLOT(sendMessage()));
  connect(contactList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(startConversation(QListWidgetItem*)));

  
  // SETUP
  // =======================================

  setEnvironment();                            // load the settings file

  QWidget *window = new QWidget;      // create the window
  window->setLayout(mainLayout);      // set the content
  window->setMinimumSize(700, 550);   // define the dimensions

  // NULL initialize the dialogs to speed up launch and preserve memory
  addContactDialog = 0;
  settingsDialog = 0;

  hadPreviousConversation = false;        // Initialize this value as false at the startup 
  currentConversation[0] = '\0';          // Initialize the curretConversation array

  setCentralWidget(window);           // own it
}
// (END) MainWindow



/*
 * ---------------------------------------------------------------------|
 * METHODS                                                              |
 * ---------------------------------------------------------------------|
*/


//----------------------------------------------------------------------
// METHOD: sendMessage
// Sends a text based message to the user on the receiving end
//----------------------------------------------------------------------
void MainWindow::sendMessage() {

  // If there is something to send, try to send it
  if(inputIsEmpty()) {

    // Format the message to be presented
    QString message = "<span style='color:red;'>"+userName+": </span>"+
                       userInput->toPlainText(); 

    // Write the message to the ConversationBox
    conversations->currentWidget()->findChild<ConversationBox*>()->append(message);
    userInput->setText("");      // reset the input field for the user
  }
 }
// (END) sendMessage


//----------------------------------------------------------------------
// METHOD: inputIsEmpty
// Checks if the input box doesn't contain any text
//----------------------------------------------------------------------
bool MainWindow::inputIsEmpty() {
  return (userInput->toPlainText() != "") ? true : false;
}
// (END) inputIsEmpty


//----------------------------------------------------------------------
// METHOD: setEnvironment
// Loads the settings file
//----------------------------------------------------------------------
void MainWindow::setEnvironment() {

  // Load the settings
  QSettings settings(SETTINGS_FILE, QSettings::IniFormat);
  userName = settings.value("user_name", "").toString();
  realName = settings.value("real_name", "").toString();

  // Load the contacts
  ifstream contactsFile;
  contactsFile.open(CONTACTS_FILE);

  // Placeholders
  char rawIP[15];

  // Make sure file is open before reading
  if(contactsFile.is_open()) {
    contactsFile >> rawIP;
    // Read until the end of file
    while(!contactsFile.eof()) {
      contactList->addItem(rawIP);
      contactsFile >> rawIP;
    }

    // Close the contacts file
    contactsFile.close(); 
  }
}
// (END) setEnvironment




//----------------------------------------------------------------------
// ACTION SLOTS
// ==========================================
//----------------------------------------------------------------------


//----------------------------------------------------------------------
// METHOD: showAddContact
// Shows the dialog to add a new contact to the user's "Contact" list
//----------------------------------------------------------------------
void MainWindow::showAddContact() {

  // Check if the dialog exists yet
  if(!addContactDialog) {
    addContactDialog = new NewContactDialog(this);        // Initialize the dialog
    connect(addContactDialog, SIGNAL(foundUser(QString)), // Connect the signal from the dialog 
            this, SLOT(addUserToContacts(QString)));
  }

  addContactDialog->clearText(); // Reset the dialog for a new entry

  // Show and focus the dialog
  addContactDialog->setModal(true);
  addContactDialog->show();
  addContactDialog->activateWindow();
}
// (END) showAddContact


//----------------------------------------------------------------------
// METHOD: addUserToContacts
// If a user is found the the 'NewContactDialog', the user is added to the
// contact list
//----------------------------------------------------------------------
void MainWindow::addUserToContacts(QString userAddress) {

  // Create an output file stream to write to the Contacts file
  ofstream contactsFile;
  contactsFile.open(CONTACTS_FILE, ios::out | ios::app);  // Make sure we append the output

  // Make sure it is open before writing
  if(contactsFile.is_open()) {
    char* userAddressRaw = stripQ(userAddress);
    contactsFile << userAddressRaw << endl;    // Write to the file
    contactsFile.close();           // Close the file
  } else {
    // WILL HANDLE ERROR 
  }

  // Append the user to GUI contact list
  contactList->addItem(userAddress);
}
// (END) addUserToContacts


//----------------------------------------------------------------------
// METHOD: removeSelectedContact
// Finds the contact that the user selected from the 'Contact' list and then 
// deletes it.
//----------------------------------------------------------------------
void MainWindow::removeSelectedContact() {

  // Find the selected item(s)
  QList<QListWidgetItem*> selected = contactList->selectedItems();
  QListWidgetItem *selectedItem = selected.at(0);

  // Delete the user from the contacts file
  // Read from the current contact file
  ifstream contactsFile;
  contactsFile.open(CONTACTS_FILE, ios::in);

  // Using temporary file to transfer contacts that will be staying with the user
  ofstream tempFile;
  tempFile.open(".tempdata", ios::out);

  char IP[15];  // Input placeholder

  // Make sure both files are open before performing any action
  if(contactsFile.is_open() && tempFile.is_open()) {
    contactsFile >> IP; 
    while(!contactsFile.eof()) {

      // If the IP address read from CONTACTS_FILE is not the IP selected,
      // write it to the temporary data file
      if(IP != selectedItem->text())
        tempFile << IP << endl;

      contactsFile >> IP;   // Read next IP address until end of file 
    } 

    // Close both files
    contactsFile.close(); 
    tempFile.close();
    
    // Replace the old contacts file with the temporary data
    remove(CONTACTS_FILE);
    rename(".tempdata", CONTACTS_FILE);
  }

  // Find the selected item in the list and then remove it
  for(int i=0;i<contactList->count();i++) {
    if(contactList->item(i) == selectedItem) {
      contactList->takeItem(i);                       // take
      contactList->removeItemWidget(selectedItem);  // delete
    }
  }

}
// (END) removeSelectedContact


//----------------------------------------------------------------------
// METHOD: startConversation
// Adds a new 'messageBox' to the stack
//----------------------------------------------------------------------
void MainWindow::startConversation(QListWidgetItem* item) {

  // Get the IP address of the selected item
  QString IP = item->text();

  //Setup a new conversation box if there is not one found/already setup
  //If so, then exit
  //If not found, then move forward to create a new conversation box
  for(int i=0;i<conversations->count();i++) {

    // If there is a ConversationBox found within this widget
    if(ConversationBox *conversation = conversations->widget(i)->findChild<ConversationBox*>()) {

      // If this is the selected conversation, set it as current and then return
      if(conversation->conversationID == IP) {
        conversations->setCurrentWidget(conversations->widget(i));  
        return;
      } 
    }
  }

  //Create a new conversation box
  ConversationBox *newConversation = new ConversationBox(IP);

  // Add the ConversationBox to a layout
  QHBoxLayout *newConversationLayout = new QHBoxLayout;
  newConversationLayout->addWidget(newConversation);

  // Embed that layout into the wrapper widget
  QWidget *newConversationWrap = new QWidget;
  newConversationWrap->setLayout(newConversationLayout);

  // Add the whole thing to the StackedWidget
  conversations->addWidget(newConversationWrap);
  conversations->setCurrentWidget(conversations->widget(conversations->count()-1));
  
}
// (END) startConversation


//----------------------------------------------------------------------
// METHOD: deleteHistory
// Erases the entire conversation history between another user
//----------------------------------------------------------------------
void MainWindow::deleteHistory() {
  
  // Grab the current conversation
  ConversationBox *conversation = conversations->currentWidget()->findChild<ConversationBox*>();

  // Opens up the file to write to 
  QFile file("resources/hist/" + conversation->conversationID + ".hist");

  // If the file is open, then write the current conversation to it
  if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    QTextStream output(&file);
    output << "";   // Make sure nothing is written to the file
  }

  // Clear the current view of the conversation
  conversation->setText("");
}
// (END) deleteHistory


//----------------------------------------------------------------------
// METHOD: closeConversation
// Removes the 'messageBox' from the stack
//----------------------------------------------------------------------
void MainWindow::closeConversation() {

  // Grab the current conversation
  ConversationBox *conversation = conversations->currentWidget()->findChild<ConversationBox*>();

  // Opens up the file to write to 
  QFile file("resources/hist/" + conversation->conversationID + ".hist");

  // If the file is open, then write the current conversation to it
  if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    QTextStream output(&file);
    output << conversation->toHtml();   // Write as HTML since that's how everything is formatted
  }

  // Pop the current conversation from the conversations list
  conversations->removeWidget(conversations->currentWidget());
}
// (END) closeConversation


//----------------------------------------------------------------------
// METHOD: quitApp
// Closes the application after writing everything that happened that session
//----------------------------------------------------------------------
void MainWindow::quitApp() {
  
  // Make sure to write all open conversations to disk
  for(int i=0;i<conversations->count();i++) {

    // Make sure it found a conversation in that widget
    if(ConversationBox *conversation = conversations->widget(i)->findChild<ConversationBox*>()) {

      // We don't want to write the default window
      if(conversation->conversationID != "Default") {

        // Opens up the file to write to 
        QFile file("resources/hist/" + conversation->conversationID + ".hist");

        // If the file is open, then write the current conversation to it
        if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
          QTextStream output(&file);
          output << conversation->toHtml();   // Write as HTML since that's how everything is formatted
        }    
      }
    }
  }

  // Close the application
  close();

}
// (END) quitApp


//----------------------------------------------------------------------
// METHOD: openSettings
// Opens the settings dialog so the user can change their preferences
//----------------------------------------------------------------------
void MainWindow::openSettings() {
  if(!settingsDialog) {
    settingsDialog = new ChatSettings;
    connect(settingsDialog, SIGNAL(settingsUpdated()), this, SLOT(setEnvironment()));
  }  

  settingsDialog->loadSettings();   // Reset LineEdits to the current settings

  settingsDialog->setModal(true);   // Keep focus on this window
  settingsDialog->show();
  settingsDialog->activateWindow();
}
// (END) openSettings
