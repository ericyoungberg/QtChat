/*
 * FILE: chatinput.h
 * Defines the ChatInput class. See chatinput.cpp for further info.
 */

#ifndef CHATINPUT_H
#define CHATINPUT_H

#include <QTextEdit>

class ChatInput : public QTextEdit
{
  Q_OBJECT

public:
    ChatInput();
signals:
    void enterKeyPressed();
protected:
    void keyPressEvent(QKeyEvent *event);
};

#endif
