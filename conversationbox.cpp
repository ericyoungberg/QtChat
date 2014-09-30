#include <conversationbox.h>
#include <iostream>

using std::cout;
using std::endl;

//Constructor
ConversationBox::ConversationBox(QString IP) {

  // Set the conversationID member to reference when looking up conversations
  conversationID = IP; 

  // Set the text of the conversation box if there wasn't a history file found
  if(conversationID == "Default") {
    setText("<span style='color:#666;font-size:10px;'>Welcome to QtChat!</span>"); 
  } else {
    char convoStarter[80] = "<span style='color:#666;font-size:10px;'>New conversation started with ";
    setText(convoStarter + conversationID + "</span>");

  }
}
