#ifndef GRAMMAR
#define GRAMMAR
#include "Lex.h"
#include "global.h"

namespace compiler {
class Grammar {
 public:
  Grammar(){};

  void init(std::string filename);

  void program();

  void block(int level, bool iffunction);
  void const_declare(int level);
  void var_declare(int level);
  void var_define(int level);

  void procedure_declare(int level);
  void function_declare(int level);

  void parameter_handle(int level);

  int format_parameter(int level);

  int one_parameter(int level);

  void array_declare(int level, std::vector<std::string> &tempStrings);

  void statement(int level);
  void multistatement(int level);
  void ifstatement(int level);
  void forstatement(int level);
  void repeatstatement(int level);
  void writestatement(int level);
  void readstatement(int level);
  void becomestatement(int level);

  void const_(int level);

  void condition(int level);
  void expression(int level);
  void item(int level);
  void factor(int level);

  void enter(int level, std::string name, SymType kind, int size);
  void enter(int level, std::string name, SymType kind, int size, bool ifMinus);
  void enter(int level, std::string name, SymType kind, int size, int value);
  void enter(int level, std::string name, SymType kind, int size, double value);
  void enter(int level, std::string name, SymType kind, int size, char value);

  void clearTable(int level);

  void printTable();

  int position(std::string name, int level, int other);

  void emit(std::string op);
  void emit(std::string op, int y);
  void emit(std::string op, int x, int y);
  void emit(std::string op, int x, std::string write_string);

  void printCode();

  void runCodes();
  void runSingleCode();

  int base(int levelGap, int b);

  void error();

 private:
  Lex lex;
  std::vector<table> tables;
  int tab_size;
  int max_level;
  int last_address;
  int pc = 0;
  int t = 0;
  int b = 1;
  int dx = 0;
  std::map<int, int> level_last_adr;
  std::string output_file;
  std::vector<code> codes;

 public:
};
}  // namespace compiler

#endif
