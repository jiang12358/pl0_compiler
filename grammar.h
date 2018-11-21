#ifndef GRAMMAR
#define GRAMMAR
#include "global.h"

namespace compiler {
class Grammar {
 public:
  Grammar(){};

  void Init(std::string filename);

  void program();

  void block();
  void const_declare();
  void var_declare();
  void var_define();

  void procedure_declare();
  void procedure_define();
  void function_declare();
  void function_define();

  void parameter_handle();

  void format_parameter();

  void one_parameter();

  void array_declare();

  void statement();
  void multistatement();
  void ifstatement();
  void forstatement();
  void repeatstatement();
  void writestatement();
  void readstatement();
  void becomestatement();

  void parameter();

  void const_();

  void condition();
  void expression();
  void item();
  void factor();
};
}  // namespace compiler

#endif