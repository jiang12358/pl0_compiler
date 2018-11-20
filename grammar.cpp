#include "grammar.h"
#include <iostream>

namespace compiler {
extern void getsym();
extern void error();

void Grammar::block() {
  getsym();
  if (sym == kConst) {
    getsym();
    const_declare();
  }

  while (sym == kSemiColon) {
    getsym();
    if (sym == kIdent) {
      const_declare();
    } else {
      break;
    }
  }

  if (sym == kVar) {
    getsym();
    var_declare();
  }

  while (sym == kFunction || sym == kProcedure) {
    if (sym == kFunction) {
      function_declare();
    }

    if (sym == kProcedure) {
      procedure_declare();
    }
  }

  if (sym == kBegin) {
    getsym();
    statement();
  }

  std::cout << "this is a block" << std::endl;
}

void Grammar::const_declare() {
  if (sym == kIdent) {
    getsym();
    if (sym == kEqual) {
      getsym();
      const_();
    }
  }
  std::cout << "This is a const declaration" << std::endl;
}

void Grammar::const_() {
  if (sym == kPlus || sym == kMinus) {
    getsym();
    if (sym == kReal || sym == kNumber) {
      getsym();
      std::cout << "This is a const";
    }
  } else if (sym == kReal || sym == kNumber) {
    getsym();
    std::cout << "This is a const";
  } else if (sym == kCharacter) {
    getsym();
    std::cout << "This is a const";
  }
}

void Grammar::var_declare() {
  var_define();
  while (sym == kSemiColon) {
    getsym();
    var_define();
  }
  std::cout << "This is a variable declaration" << std::endl;
}

void Grammar::var_define() {
  if (sym == kIdent) {
    getsym();
    while (sym == kComma) {
      getsym();
      if (sym == kIdent) {
        getsym();
      }
    }

    if (sym == kColon) {
      getsym();
      if (sym == kInteger || sym == kReal || sym == kChar) {
        getsym();
      } else if (sym == kArray) {
        getsym();
        array_declare();
      } else {
        error();
      }
    }
  }
  std::cout << "This is a variable defination" << std::endl;
}

void Grammar::array_declare() {
  if (sym == kLSquareBracket) {
    getsym();
    if (sym == kNumber) {
    }
  }
}

void Grammar::function_declare() {
  std::cout << "This is a function declaration" << std::endl;
}

void Grammar::procedure_declare() {
  std::cout << "This is a procedure declaration" << std::endl;
}

void Grammar::statement() {
  switch (sym) {
  case kBegin: {
    getsym();
    multistatement();
    break;
  }
  case kIf: {
    getsym();
    ifstatement();
    break;
  }
  case kFor: {
    getsym();
    forstatement();
    break;
  }
  case kRepeat: {
    getsym();
    repeatstatement();
    break;
  }
  case kWrite: {
    getsym();
    writestatement();
    break;
  }
  case kRead: {
    getsym();
    readstatement();
    break;
  }
  case kIdent: {
    getsym();
    if (sym == kLBracket) {
      getsym();
      if (sym != kRBracket) {
        getsym();
        parameter_handle();
      }
      if (sym != kRBracket) {
        error();
      } else {
        getsym();
        std::cout << "This is a procedure call" << std::endl;
      }
    } else {
      becomestatement();
    }
    break;
  }
  }
  std::cout << "this is a statement" << std::endl;
}

void Grammar::multistatement() {
  if (sym == kBegin) {
    getsym();
    statement();
    while (sym == kSemiColon) {
      getsym();
      statement();
    }
    if (sym != kEnd) {
      error();
    }
  } else {
    error();
  }
  std::cout << "This is a multistatement" << std::endl;
}

void Grammar::ifstatement() {
  getsym();
  condition();
  if (sym == kThen) {
    getsym();
    statement();
    if (sym == kElse) {
      getsym();
      statement();
    }
  }
  std::cout << "This is an if statement" << std::endl;
}

void Grammar::forstatement() {
  getsym();
  if (sym == kIdent) {
    getsym();
    expression();
    if (sym == kTo || sym == kDownto) {
      getsym();
      expression();
      if (sym == kDo) {
        getsym();
        statement();
      } else {
        error();
      }
    } else {
      error();
    }
  }
  std::cout << "This is a for statement" << std::endl;
}

void Grammar::repeatstatement() {
  getsym();
  statement();
  if (sym == kUntil) {
    getsym();
    condition();
  } else {
    error();
  }
  std::cout << "This is a repeat statement" << std::endl;
}

void Grammar::writestatement() {
  getsym();
  if (sym == kLBracket) {
    getsym();
    if (sym == kString) {
      getsym();
      if (sym == kComma) {
        getsym();
        expression();
      } else {
        getsym();
      }
    } else {
      expression();
    }
    if (sym != kRBracket) {
      error();
    } else {
      getsym();
    }
  }
  std::cout << "This is a writestatement" << std::endl;
}

void Grammar::readstatement() {
  getsym();
  if (sym == kLBracket) {
    getsym();
    if (sym == kIdent) {
      getsym();
      while (sym == kComma) {
        getsym();
        if (sym != kIdent) {
          error();
        }
        getsym();
      }

      if (sym != kRBracket) {
        error();
      } else {
        getsym();
      }
    } else {
      error();
    }
  }
  std::cout << "This is a readstatement" << std::endl;
}

void Grammar::becomestatement() {
  if (sym == kIdent) {
    getsym();

    if (sym == kLSquareBracket) {
      getsym();
      expression();
      if (sym != kRSquareBracket) {
        error();
      } else {
        getsym();
      }
    }

    if (sym == kBecome) {
      getsym();
      expression();
    }
  }
  std::cout << "This is a become statement" << std::endl;
}

void Grammar::expression() {
  if (sym == kPlus || sym == kMinus) {
    getsym();
  }
  item();
  while (sym == kPlus || sym == kMinus) {
    getsym();
    item();
  }
  std::cout << "This is an expression" << std::endl;
}

void Grammar::item() {
  factor();
  while (sym == kMulti || sym == kDevide) {
    getsym();
    factor();
  }
  std::cout << "This is an item" << std::endl;
}

void Grammar::factor() {
  if (sym == kIdent) {
    getsym();
    if (sym == kLSquareBracket) {
      getsym();
      expression();
      if (sym != kRSquareBracket) {
        error();
      } else {
        getsym();
      }
    } else if (sym == kLBracket) {
      getsym();
      if (sym != kRBracket) {
        parameter_handle();
      }
      if (sym != kRBracket) {
        error();
      } else {
        getsym();
        std::cout << "This is a function call" << std::endl;
      }
    }
  } else if (sym == kNumber) {
    getsym();
    std::cout << "This is a number" << std::endl;
  } else if (sym == kReal) {
    getsym();
    std::cout << "This is a real" << std::endl;
  } else if (sym == kLBracket) {
    getsym();
    expression();
    if (sym != kRBracket) {
      error();
    } else {
      getsym();
    }
  }
  std::cout << "This is a factor" << std::endl;
}

void Grammar::parameter_handle() {
  if (sym != kRBracket) {
    expression();
    while (sym == kComma) {
      getsym();
      expression();
    }
  }

  std::cout << "This is a parameter expression"
}
} // namespace compiler