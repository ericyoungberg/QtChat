#ifndef CONVERSATIONBOX_H
#define CONVERSATIONBOX_H

#include <QTextBrowser>

class ConversationBox : public QTextBrowser
{
public:
  ConversationBox(char ID[20]);

  char conversationID[20];
};

#endif
