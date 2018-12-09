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

  void block(int level);
  void const_declare(int level);
  void var_declare(int level);
  void var_define(int level);

  void procedure_declare(int level);
  void function_declare(int level);

  void parameter_handle(int level);

  void format_parameter(int level);

  void one_parameter(int level);

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
  void enter(int level, std::string name, SymType kind, int size, int value);
  void enter(int level, std::string name, SymType kind, int size, double value);
  void enter(int level, std::string name, SymType kind, int size, char value);

  void clearTable(int level);

  void printTable();

  int position(std::string name, int level, int other);

  void emit(std::string op);
  void emit(std::string op, int y);
  void emit(std::string op, int x, int y);

  void printCode();

 private:
  Lex lex;
  std::vector<table> tables;
  int tab_size;
  int max_level;
  int last_address;
  std::map<int, int> level_last_adr;
  std::string output_file;
  std::vector<code> codes;

 public:
};
}  // namespace compiler

#endif