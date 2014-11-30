/*
 * FILE: MainWindow.cpp
 * Declares the MainWindow class along with all of its methods.
 */


#include <QtGui>
#include <QtWidgets>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

#include "Helpers.h"
#include "MainWindow.h"
#include "ConversationBox.h"
#include "ChatSettings.h"
#include "NewContactDialog.h"
#include "ChatInput.h"
#include "../net/NetworkHandler.h"
#include "../net/ChatAdaptor.h"
#include "../net/ChatInterface.h"

using namespace std;


// ENVIRONMENT

#define CONTACTS_FILE "resources/contacts"
#define SETTINGS_FILE "resources/settings.ini"

// (end) ENVIRONMENT



//----------------------------------------------------------------------
// CLASS: MainWindow
// Holds everything in the application GUI
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
  chatLayout->addWidget(conversations, 1);                // setup
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


  // IPC via Qt's D-Bus API
  new ChatAdaptor(this);
  QDBusConnection::sessionBus().registerObject("/", this);

  ipc = new ChatInterface(QString(), QString(), QDBusConnection::sessionBus(), this);
  
  connect(ipc, SIGNAL(queueRouter(QString, QString)), this, SLOT(route(QString, QString)));


  network = new NetworkHandler;       // Handles all of your outward network calls

  initialLoad = true;                 // Indicate that this is the first time setting the environment
  setEnvironment();                   // load the settings file

  QWidget *window = new QWidget;      // create the window
  window->setLayout(mainLayout);      // set the content
  window->setMinimumSize(700, 550);   // define the dimensions

  // initialize the dialogs to speed up launch
  addContactDialog = 0;
  settingsDialog = 0;

  hadPreviousConversation = false;    // Initialize this value as false at the startup 
  currentConversation[0] = '\0';      // Initialize the currentConversation array

  broadcast((char*)"CON");            // Let everyone in your contacts list know you are online
                                      // Doing this before showing the window cause a stale window is worse
                                      // than a window that hasn't shown yet (loading)

  setCentralWidget(window);           // own it
}
// (END) MainWindow



/*
 * ---------------------------------------------------------------------|
 * METHODS                                                              |
 * ---------------------------------------------------------------------|
*/


//----------------------------------------------------------------------
// METHOD: broadcast
// Sends everyone on the contact list a message
//----------------------------------------------------------------------
void MainWindow::broadcast(char* action) {
  for(int i=0;i<contactList->count();i++) {
    network->transmit((char*)contactList->item(i)->text().toStdString().c_str(), action);
  } 
}
// (END) broadcastEntrance


//----------------------------------------------------------------------
// METHOD: sendMessage
// Sends a text based message to the user on the receiving end
//----------------------------------------------------------------------
void MainWindow::sendMessage() {

  // If there is something to send, try to send it
  if(inputIsEmpty()) {

    // Format the message to be presented
    QString message = "<span style='color:"+userColor+";'>"+userName+": </span>"+userInput->toPlainText();

    // Write the message to the ConversationBox
    ConversationBox* currentConversation = conversations->currentWidget()->findChild<ConversationBox*>();
    currentConversation->append(message);

    // Append the message command to the user input before transmitting
    QString input("MES " + message);

    // Transmit the message across the internet of things
    network->transmit((char*)currentConversation->conversationID.toStdString().c_str(), (char*)input.toStdString().c_str());

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

  // Make the resources directory if one does not exist
  mkdir("resources", 0700);
  mkdir("resources/hist", 0700);

  // Load the settings
  QSettings settings(SETTINGS_FILE, QSettings::IniFormat);
  userName = settings.value("user_name", "NewUser").toString();
  userColor = settings.value("user_color", "red").toString();

  // Only load in the contacts if it is the first time setting the environment
  if(initialLoad) {

    initialLoad = false;  // Change the initialLoad flag to false
 
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
        contactList->addItem(new QListWidgetItem(onlineStatusIcon(false), rawIP));
        contactsFile >> rawIP;
      }

      // Close the contacts file
      contactsFile.close(); 
    } 
  }
}
// (END) setEnvironment


