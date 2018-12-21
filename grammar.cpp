
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
  block(0, false);
  if (lex.sym == kDot) {
    emit("OPR", 0, 0);
    std::cout << "This is a program" << std::endl;
  }
  printTable();
  printCode();
}

void Grammar::block(int level, bool iffunction) {
  dx = 3;
  int tx0 = tables.size() - 1;
  int cx0 = codes.size() - 1;
  
 
  int jmp_flag = codes.size();

  int procedure_flag = tables.size() - 1;
  int params_num = 0;
  if (lex.sym == kLBracket) {
	  lex.getsym();
	  if (lex.sym != kRBracket) {
		  params_num = format_parameter(level);
	  }
	  tables[procedure_flag].params_num = params_num;
	  // std::cout << lex.sym << std::endl;
	  if (lex.sym == kRBracket) {
		  lex.getsym();
		  if (iffunction) {
			  if (lex.sym == kColon) {
				  lex.getsym();
				  if (lex.sym == kInteger || lex.sym == kChar || lex.sym == kReal) {
					  lex.getsym();
				  }
			  }
		  }
		  if (lex.sym == kSemiColon) {
			  
			  
			  tables[procedure_flag].address = codes.size();
			  for (int i = params_num; i > 0; i--) {
				  if (tables[tables.size() - i].kind == kVarParam) {
					  //load address

				  }
				  else {
					  //load value
					  emit("STO", 0, tables[tables.size() - i].address);
				  }
			  }
			  jmp_flag = codes.size();
			  emit("JMP", 0, 0);
			  // std::cout << lex.sym << std::endl;
			  // std::cout << lex.sym << std::endl;
		  }
	  }
  }
  else {
	  jmp_flag = codes.size();
	  emit("JMP", 0, 0);
  }

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
    } else {
      lex.getsym();
      procedure_declare(level);
    }
	if (lex.sym == kSemiColon) {
		lex.getsym();
	}
  }

  if (lex.sym == kBegin) {
    
    codes[jmp_flag].y = codes.size();
	emit("INC", 0, dx);
	
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
    bool ifMinus = lex.sym == kMinus;
    lex.getsym();
    if (lex.sym == kRealNum || lex.sym == kNumber) {
      SymType tempSym = lex.sym;
      lex.getsym();
      enter(level, lex.last_id, tempSym, 1, ifMinus);
      std::cout << "This is a const" << std::endl;
    }
  } else if (lex.sym == kRealNum || lex.sym == kNumber) {
    SymType tempSym = lex.sym;
    lex.getsym();
    enter(level, lex.last_id, tempSym, 1);
    std::cout << "This is a const" << std::endl;
  } else if (lex.sym == kCharacter) {
    SymType tempSym = lex.sym;
    lex.getsym();
    enter(level, lex.last_id, tempSym, 1);
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

	int params_num = 0;
	int function_flag = tables.size() - 1;
	tables[function_flag].address = codes.size();


    lex.getsym();
    /*if (lex.sym == kLBracket) {
      lex.getsym();
      if (lex.sym != kRBracket) {
        params_num = format_parameter(level + 1);
      }
	  tables[function_flag].params_num = params_num;
      if (lex.sym == kRBracket) {
        lex.getsym();
        if (lex.sym == kColon) {
          lex.getsym();
          if (lex.sym == kInteger || lex.sym == kChar || lex.sym == kReal) {
            lex.getsym();
            if (lex.sym == kSemiColon) {
              int i = position(function_name, 0, 0);
              tables[i].address = codes.size();*/
              block(level + 1, true);
			  std::cout << "This is a function declaration" << std::endl;
              /*if (lex.sym == kSemiColon) {
                lex.getsym();
                std::cout << "This is a function declaration" << std::endl;
              }
            }
          }
        }
      }
    }*/
  }
}

void Grammar::procedure_declare(int level) {
  std::string procedure_name;
  if (lex.sym == kIdent) {
    clearTable(level + 1);
    procedure_name = lex.last_id;
    enter(level, procedure_name, kProcedure, 0);

	int params_num = 0;
	int procedure_flag = tables.size() - 1;
	tables[procedure_flag].address = codes.size();

    lex.getsym();
    if (lex.sym == kLBracket) {

          block(level + 1, false);
          // std::cout << lex.sym << std::endl;
          if (lex.sym == kSemiColon) {
            lex.getsym();
            std::cout << "This is a procedure declaration" << std::endl;
          }
        
      
    }
  }
}

int Grammar::format_parameter(int level) {
	int params_num = 0;
	params_num+=one_parameter(level);
  while (lex.sym == kSemiColon) {
    lex.getsym();
	params_num += one_parameter(level);
  }
  std::cout << "This is a format parameter" << std::endl;
  return params_num;
}

