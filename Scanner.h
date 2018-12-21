#ifndef COMPILER_SCANNER
#define COMPILER_SCANNER
#include <iostream>
#include <queue>
namespace compiler {
class Scanner {
 public:
  Scanner(){};

  void init(std::string filename) {
    InFile = filename;
    Code = "";
    ThisChar = 0;
	ThisLine = 0;
  };
  void ScanFile();

  std::string GetNextLine();

  bool GetNextChar(char &c);

  std::string GetThisLine();

 private:
  std::string InFile;
  std::string Code;
  std::queue<std::string> CodeQueue;
  std::queue<char> CodeLine;
  std::vector<std::string> codestrings;
  int ThisChar;
  int ThisLine;
};
}  // namespace compiler
#endif