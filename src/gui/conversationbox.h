#ifndef CONVERSATIONBOX_H
#define CONVERSATIONBOX_H

#include <QTextBrowser>

// ----------------------------------------------------------------
// CLASS: ConversationBox
// A widget that gets laid over other ConversationBoxes for
// switching around conversations without closing them
// ----------------------------------------------------------------
class ConversationBox : public QTextBrowser
{
public:
  ConversationBox(QString);

  QString conversationID;
};
// (END) ConversationBox

#endif
