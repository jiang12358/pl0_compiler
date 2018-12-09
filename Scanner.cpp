#include "Scanner.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
namespace compiler {
void Scanner::ScanFile() {
  std::ifstream input(InFile.c_str());
  std::string temp;
  if (!input.is_open()) {
    std::cout << "can't open file!" << std::endl;
    exit(0);
  }
  while (getline(input, temp)) {
    Code = Code + " " + temp;
  }
  Code = Code + " ";
}

/*std::string GetNextLine() {
  CodeLine = CodeQueue.top();
  LineNextChar = CodeLine.begin();
  return CodeLine;
};*/

bool Scanner::GetNextChar(char &c) {
  if (this->ThisChar >= this->Code.size()) {
    std::exit(0);
    return false;
  } else {
    c = Code[this->ThisChar];
    this->ThisChar++;
    return true;
  }
}
}  // namespace compiler
