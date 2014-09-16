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

// CLASS: MainWindow
// Holds everything in the application
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

  // Start Conversation
  startAction = new QAction(tr("Start Conversation"), this);
  startAction->setShortcut(tr("Ctrl+e"));
  connect(removeAction, SIGNAL(triggered()), this, SLOT(startConversation()));

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
  editMenu->addAction(startAction);
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
  messageBox = new QTextBrowser;
  chatLayout->addWidget(messageBox, 1);                   // setup
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

  
  // SETUP
  // =======================================

  setEnvironment();                            // load the settings file


  QWidget *window = new QWidget;      // create the window
  window->setLayout(mainLayout);      // set the content
  window->setMinimumSize(700, 550);   // define the dimensions

  // NULL initialize the dialogs to speed up launch and preserve memory
  addContactDialog = 0;
  settingsDialog = 0;


  setCentralWidget(window);           // own it
}
// (END) MainWindow



/*
 * ---------------------------|
 * METHODS                    |
 * ---------------------------|
*/


// METHOD: sendMessage
// Sends a text based message to the user on the receiving end
void MainWindow::sendMessage() {

  // If there is something to send, try to send it
  if(inputIsEmpty()) {

    // Format the message to be presented
    QString message = "<span style='color:red;'>"+userName+": </span>"+
                       userInput->toPlainText(); 

    messageBox->append(message); // add the message the user sent to the conversation
    userInput->setText("");      // reset the input field for the user
  }
 }
// (END) sendMessage


// METHOD: inputIsEmpty
// Checks if the input box doesn't contain any text
bool MainWindow::inputIsEmpty() {
  return (userInput->toPlainText() != "") ? true : false;
}
// (END) inputIsEmpty


// METHOD: setEnvironment
// Loads the settings file
void MainWindow::setEnvironment() {
  QSettings settings("resources/settings.ini", QSettings::IniFormat);
  userName = settings.value("user_name", "").toString();
  realName = settings.value("real_name", "").toString();
}
// (END) setEnvironment



// ACTION SLOTS
// ==========================================


// METHOD: showAddContact
// Shows the dialog to add a new contact to the user's "Contact" list
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


// METHOD: addUserToContacts
// If a user is found the the 'NewContactDialog', the user is added to the
// contact list
void MainWindow::addUserToContacts(QString userAddress) {
  contactList->addItem(userAddress);
}
// (END) addUserToContacts


// METHOD: removeSelectedContact
// Finds the contact that the user selected from the 'Contact' list and then 
// deletes it.
void MainWindow::removeSelectedContact() {

  // Find the selected item(s)
  QList<QListWidgetItem*> selected = contactList->selectedItems();

  // Find the selected item in the list and then remove it
  for(int i=0;i<contactList->count();i++)
    if(contactList->item(i) == selected.at(0)) {
      contactList->takeItem(i);                       // take
      contactList->removeItemWidget(selected.at(0));  // delete
    }
}
// (END) removeSelectedContact


// METHOD: startConversation
// Adds a new 'messageBox' to the stack
void MainWindow::startConversation() {

}
// (END) startConversation


// METHOD: deleteHistory
// Erases the entire conversation history between another user
void MainWindow::deleteHistory() {

}
// (END) deleteHistory


// METHOD: closeConversation
// Removes the 'messageBox' from the stack
void MainWindow::closeConversation() {

}
// (END) closeConversation


// METHOD: quitApp
// Closes the application after writing everything that happened that session
void MainWindow::quitApp() {

}
// (END) quitApp


// METHOD: openSettings
// Opens the settings dialog so the user can change their preferences
void MainWindow::openSettings() {
  if(!settingsDialog) {
    settingsDialog = new ChatSettings;
    connect(settingsDialog, SIGNAL(settingsUpdated()), this, SLOT(setEnvironment()));
  }  

  settingsDialog->setModal(true);   // Keep focus on this window
  settingsDialog->show();
  settingsDialog->activateWindow();
}
// (END) openSettings
