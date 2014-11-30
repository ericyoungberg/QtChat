/*
 * FILE: NewContactDialog.h
 * Defines the NewContactDialog class. See NewContactDialog.cpp for further info.
 */

#ifndef NEWCONTACTDIALOG_H
#define NEWCONTACTDIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;

// ------------------------------------------------------------
// CLASS: NewContactDialog
// The dialog for adding new IPs to your contact list
// ------------------------------------------------------------
class NewContactDialog : public QDialog
{
  Q_OBJECT

public:
  NewContactDialog(QWidget *parent = 0);
  void clearText();
signals:
  void foundUser(QString userAddress);
private slots:
  void findUser();
private:
  QLabel *addPromptLabel;

  QLineEdit *addressInput;
  
  QPushButton *cancelButton;
  QPushButton *findButton;
};
// (END) NewContactDialog

#endif
