/*
 * FILE: newcontactdialog.h
 * Defines the NewContactDialog class. See newcontactdialog.cpp for further info.
 */

#ifndef NEWCONTACTDIALOG_H
#define NEWCONTACTDIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;

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

#endif
