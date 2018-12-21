#ifndef COMPILER_LEX
#define COMPILER_LEX
#include <iostream>
#include "Scanner.h"
#include "global.h"
namespace compiler {
class Lex {
 public:
  Lex(){};

  void init(std::string filename);

  SymType getsym();

  void Run();

  std::string GetThisLine();

  SymType sym;

  std::string last_id;

  int last_num;
  double double_num;
  char char_value;

  std::string last_string;
 private:
  Scanner scanner;
  char c;
};

}  // namespace compiler
#endif
