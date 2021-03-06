#include "Lex.h"
#include <iostream>
#include "global.h"
namespace compiler {

void Lex::init(std::string filename) {
  scanner.init(filename);
  scanner.ScanFile();
  scanner.GetNextChar(c);
}

void Lex::Run() {
  scanner.GetNextChar(c);
  while (true) {
    getsym();
  }
}

std::string Lex::GetThisLine() {
	return scanner.GetThisLine();
}
SymType Lex::getsym() {
  while (c == ' ') {
    scanner.GetNextChar(c);
  }

  if (((c >= 'a') && (c <= 'z'))|| ((c >= 'A') && (c <= 'Z'))) {  // check if the reserved word
    std::string tempString = "";
    while (((c >= 'a') && (c <= 'z')) || ((c >= '0') && (c <= '9')) ||
           ((c >= 'A') && (c <= 'Z'))) {
      // if (tempString.size() < al) {
      tempString = tempString + c;
      //}
      scanner.GetNextChar(c);
    }

    if (WordSymbol.find(tempString) == WordSymbol.end()) {
      sym = kIdent;
      last_id = tempString;
    } else {
      sym = WordSymbol[tempString];
    }
    // std::cout << SymbolDict[sym] << "\t" << tempString << std::endl;
    /*if (al != tempString.size()) {
      for (int i = 0;i<al-tempString.size();i++) {
                    tempString = tempString + " ";
      }
    }*/
  } else if ((c >= '0') && (c <= '9')) {  // check if number
    int k = 0;
    int num = 0;
    while ((c >= '0') && (c <= '9')) {
      k++;
      if (k > NMAX) {
        std::cout << "error" << std::endl;
        exit(0);
      }
      num = 10 * num + (c - '0');
      scanner.GetNextChar(c);
    }

    if (c != '.') {
      last_num = num;
      sym = kNumber;
    } else {
      double_num = num;
      num = 0;
      double digit = 1;
      scanner.GetNextChar(c);
      while ((c >= '0') && (c <= '9')) {
        scanner.GetNextChar(c);
        digit *= 10;
        num = 10 * num + (c - '0');
      }
      double_num += num / digit;
      sym = kRealNum;
    }
    // std::cout << SymbolDict[sym] << "\t" << num << std::endl;
  } else if (c == ':') {
    scanner.GetNextChar(c);
    if (c == '=') {
      scanner.GetNextChar(c);
      sym = kBecome;
      // std::cout << SymbolDict[sym] << "\t"<< ":=" << std::endl;
    } else {
      sym = kColon;
      // std::cout << SymbolDict[sym] << "\t"<< ":" << std::endl;
    }
  } else if (c == '<') {
    scanner.GetNextChar(c);
    if (c == '=') {
      sym = kLessEqual;
      scanner.GetNextChar(c);
      // std::cout << SymbolDict[sym] << "\t"<< "<=" << std::endl;
    } else if (c == '>') {
      sym = kNotEqual;
      scanner.GetNextChar(c);
      // std::cout << SymbolDict[sym] << "\t"<< "<>" << std::endl;
    } else {
      sym = kLess;
      // std::cout << SymbolDict[sym] << "\t"<< "<" << std::endl;
    }
  } else if (c == '>') {
    scanner.GetNextChar(c);
    if (c == '=') {
      sym = kGreaterEqual;
      scanner.GetNextChar(c);
      // std::cout << SymbolDict[sym] << "\t"                << ">=" <<
      // std::endl;
    } else {
      sym = kGreater;
      // std::cout << SymbolDict[sym] << "\t"                << ">" <<
      // std::endl;
    }
  } else if (c == '\"') {
    last_string = "";
    //last_string = last_string + c;
    scanner.GetNextChar(c);
    while ((c == 32) || (c == 33) || ((c >= 35) && (c <= 126))) {
      last_string = last_string + c;
      scanner.GetNextChar(c);
    }
    if (c == '\"') {
      sym = kString;
      scanner.GetNextChar(c);
    }
  } else if (c == '\'') {
    scanner.GetNextChar(c);
    if (((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')) ||
        ((c >= '0') && (c <= '9'))) {
      char_value = c;
      scanner.GetNextChar(c);
      if (c == '\'') {
        sym = kCharacter;
        scanner.GetNextChar(c);
      }
    }
  } else {
    sym = CharSymbol[c];
    // std::cout << SymbolDict[sym] << "\t" << c << std::endl;
    scanner.GetNextChar(c);
  }
}
}  // namespace compiler
