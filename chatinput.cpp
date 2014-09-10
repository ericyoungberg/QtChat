/*
 * FILE: chatinput.cpp
 * Declares the ChatInput class along with all of its methods.
 */

#include <QTextEdit>
#include <QKeyEvent>
#include <chatinput.h>


// CLASS: ChatInput
// The input box where the user types their messages to send
ChatInput::ChatInput() {
  setMaximumHeight(100); 
}
// (END) ChatInput


/*
 * ---------------------------|
 * METHODS                    |
 * ---------------------------|
*/

// METHOD: keyPressEvent
// Overrides the protected key press event
void ChatInput::keyPressEvent(QKeyEvent *event) {

  // If the user presses the 'Enter' key while in the text box,
  // send the message instead of adding a new line
  if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
    emit enterKeyPressed();
  } else {
    QTextEdit::keyPressEvent(event);
  }
}
// (END) keyPressEvent
