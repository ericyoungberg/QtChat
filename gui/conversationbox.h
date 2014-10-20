#ifndef CONVERSATIONBOX_H
#define CONVERSATIONBOX_H

#include <QTextBrowser>

class ConversationBox : public QTextBrowser
{
public:
  ConversationBox(QString);

  QString conversationID;
};

#endif
