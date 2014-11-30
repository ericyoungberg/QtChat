/*
 * FILE: ChatSettings.cpp
 * Declares the ChatSettings class along with all of its methods.
 */

#include <QtGui>
#include <QtWidgets>

#include "ChatSettings.h"

#define SETTINGS_FILE "resources/settings.ini"

//----------------------------------------------------------------------
// CLASS: ChatSettings
// Creates the dialog for editing the applications settings and
// user preferences
//----------------------------------------------------------------------
ChatSettings::ChatSettings(QWidget *parent) : QDialog(parent) {

  // LAYOUT
  // =================================
  
  // Init
  QHBoxLayout *mainLayout = new QHBoxLayout;
  QVBoxLayout *labelLayout = new QVBoxLayout;
  QVBoxLayout *inputLayout = new QVBoxLayout;
  QHBoxLayout *buttonLayout = new QHBoxLayout;

  
  // Define
  userNameLabel = new QLabel(tr("Username: "));
  userNameLabel->setMinimumHeight(20);
  userColorLabel = new QLabel(tr("Color: "));
  userColorLabel->setMinimumHeight(27);

  userNameEdit = new QLineEdit;
  userColorEdit = new QLineEdit;

  saveButton = new QPushButton(tr("Save"));
  saveButton->setDefault(true);
  cancelButton = new QPushButton(tr("Cancel"));


  // Setup
  labelLayout->addWidget(userNameLabel);
  labelLayout->addWidget(userColorLabel);
  labelLayout->addStretch();

  buttonLayout->addWidget(saveButton);
  buttonLayout->addWidget(cancelButton);

  inputLayout->addWidget(userNameEdit);
  inputLayout->addWidget(userColorEdit);
  inputLayout->addLayout(buttonLayout);

  mainLayout->addLayout(labelLayout);
  mainLayout->addLayout(inputLayout);


  // EVENTS
  // =================================

  connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
  connect(saveButton, SIGNAL(clicked()), this, SLOT(saveSettings()));


  // SETUP
  // =================================

  setLayout(mainLayout);
  setWindowTitle(tr("Settings"));
  setFixedSize(200, 400);
  layout()->setSizeConstraint(QLayout::SetFixedSize);

}
// (END) ChatSettings



/*
 * ---------------------------|
 * METHODS                    |
 * ---------------------------|
*/


//----------------------------------------------------------------------
// METHOD: loadSettings
// Loads the settings so that show the current information
//----------------------------------------------------------------------
void ChatSettings::loadSettings() {
  QSettings settings(SETTINGS_FILE, QSettings::IniFormat);
  userNameEdit->setText(settings.value("user_name", "NewUser").toString());
  userColorEdit->setText(settings.value("user_color", "red").toString());
}
// (END) loadSettings


//----------------------------------------------------------------------
// METHOD: saveSettings
// Emits a signal to the rest of the application so it can right the new
// information to the settings file
//----------------------------------------------------------------------
void ChatSettings::saveSettings() {
  QSettings settings(SETTINGS_FILE, QSettings::IniFormat);
  settings.setValue("user_name", userNameEdit->text());
  settings.setValue("user_color", userColorEdit->text());

  emit settingsUpdated();
  close();
}
// (END) saveSettings