int Grammar::one_parameter(int level) {
	int params_num = 0;
	bool ifvar = false;
	std::vector<std::string> temp_strings;
  if (lex.sym == kVar) {
    lex.getsym();
	ifvar = true;
  }
  if (lex.sym == kIdent) {
	  params_num++;
	  if (ifvar) {
	     enter(level, lex.last_id, kVarParam, 1);
	  }
	  else {
		  temp_strings.emplace_back(lex.last_id);
	  }
    
    lex.getsym();
    while (lex.sym == kComma) {
      lex.getsym();
      if (lex.sym == kIdent) {
		  params_num++;
		  if (ifvar) {
			  enter(level, lex.last_id, kVarParam, 1);
		  }
		  else {
			  temp_strings.emplace_back(lex.last_id);
		  }
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
		if (!ifvar) {
			for (auto temp_string : temp_strings) {
				enter(level, temp_string, kVar, 1);
			}
		}
      }
    }
  }
  return params_num;
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
		std::string procedure_name = lex.last_id;
		int i = position(procedure_name, level, 0);
      lex.getsym();

      if (lex.sym == kLBracket) {
        lex.getsym();
        if (lex.sym != kRBracket) {
          parameter_handle(level);
		  emit("JMP", level - tables[i].level, tables[i].address);
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
   std::cout << lex.sym << std::endl;
  std::cout << "this is a statement" << std::endl;
  std::cout << lex.GetThisLine() << std::endl;
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
  emit("JEZ", 0, 0);
  int codeflag = codes.size() - 1;
  if (lex.sym == kThen) {
    lex.getsym();
    statement(level);
    if (lex.sym == kElse) {
		emit("JMP", 0, 0);
		int jmp_flag = codes.size() - 1;
      codes[codeflag].y = codes.size();
      lex.getsym();
      statement(level);
	  codes[jmp_flag].y = codes.size();
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
int for_start_mark = codes.size();
	emit("LOD", level - tables[i].level, tables[i].address);
    if (lex.sym == kTo ) {
      lex.getsym();
      
      expression(level);
	  
	  emit("OPR", 0, 11);
	  emit("JEZ", 0, 0);
	  int for_end_flag = codes.size() - 1;
      if (lex.sym == kDo) {
        begin = codes.size();
        lex.getsym();
        statement(level);
		emit("LOD", level - tables[i].level, tables[i].address);
		emit("LIT", 0, 1);
		emit("OPR", 0, 2);
		emit("STO", level - tables[i].level, tables[i].address);
		emit("JMP", 0, for_start_mark);
		codes[for_end_flag].y = codes.size();
      } else {
        error();
      }
    } else if (lex.sym == kDownto){
		lex.getsym();
		expression(level);
		emit("OPR", 0, 13);
		emit("JEZ", 0, 0);
		int for_end_flag = codes.size() - 1;
		if (lex.sym == kDo) {
			begin = codes.size();
			lex.getsym();
			statement(level);
			emit("LOD", level - tables[i].level, tables[i].address);
			emit("LIT", 0, 1);
			emit("OPR", 0, 3);
			emit("STO", level - tables[i].level, tables[i].address);
			emit("JMP", 0, for_start_mark);
			codes[for_end_flag].y = codes.size();
		}
		else {
			error();
		}
	}
	else {
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
    emit("JEZ", 0, mark);
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
      saved_string.emplace_back(lex.last_string);
      emit("WRT", 0, saved_string.size() - 1);
      lex.getsym();
      if (lex.sym == kComma) {
        lex.getsym();
        expression(level);
        emit("WRT", 0, -1);
      }
    } else {
      expression(level);
      emit("WRT", 0, -1);
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
      emit("RED", 0, 0);
      emit("STO", level - tables[position(lex.last_id, 0, 0)].level,
           tables[position(lex.last_id, 0, 0)].address);
      lex.getsym();
      while (lex.sym == kComma) {
        lex.getsym();
        if (lex.sym != kIdent) {
          error();
        }
        emit("RED", 0, 0);
        emit("STO", level - tables[position(lex.last_id, 0, 0)].level,
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
	std::cout << lex.sym << std::endl;
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
		std::cout << lex.sym << std::endl;
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
  } else if (lex.sym == kRealNum) {
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

  temp.kind = kind;

  if (size == 1) {
    temp.ifArray = false;
    if (kind == kVar) {
      temp.address = dx;
      dx++;
      temp.level = level;
	}
	else if (kind == kVarParam) {
		
	}
	else if (kind == kChar) {
      temp.c = lex.char_value;
    } else if (kind == kNumber) {
      temp.num = lex.last_num;
    } else if (kind == kRealNum) {
      temp.real = lex.double_num;
    } else if (kind == kFunction) {
      temp.level = level;
    } else if (kind == kProcedure) {
      temp.level = level;
    }
  } else if (size > 1) {
    temp.ifArray = true;
    if (kind == kVar) {
      temp.address = dx;
      dx += size;
      temp.level = level;
    }
  } else {
    temp.ifArray = false;
  }

  tables.push_back(temp);
}

void Grammar::enter(int level, std::string name, SymType kind, int size,
                    bool ifMinus) {
  table temp = table();
  temp.name = name;

  temp.kind = kind;

  if (size == 1) {
    temp.ifArray = false;
    if (kind == kNumber) {
      temp.num = -lex.last_num;
    } else if (kind == kRealNum) {
      temp.real = -lex.double_num;
    } else {
      error();
    }
  } else {
    error();
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
      return i;
    }
  }
/*
  if (!tables[flag].ifArray) {
    return tables[flag].address;
  } else {
    return tables[flag].address + other;
  }
*/
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
	int p = 0;
  for (auto code : codes) {
    // if (code.op <= 3) {
    std::cout << p<<"  "<<code.op << " " << code.x << " " << code.y << std::endl;
	p++;
    //}
    // else if (code.op <= 30) {
    //	std::cout << CodeSymbol[code.op] << " " << code.x << " " << code.y <<
    // std::endl;
    //}
    // else {
    //	std::cout << CodeSymbol[code.op] << std::endl;
    //}
  }
}

void Grammar::runCodes() {
  pc = 0;
  t = 0;
  b = 1;
  runningStack[0] = 0;
  runningStack[1] = 0;
  runningStack[2] = 0;
  runningStack[3] = 0;
  do {
    runSingleCode();
  } while (pc != 0);
}

void Grammar::runSingleCode() {
  code thisCode = codes[pc];
  pc++;
  if (thisCode.op == "OPR") {
    if (thisCode.y == 0) {
      t = b - 1;
      pc = runningStack[t + 3];
      b = runningStack[t + 2];
    } else if (thisCode.y == 1) {
      runningStack[t] = -runningStack[t];
    } else if (thisCode.y == 2) {
      t--;
      runningStack[t] = runningStack[t] + runningStack[t + 1];

    } else if (thisCode.y == 3) {
      t--;
      runningStack[t] = runningStack[t] - runningStack[t + 1];
    } else if (thisCode.y == 4) {
      t--;
      runningStack[t] = runningStack[t] * runningStack[t + 1];

    } else if (thisCode.y == 5) {
      t--;
      runningStack[t] = runningStack[t] / runningStack[t + 1];
    } else if (thisCode.y == 6) {
    } else if (thisCode.y == 7) {
    } else if (thisCode.y == 8) {
      t--;
      runningStack[t] = (runningStack[t] == runningStack[t + 1]);
    } else if (thisCode.y == 9) {
      t--;
      runningStack[t] = (runningStack[t] != runningStack[t + 1]);
    } else if (thisCode.y == 10) {
      t--;
      runningStack[t] = (runningStack[t] < runningStack[t + 1]);
    } else if (thisCode.y == 11) {
      t--;
      runningStack[t] = (runningStack[t] <= runningStack[t + 1]);
    } else if (thisCode.y == 12) {
      t--;
      runningStack[t] = (runningStack[t] > runningStack[t + 1]);
    } else if (thisCode.y == 13) {
      t--;
      runningStack[t] = (runningStack[t] >= runningStack[t + 1]);
    } else {
      error();
    }
  } else if (thisCode.op == "JMP") {
    pc = thisCode.y;
  } else if (thisCode.op == "JEZ") {
	  if (runningStack[t] == 0) {
		  pc = thisCode.y;
	}
	  t--;
  } else if (thisCode.op == "LOD") {
    t++;
    runningStack[t] = runningStack[base(thisCode.x, b) + thisCode.y];
  } else if (thisCode.op == "LIT") {
    t++;
    runningStack[t] = thisCode.y;
  } else if (thisCode.op == "RED") {
    t++;
    int a;
    scanf_s("%d", &a);
    runningStack[t] = a;
  } else if (thisCode.op == "STO") {
    runningStack[base(thisCode.x, b) + thisCode.y] = runningStack[t];
    t--;
  } else if (thisCode.op == "WRT") {
    if (thisCode.y == -1) {
      
      std::cout << runningStack[t]<<std::endl;
      t--;
    } else {
      std::cout << saved_string[thisCode.y]<<std::endl;
    }
  } else if (thisCode.op == "F1U") {
  } else if (thisCode.op == "F2U") {
  } else if (thisCode.op == "INC") {
    t = t + thisCode.y;
  }
}

int Grammar::base(int levelGap, int b) {
  int b1 = b;
  while (levelGap >0) {
    b1 = runningStack[b1];
    levelGap --;
  }
  return b1;
}

void Grammar::error() {
	std::cout << "ERROR" << std::endl;
	std::cout << lex.GetThisLine() << std::endl;
	std::cout << lex.sym << std::endl;
	std::exit(0);

}
}  // namespace compiler

int main(int argc, char *argv[]) {
  compiler::Grammar grammar;
  grammar.init(std::string(argv[1]));
  grammar.program();
  grammar.runCodes();
}
