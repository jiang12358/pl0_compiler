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
  void const_declare(int &dx, int level);
  void var_declare(int &dx, int level);
  void var_define(int &dx, int level);

  void procedure_declare(int &dx, int level);
  void function_declare(int &dx, int level);

  void parameter_handle(int level,int pos);

  int format_parameter(int &dx, int level, int procedure_flag);

  int one_parameter(int &dx, int level, int procedure_flag);

  void array_declare(int &dx, int level, std::vector<std::string> &tempStrings);

  void statement(int level);
  void multistatement(int level);
  void ifstatement(int level);
  void forstatement(int level);
  void repeatstatement(int level);
  void writestatement(int level);
  void readstatement(int level);
  void becomestatement(int level);

  void const_(int &dx, int level);

  void condition(int level);
  void expression(int level);
  void item(int level);
  void factor(int level);

  void load_var_address(int level);

  void enter(int &dx, int level, std::string name, SymType kind, int size);
  void enter(int &dx, int level, std::string name, SymType kind, int size, bool ifMinus);
  void enter(int &dx, int level, std::string name, SymType kind, int size, int value);
  void enter(int &dx, int level, std::string name, SymType kind, int size, double value);
  void enter(int &dx, int level, std::string name, SymType kind, int size, char value);

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
  double fun_value;
  std::map<int, int> level_last_adr;
  std::string output_file;
  std::vector<code> codes;

  bool DEBUG = false;

 public:
};
}  // namespace compiler

#endif
