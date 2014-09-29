#include <conversationbox.h>
#include <iostream>

using std::cout;
using std::endl;

//Constructor
ConversationBox::ConversationBox(char ID[20]) {

  // Set the conversationID member to reference when looking up conversations
  strcpy(conversationID, ID);

  cout << "ConversationID: " << conversationID << endl;

  // Set the text of the conversation box if there wasn't a history file found
  if(strcmp(conversationID, "Default") == 0) {
    setText("<span style='color:#666;font-size:10px;'>Welcome to QtChat!</span>"); 
  } else {
    char convoStarter[40] = "New conversation started with ";
    setText(conversationID);
  }
}
