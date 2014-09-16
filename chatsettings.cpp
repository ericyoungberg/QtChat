#include <QtGui>
#include <QtWidgets>
#include <chatsettings.h>


// CLASS: ChatSettings
// Creates the dialog for editing the applications settings and
// user preferences
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
  realNameLabel = new QLabel(tr("Real name: "));
  realNameLabel->setMinimumHeight(27);

  userNameEdit = new QLineEdit;
  realNameEdit = new QLineEdit;

  saveButton = new QPushButton(tr("Save"));
  saveButton->setDefault(true);
  cancelButton = new QPushButton(tr("Cancel"));


  // Setup
  labelLayout->addWidget(userNameLabel);
  labelLayout->addWidget(realNameLabel);
  labelLayout->addStretch();

  buttonLayout->addWidget(saveButton);
  buttonLayout->addWidget(cancelButton);

  inputLayout->addWidget(userNameEdit);
  inputLayout->addWidget(realNameEdit);
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

// METHOD: loadSettings
// Loads the settings so that show the current information
void ChatSettings::loadSettings() {
  QSettings settings(SETTINGS_FILE, QSettings::IniFormat);
  userNameEdit->setText(settings.value("user_name", "").toString());
  realNameEdit->setText(settings.value("real_name", "").toString());
}
// (END) loadSettings


// METHOD: saveSettings
// Emits a signal to the rest of the application so it can right the new
// information to the settings file
void ChatSettings::saveSettings() {
  QSettings settings(SETTINGS_FILE, QSettings::IniFormat);
  settings.setValue("user_name", userNameEdit->text());
  settings.setValue("real_name", realNameEdit->text());

  emit settingsUpdated();
  close();
}
// (END) saveSettings









