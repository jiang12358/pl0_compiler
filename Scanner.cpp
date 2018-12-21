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
	codestrings.emplace_back(temp + " ");
  }
  Code = Code + " ";
}

/*std::string GetNextLine() {
  CodeLine = CodeQueue.top();
  LineNextChar = CodeLine.begin();
  return CodeLine;
};*/

bool Scanner::GetNextChar(char &c) {
	while (codestrings[ThisLine].size() == 0) {
		ThisLine++;
	}
	if (ThisChar >= codestrings[ThisLine].size() - 1) {
c = codestrings[ThisLine][ThisChar];
		ThisChar = 0;
		ThisLine++;
		
	}
	else {
		
		c = codestrings[ThisLine][ThisChar];
		ThisChar++;
	}
	return true;
  /*if (this->ThisChar >= this->Code.size()) {
    std::exit(0);
    return false;
  } else {
    c = Code[this->ThisChar];
    this->ThisChar++;
    return true;
  }*/
}

std::string Scanner::GetThisLine() {
	return codestrings[ThisLine];
}
}  // namespace compiler
