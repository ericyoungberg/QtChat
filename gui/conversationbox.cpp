#include <QFile>
#include <conversationbox.h>
#include <iostream>

using std::cout;
using std::endl;

//Constructor
ConversationBox::ConversationBox(QString IP) {

  // Set the conversationID member to reference when looking up conversations
  conversationID = IP; 

  // The history file for this conversation
  QFile file("resources/hist/"+IP+".hist"); 

  // If there is a file found, then open it
  // If not, then fill the ConversationBox with entry information
  if(file.open(QIODevice::ReadOnly)) {
    QString input;
    while(!file.atEnd()) {
      QByteArray line = file.readLine();
      input.append(line);
    } 
    setText(input);
  } else {
    // Set the text of the conversation box if there wasn't a history file found
    if(conversationID == "Default") {
      setText("<span style='color:#666;font-size:10px;'>Welcome to QtChat!</span>"); 
    } else {
      char convoStarter[80] = "<span style='color:#666;font-size:10px;'>New conversation started with ";
      setText(convoStarter + conversationID + "</span>");
    }  
  }
}