//----------------------------------------------------------------------
// METHOD: grabConversation
// Finds a ConversationBox. If we can't then we create a new one.
//----------------------------------------------------------------------
QWidget* MainWindow::grabConversation(QString IP, bool create = true) {
  //Setup a new conversation box if there is not one found/already setup
  //If so, then exit
  //If not found, then move forward to create a new conversation box
  for(int i=0;i<conversations->count();i++) {

    // If there is a ConversationBox found within this widget
    if(ConversationBox *conversation = conversations->widget(i)->findChild<ConversationBox*>()) {

      // If this is the selected conversation, set it as current and then return
      if(conversation->conversationID == IP) {
        return conversations->widget(i);  
      } 
    }
  }

  // Don't create a new widget if we are only wanting one that already exists
  if(!create) {
    return NULL; 
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

  return conversations->widget(conversations->count()-1);

}
// (END) grabConversation


//----------------------------------------------------------------------
// METHOD: closeEvent
// Overrides the default QDialog close event method
//----------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent *event) {
  emit quitApp();
  event->accept();
}
// (END) closeEvent

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
    contactsFile.close();                      // Close the file
  } else {
    cout << "Cannot open the contacts file...\n";
  }

  // Append the user to GUI contact list
  contactList->addItem(new QListWidgetItem(onlineStatusIcon(false), userAddress));

  // Check to see if the newly added user is online
  network->transmit((char*)userAddress.toStdString().c_str(), (char*)"CON");
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

  // Either find or make a new conversation
  QWidget *currentWidget = grabConversation(IP);
  
  // Set the latest conversation to the current widget showing
  conversations->setCurrentWidget(currentWidget);

  // Strip Qt qualities and create a new outward socket
  char* rawIP = stripQ(IP);
  if(network->createOutwardConnection(rawIP) == -1) {
    // If the connection failed, display it in the new conversation box
    conversations->currentWidget()->findChild<ConversationBox*>()->append("<span style='color:red;font-size:10px;'>Could not connect to "+IP+"</span>");
  }
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

        QString fileName("resources/hist/"+ conversation->conversationID + ".hist");

        // Opens up the file to write to 
        QFile file(fileName);

        // If the file is open, then write the current conversation to it
        if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
          QTextStream output(&file);
          output << conversation->toHtml();   // Write as HTML since that's how everything is formatted
        } else {
          cout << "Could not open up the file...\n";
        } 
      }
    }
  }

  broadcast((char*)"OFF"); // Let everyone on your contact list know that you have now logged off

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



//----------------------------------------------------------------------
// IPC SLOTS
// ==========================================
//----------------------------------------------------------------------


//----------------------------------------------------------------------
// METHOD: route
// Parses the string sent from the IPC server and then routes the
// signal to its respective function call
//----------------------------------------------------------------------
void MainWindow::route(QString IP, QString message) {

  // Pull the message type from what was received
  QString messageType = message.left(3);

  // The router
  if(messageType == "MES") {            // Message
    receivedMessage(IP, message.mid(4));
  } else if(messageType == "OFF") {     // A user disconnected
    loggedOff(IP);
  } else if(messageType == "REC") {     // Handshake from a user that read your connection reply confirming they are online too
    toggleOnlineStatus(IP, true);
  } else if(messageType == "CON") {     // A user connected
    connected(IP);
  } 

}
// (END) route


//----------------------------------------------------------------------
// METHOD: receivedMessage
// Handles incoming messages
//----------------------------------------------------------------------
void MainWindow::receivedMessage(QString IP, QString message) {
  // Find the conversation window
  QWidget *conversation = grabConversation(IP);

  // Display the message in that window
  conversations->widget(conversations->indexOf(conversation))->findChild<ConversationBox*>()->append(message);
}
// (END) receivedMessage


//----------------------------------------------------------------------
// METHOD: loggedOff
// Displays that the current user is offline
//----------------------------------------------------------------------
void MainWindow::loggedOff(QString IP) {

  // Change the online status of that user
  toggleOnlineStatus(IP, false);

  // Delete the connection from the connections vector stored in the NetworkHandler
  network->deleteOutwardConnection(stripQ(IP));

  // Find the conversation window
  QWidget *conversation = grabConversation(IP, false);

  // Report to the user that they are offline now only if they have an open conversation with them currently
  if(conversation) {
    conversations->widget(conversations->indexOf(conversation))->findChild<ConversationBox*>()->append("<span style='color:#aa0000;font-size:10px;'>"+IP+" has logged off...</span>");
  }
}
// (END) loggedOff


//----------------------------------------------------------------------
// METHOD: connected
// Changes the online status of a user and sends a handshake back
//----------------------------------------------------------------------
void MainWindow::connected(QString IP) {

  // Change the online status of that user
  toggleOnlineStatus(IP, true);

  // Transmit a handshake command to let that user know you are online as well
  network->transmit((char*)IP.toStdString().c_str(), (char*)"REC");
}
  
// (END) connected


//----------------------------------------------------------------------
// METHOD: toggleOnlineStatus
// Changes the online status of a user
//----------------------------------------------------------------------
void MainWindow::toggleOnlineStatus(QString IP, bool onlineStatus) {
  
  // Find the contact
  for(int i=0;i<contactList->count();i++) {
    if(contactList->item(i)->text() == IP) {
      QListWidgetItem *contact = contactList->item(i);  // Found the contact

      // Change the online status
      contact->setIcon(onlineStatusIcon(onlineStatus)); 
    } 
  }

}
// (END) toggleOnlineStatus
