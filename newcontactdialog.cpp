/*
 * FILE: newcontactdialog.cpp
 * Declares the NewContactDialog class along with all of its methods.
 */

#include <QtGui>
#include <QtWidgets>
#include <newcontactdialog.h>


// CLASS: NewContactDialog
// A dialog for the user to add new contacts to their 'Contact' list
NewContactDialog::NewContactDialog(QWidget *parent) : QDialog(parent) {

  // LAYOUT
  // ==============================================

  QVBoxLayout *mainLayout = new QVBoxLayout;                        // init
  QHBoxLayout *inputLayout = new QHBoxLayout;
  QHBoxLayout *buttonLayout = new QHBoxLayout;

  addPromptLabel = new QLabel(tr("Enter the user's IP address"));   // define
  addressInput = new QLineEdit; 

  findButton = new QPushButton(tr("Find"));
  findButton->setDefault(true);
  cancelButton = new QPushButton(tr("Cancel"));

  inputLayout->addWidget(addPromptLabel);                           // setup
  inputLayout->addWidget(addressInput);

  buttonLayout->addWidget(findButton);
  buttonLayout->addWidget(cancelButton);

  mainLayout->addLayout(inputLayout);
  mainLayout->addLayout(buttonLayout);


  // EVENTS
  // ==============================================

  connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
  connect(findButton, SIGNAL(clicked()), this, SLOT(findUser()));


  // SETUP
  // ==============================================

  setLayout(mainLayout);
  setWindowTitle(tr("New Contact"));
  setFixedSize(300, 120);
  layout()->setSizeConstraint(QLayout::SetFixedSize); // no one should resize a dialog
}
// (END) NewContactDialog




/*
 * ---------------------------|
 * METHODS                    |
 * ---------------------------|
*/

// METHOD: clearText
// Wrapper for private input
void NewContactDialog::clearText() {
  addressInput->setText("");
}
// (END) clearText


// METHOD: findUser
// Try to find an account online through the IP address entered by the user
void NewContactDialog::findUser() {
  emit foundUser(addressInput->text());
  close();
}
// (END) findUser
