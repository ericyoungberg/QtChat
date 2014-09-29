#include <conversationbox.h>

ConversationBox::ConversationBox() {
  conversationID = new char[20];
}

ConversationBox::ConversationBox(char* ID) {
  strcpy(conversationID, ID);

  setText(strcat((char*)"New conversation started with ", conversationID));
}
