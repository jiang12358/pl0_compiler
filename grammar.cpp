
#include "grammar.h"
#include <iostream>
#include "Lex.h"

namespace compiler {
// extern void lex.getsym();

void Grammar::init(std::string filename) {
  lex.init(filename);
  tab_size = 0;
  last_address = 0;
  max_level = 0;
}

void Grammar::program() {
  block(0);
  if (lex.sym == kDot) {
    emit("OPR", 0, 0);
    std::cout << "This is a program" << std::endl;
  }
  printTable();
  printCode();
}

void Grammar::block(int level) {
  emit("JMP", 0, 0);
  int jmp_flag = codes.size() - 1;
  lex.getsym();
  if (lex.sym == kConst) {
    lex.getsym();
    const_declare(level);
  }

  while (lex.sym == kComma) {
    lex.getsym();
    if (lex.sym == kIdent) {
      const_declare(level);
    } else {
      break;
    }
  }

  if (lex.sym == kSemiColon) {
    lex.getsym();
  }

  if (lex.sym == kVar) {
    lex.getsym();
    var_declare(level);
  }

  while (lex.sym == kFunction || lex.sym == kProcedure) {
    if (lex.sym == kFunction) {
      lex.getsym();
      function_declare(level);
    }

    if (lex.sym == kProcedure) {
      lex.getsym();
      procedure_declare(level);
    }
  }

  if (lex.sym == kBegin) {
    codes[jmp_flag].y = codes.size();
    multistatement(level);
  }

  emit("OPR", 0, 0);
  std::cout << "this is a block" << std::endl;
}

void Grammar::const_declare(int level) {
  if (lex.sym == kIdent) {
    lex.getsym();
    if (lex.sym == kEqual) {
      lex.getsym();
      const_(level);
    }
  }
  std::cout << "This is a const declaration" << std::endl;
}

void Grammar::const_(int level) {
  if (lex.sym == kPlus || lex.sym == kMinus) {
    lex.getsym();
    if (lex.sym == kRealNum || lex.sym == kNumber) {
      lex.getsym();
      enter(level, lex.last_id, kConst, 1);
      std::cout << "This is a const" << std::endl;
    }
  } else if (lex.sym == kRealNum || lex.sym == kNumber) {
    lex.getsym();
    enter(level, lex.last_id, kConst, 1);
    std::cout << "This is a const" << std::endl;
  } else if (lex.sym == kCharacter) {
    lex.getsym();
    enter(level, lex.last_id, kConst, 1);
    std::cout << "This is a const" << std::endl;
  }
}

void Grammar::var_declare(int level) {
  var_define(level);
  while (lex.sym == kSemiColon) {
    lex.getsym();
    var_define(level);
  }
  std::cout << "This is a variable declaration" << std::endl;
}

void Grammar::var_define(int level) {
  std::vector<std::string> tempStrings;
  if (lex.sym == kIdent) {
    tempStrings.push_back(lex.last_id);
    lex.getsym();
    while (lex.sym == kComma) {
      lex.getsym();
      if (lex.sym == kIdent) {
        tempStrings.push_back(lex.last_id);
        lex.getsym();
      }
    }

    if (lex.sym == kColon) {
      lex.getsym();
      if (lex.sym == kInteger || lex.sym == kReal || lex.sym == kChar) {
        for (auto id : tempStrings) {
          enter(level, id, kVar, 1);
        }
        lex.getsym();
      } else if (lex.sym == kArray) {
        lex.getsym();
        array_declare(level, tempStrings);
      } else {
        error();
      }
    }
    std::cout << "This is a variable defination" << std::endl;
  }
}

void Grammar::array_declare(int level, std::vector<std::string> &tempStrings) {
  if (lex.sym == kLSquareBracket) {
    lex.getsym();
    if (lex.sym == kNumber) {
      lex.getsym();
      if (lex.sym == kRSquareBracket) {
        lex.getsym();
        if (lex.sym == kOf) {
          lex.getsym();
          if (lex.sym == kInteger || lex.sym == kReal || lex.sym == kChar) {
            for (auto id : tempStrings) {
              enter(level, id, kVar, lex.last_num);
            }
            lex.getsym();
            std::cout << "This is an array defination" << std::endl;
          }
        }
      }
    }
  }
}

void Grammar::function_declare(int level) {
  std::string function_name;
  if (lex.sym == kIdent) {
    clearTable(level + 1);
    function_name = lex.last_id;
    enter(level, function_name, kFunction, 0);

    lex.getsym();
    if (lex.sym == kLBracket) {
      lex.getsym();
      if (lex.sym != kRBracket) {
        format_parameter(level + 1);
      }
      if (lex.sym == kRBracket) {
        lex.getsym();
        if (lex.sym == kColon) {
          lex.getsym();
          if (lex.sym == kInteger || lex.sym == kChar || lex.sym == kReal) {
            lex.getsym();
            if (lex.sym == kSemiColon) {
              int i = position(function_name, 0, 0);
              tables[i].address = codes.size();
              block(level + 1);
              if (lex.sym == kSemiColon) {
                lex.getsym();
                std::cout << "This is a function declaration" << std::endl;
              }
            }
          }
        }
      }
    }
  }
}

void Grammar::procedure_declare(int level) {
  std::string procedure_name;
  if (lex.sym == kIdent) {
    clearTable(level + 1);
    procedure_name = lex.last_id;
    enter(level, procedure_name, kProcedure, 0);

    lex.getsym();
    if (lex.sym == kLBracket) {
      lex.getsym();
      if (lex.sym != kRBracket) {
        format_parameter(level + 1);
      }

      // std::cout << lex.sym << std::endl;
      if (lex.sym == kRBracket) {
        lex.getsym();
        if (lex.sym == kSemiColon) {
          int i = position(procedure_name, 0, 0);
          tables[i].address = codes.size();
          // std::cout << lex.sym << std::endl;
          block(level + 1);
          // std::cout << lex.sym << std::endl;
          if (lex.sym == kSemiColon) {
            lex.getsym();
            std::cout << "This is a procedure declaration" << std::endl;
          }
        }
      }
    }
  }
}

void Grammar::format_parameter(int level) {
  one_parameter(level);
  while (lex.sym == kSemiColon) {
    lex.getsym();
    one_parameter(level);
  }
  std::cout << "This is a format parameter" << std::endl;
}

void Grammar::one_parameter(int level) {
  if (lex.sym == kVar) {
    lex.getsym();
  }
  if (lex.sym == kIdent) {
    enter(level, lex.last_id, kVar, 1);
    lex.getsym();
    while (lex.sym == kComma) {
      lex.getsym();
      if (lex.sym == kIdent) {
        enter(level, lex.last_id, kVar, 1);
        lex.getsym();
      } else {
        error();
      }
    }
    if (lex.sym == kColon) {
      lex.getsym();
      if (lex.sym == kInteger || lex.sym == kReal || lex.sym == kChar) {
        lex.getsym();
        std::cout << "This is a parameter" << std::endl;
      }
    }
  }
}

void Grammar::statement(int level) {
  switch (lex.sym) {
    case kBegin: {
      multistatement(level);
      break;
    }
    case kIf: {
      ifstatement(level);
      break;
    }
    case kFor: {
      forstatement(level);
      break;
    }
    case kRepeat: {
      repeatstatement(level);
      break;
    }
    case kWrite: {
      writestatement(level);
      break;
    }
    case kRead: {
      readstatement(level);
      break;
    }
    case kIdent: {
      lex.getsym();

      if (lex.sym == kLBracket) {
        lex.getsym();
        if (lex.sym != kRBracket) {
          lex.getsym();
          parameter_handle(level);
        }
        if (lex.sym != kRBracket) {
          error();
        } else {
          lex.getsym();
          std::cout << "This is a procedure call" << std::endl;
        }
      } else {
        becomestatement(level);
      }
      break;
    }
    case kEnd:
      break;
    default:
      break;
  }
  // std::cout << lex.sym << std::endl;
  std::cout << "this is a statement" << std::endl;
}

void Grammar::multistatement(int level) {
  // std::cout << lex.sym << std::endl;
  lex.getsym();
  statement(level);
  while (lex.sym == kSemiColon) {
    lex.getsym();
    statement(level);
  }
  if (lex.sym != kEnd) {
    error();
  }
  lex.getsym();
  std::cout << "This is a multistatement" << std::endl;
}

void Grammar::ifstatement(int level) {
  lex.getsym();
  condition(level);
  emit("JRC", 0, 0);
  int codeflag = codes.size() - 1;
  if (lex.sym == kThen) {
    lex.getsym();
    statement(level);
    if (lex.sym == kElse) {
      codes[codeflag].y = codes.size();
      lex.getsym();
      statement(level);
    } else {
      codes[codeflag].y = codes.size();
    }
  }
  std::cout << "This is an if statement" << std::endl;
}

void Grammar::forstatement(int level) {
  int begin, over;
  lex.getsym();
  if (lex.sym == kIdent) {
    int i = position(lex.last_id, 0, 0);
    lex.getsym();
    if (lex.sym == kBecome) {
      lex.getsym();
    } else {
      error();
    }

    expression(level);
    emit("STO", level - tables[i].level, tables[i].address);
    if (lex.sym == kTo || lex.sym == kDownto) {
      lex.getsym();
      expression(level);

      if (lex.sym == kDo) {
        begin = codes.size();
        emit("F1U", 0, begin - 1);
        lex.getsym();
        statement(level);
        emit("F2U", 0, codes.size() - 1);
      } else {
        error();
      }
    } else {
      error();
    }
  }
  std::cout << "This is a for statement" << std::endl;
}

void Grammar::repeatstatement(int level) {
  int mark = codes.size();
  lex.getsym();
  statement(level);
  if (lex.sym == kUntil) {
    lex.getsym();
    condition(level);
    emit("JRC", 0, mark);
  } else {
    error();
  }
  std::cout << "This is a repeat statement" << std::endl;
}

void Grammar::writestatement(int level) {
  lex.getsym();
  if (lex.sym == kLBracket) {
    lex.getsym();
    if (lex.sym == kString) {
      lex.getsym();
      if (lex.sym == kComma) {
        lex.getsym();
        expression(level);
        emit("WRT", 0, 0);
      }
    } else {
      expression(level);
      emit("WRT", 0, 0);
    }
    if (lex.sym != kRBracket) {
      error();
    } else {
      lex.getsym();
    }
  }
  std::cout << "This is a writestatement" << std::endl;
}

void Grammar::readstatement(int level) {
  lex.getsym();
  if (lex.sym == kLBracket) {
    lex.getsym();
    if (lex.sym == kIdent) {
      emit("RED", level - tables[position(lex.last_id, 0, 0)].level,
           tables[position(lex.last_id, 0, 0)].address);
      lex.getsym();
      while (lex.sym == kComma) {
        lex.getsym();
        if (lex.sym != kIdent) {
          error();
        }
        emit("RED", level - tables[position(lex.last_id, 0, 0)].level,
             tables[position(lex.last_id, 0, 0)].address);
        lex.getsym();
      }

      if (lex.sym != kRBracket) {
        error();
      } else {
        lex.getsym();
      }
    } else {
      error();
    }
  }

  std::cout << "This is a readstatement" << std::endl;
}

void Grammar::becomestatement(int level) {
  int i = position(lex.last_id, level, 0);
  if (lex.sym == kLSquareBracket) {
    lex.getsym();
    expression(level);
    if (lex.sym != kRSquareBracket) {
      error();
    } else {
      lex.getsym();
    }
  }

  if (lex.sym == kBecome) {
    lex.getsym();
    expression(level);
    emit("STO", level - tables[i].level, tables[i].address);
  }

  std::cout << "This is a become statement" << std::endl;
}

void Grammar::expression(int level) {
  if (lex.sym == kPlus || lex.sym == kMinus) {
    bool minusflag = (lex.sym == kMinus);
    lex.getsym();
    item(level);
    if (minusflag) {
      emit("OPR", 0, 1);
    }
  } else {
    item(level);
  }

  while (lex.sym == kPlus || lex.sym == kMinus) {
    bool minusflag = (lex.sym == kMinus);
    lex.getsym();
    item(level);
    if (minusflag) {
      emit("OPR", 0, 3);
    } else {
      emit("OPR", 0, 2);
    }
  }
  std::cout << "This is an expression" << std::endl;
}

void Grammar::item(int level) {
  bool ifmulti;
  factor(level);
  while (lex.sym == kMulti || lex.sym == kDevide) {
    ifmulti = (lex.sym == kMulti);
    lex.getsym();
    factor(level);
    if (ifmulti) {
      emit("OPR", 0, 4);
    } else {
      emit("OPR", 0, 5);
    }
  }
  std::cout << "This is an item" << std::endl;
}

void Grammar::factor(int level) {
  if (lex.sym == kIdent) {
    int i = position(lex.last_id, 0, 0);
    lex.getsym();
    if (lex.sym == kLSquareBracket) {
      lex.getsym();
      expression(level);
      if (lex.sym != kRSquareBracket) {
        error();
      } else {
        lex.getsym();
      }
    } else if (lex.sym == kLBracket) {
      lex.getsym();
      if (lex.sym != kRBracket) {
        parameter_handle(level);
      }
      if (lex.sym != kRBracket) {
        error();
      } else {
        lex.getsym();
        std::cout << "This is a function call" << std::endl;
      }
    } else {
      if (tables[i].kind == kVar) {
        emit("LOD", level - tables[i].level, tables[i].address);
      } else {
        emit("LIT", 0, tables[i].num);
        // if (lex.last_num > 100) {
        std::cout << lex.last_num << std::endl;
        //}
      }
    }
  } else if (lex.sym == kNumber) {
    emit("LIT", 0, lex.last_num);
    // if (lex.last_num > 100) {
    std::cout << lex.last_num << std::endl;
    //}
    lex.getsym();
    std::cout << "This is a number" << std::endl;
  } else if (lex.sym == kReal) {
    emit("LIT", 0, lex.last_num);
    // if (lex.last_num > 100) {
    std::cout << lex.last_num << std::endl;
    //}
    lex.getsym();
    std::cout << "This is a real" << std::endl;
  } else if (lex.sym == kLBracket) {
    lex.getsym();
    expression(level);
    if (lex.sym != kRBracket) {
      error();
    } else {
      lex.getsym();
    }
  }
  std::cout << "This is a factor" << std::endl;
}

void Grammar::parameter_handle(int level) {
  if (lex.sym != kRBracket) {
    expression(level);
    while (lex.sym == kComma) {
      lex.getsym();
      expression(level);
    }
  }
  std::cout << "This is a parameter expression" << std::endl;
}

void Grammar::condition(int level) {
  SymType temp;
  expression(level);
  if (lex.sym == kEqual || lex.sym == kLess || lex.sym == kLessEqual ||
      lex.sym == kNotEqual || lex.sym == kGreater || lex.sym == kGreaterEqual) {
    temp = lex.sym;
    lex.getsym();
    expression(level);
    switch (temp) {
      case kEqual:
        emit("OPR", 0, 8);
        break;
      case kLess:
        emit("OPR", 0, 10);
        break;
      case kLessEqual:
        emit("OPR", 0, 11);
        break;
      case kNotEqual:
        emit("OPR", 0, 9);
        break;
      case kGreater:
        emit("OPR", 0, 12);
        break;
      case kGreaterEqual:
        emit("OPR", 0, 13);
    }
    std::cout << "This a condition" << std::endl;
  }
}

void Grammar::enter(int level, std::string name, SymType kind, int size) {
  table temp = table();
  temp.name = name;
  temp.level = level;
  temp.kind = kind;

  if (size == 1) {
    temp.ifArray = false;
    temp.address = level_last_adr[level] + 1;
    level_last_adr[level] = temp.address;
  } else if (size > 1) {
    temp.ifArray = true;
    temp.address = level_last_adr[level] + 1;
    level_last_adr[level] = temp.address;
  } else {
    temp.ifArray = false;
  }

  if (size > 0 && max_level < level) {
    max_level = level;
    temp.address = 0;
    level_last_adr[level] = temp.address;
  }

  tables.push_back(temp);
}

void Grammar::enter(int level, std::string name, SymType kind, int size,
                    int number) {
  table temp = table();
  temp.name = name;
  temp.level = level;
  temp.kind = kind;
  temp.ifArray = false;
  temp.num = number;

  tables.push_back(temp);
}

void Grammar::enter(int level, std::string name, SymType kind, int size,
                    double real) {
  table temp = table();
  temp.name = name;
  temp.level = level;
  temp.kind = kind;
  temp.ifArray = false;
  temp.real = real;

  tables.push_back(temp);
}

void Grammar::enter(int level, std::string name, SymType kind, int size,
                    char c) {
  table temp = table();
  temp.name = name;
  temp.level = level;
  temp.kind = kind;
  temp.ifArray = false;
  temp.c = c;

  tables.push_back(temp);
}

void Grammar::emit(std::string op) {
  code temp = code();
  temp.op = op;
  codes.push_back(temp);
}

void Grammar::emit(std::string op, int x) {
  code temp = code();
  temp.op = op;
  temp.x = x;
  codes.push_back(temp);
}

void Grammar::emit(std::string op, int x, int y) {
  code temp = code();
  temp.op = op;
  temp.x = x;
  temp.y = y;
  codes.push_back(temp);
}

int Grammar::position(std::string name, int level, int other) {
  // int max_flag = -1;
  int flag;
  for (int i = tables.size() - 1; i >= 0; i--) {
    if (name == tables[i].name) {
      flag = i;
      break;
    }
  }

  if (!tables[flag].ifArray) {
    return tables[flag].address;
  } else {
    return tables[flag].address + other;
  }
}

void Grammar::clearTable(int level) {
  for (int i = 0; i < tables.size(); i++) {
    if (level == tables[i].level) {
      tables.erase(tables.begin() + i, tables.end());
      break;
    }
  }
}

void Grammar::printTable() {
  std::cout << tables.size() << std::endl;
  for (auto table : tables) {
    std::cout << "name:" << table.name << "   kind:" << table.kind
              << "   level:" << table.level << "    addr:" << table.address
              << std::endl;
  }
}

void Grammar::printCode() {
  for (auto code : codes) {
    // if (code.op <= 3) {
    std::cout << code.op << " " << code.x << " " << code.y << std::endl;
    //}
    // else if (code.op <= 30) {
    //	std::cout << CodeSymbol[code.op] << " " << code.x << " " << code.y <<
    //std::endl;
    //}
    // else {
    //	std::cout << CodeSymbol[code.op] << std::endl;
    //}
  }
}
}  // namespace compiler

int main(int argc, char *argv[]) {
  compiler::Grammar grammar;
  grammar.init(std::string(argv[1]));
  grammar.program();
}