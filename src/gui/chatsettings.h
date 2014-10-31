#ifndef CHATSETTINGS_H
#define CHATSETTINGS_H

#define SETTINGS_FILE "resources/settings.ini"

#include <QDialog>

class QLineEdit;
class QLabel;
class QPushButton;

class ChatSettings : public QDialog 
{
  Q_OBJECT

public:
    ChatSettings(QWidget *parent = 0);
public slots:
  void loadSettings();
private slots:
  void saveSettings();
signals:
  void settingsUpdated();
private:
  QLineEdit *userNameEdit;
  QLineEdit *realNameEdit;

  QLabel *userNameLabel;
  QLabel *realNameLabel;

  QPushButton *saveButton;
  QPushButton *cancelButton;
};

#endif
