/*
 * FILE: mainwindow.cpp
 * Declares the MainWindow class along with all of its methods.
 */


#include <QtGui>
#include <QtWidgets>
#include <mainwindow.h>
#include <newcontactdialog.h>
#include <chatinput.h>

// CLASS: MainWindow
// Holds everything in the application
MainWindow::MainWindow() {

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
  connect(addContactDialog, SIGNAL(userFound(QString)), this, SLOT(addUserToContacts(QString)));

  
  // SETUP
  // =======================================

  QWidget *window = new QWidget;      // create the window
  window->setLayout(mainLayout);      // set the content
  window->setMinimumSize(700, 550);   // define the dimensions

  // NULL initialize the dialogs to speed up launch and preserve memory
  addContactDialog = 0;

  setCentralWidget(window);           // own it
}
// (END) MainWindow



/*
 * ---------------------------|
 * METHODS                    |
 * ---------------------------|
*/

// METHOD: showAddContact
// Shows the dialog to add a new contact to the user's "Contact" list
void MainWindow::showAddContact() {

  // Check if the dialog exists yet
  if(!addContactDialog) {
    addContactDialog = new NewContactDialog(this); // Initialize the dialog
  }

  addContactDialog->clearText(); // Reset the dialog for a new entry

  // Show and focus the dialog
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


// METHOD: sendMessage
// Sends a text based message to the user on the receiving end
void MainWindow::sendMessage() {

  // If there is something to send, try to send it
  if(inputIsEmpty()) {

    // Format the message to be presented
    QString message = "<span style='color:red;'>UserName: </span>"+
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
