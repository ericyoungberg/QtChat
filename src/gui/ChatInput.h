/*
 * FILE: ChatInput.h
 * Defines the ChatInput class. See ChatInput.cpp for further info.
 */

#ifndef CHATINPUT_H
#define CHATINPUT_H

#include <QTextEdit>

// ----------------------------------------------------------------
// CLASS: ChatInput
// The text box that the user inputs their messages to send
// ----------------------------------------------------------------
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
