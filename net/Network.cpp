#include <iostream>
#include <unistd.h>

using std::cout;
using std::endl;

int main(int argc, char* argv[])  {

  cout << "Executed the Network program...\n";

  cout << "ARG 0: " << argv[1];

  /*
  while(true) {
    sleep(2);
    cout << "P1 - I: " << argv[1][0] << ", O: " << argv[1][1] << endl;
    cout << "P2 - I: " << argv[2][0] << ", O: " << argv[2][1] << endl;
    cout << endl;
  }
  */

  return 0;
}
