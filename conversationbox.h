#ifndef CONVERSATIONBOX_H
#define CONVERSATIONBOX_H

#include <QTextBrowser>

class ConversationBox : public QTextBrowser
{
public:
  ConversationBox();
  ConversationBox(char* ID);

  char* conversationID;
};

#endif
