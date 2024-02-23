//
// Created by bulatruslanovich on 23/02/2024.
//

#include "token.h"
#include <limits>

namespace s21 {
Token::Token()
    : type(unknownToken), str(""), priority(0), left2right(true), value(0) {}

Token::Token(TokenType type, const std::string &str, int priority,
             bool left2right, double value)
    : type(type),
      str(str),
      priority(priority),
      left2right(left2right),
      value(value) {}

Token::Token(TokenType type, const std::string &str, int priority,
             bool left2right)
    : type(type),
      str(str),
      priority(priority),
      left2right(left2right),
      value(std::numeric_limits<double>::quiet_NaN()) {}

Token Token::MakeToken(TokenType type, double value,
                       const std::string &string) {
  switch (type) {
    case lBracketToken:
      return Token{lBracketToken, "(", 0, true};
    case rBracketToken:
      return Token{rBracketToken, ")", 0, true};
    case addToken:
      return Token{addToken, "+", 1, true};
    case subToken:
      return Token{subToken, "-", 1, true};
    case divToken:
      return Token{divToken, "/", 2, true};
    case mulToken:
      return Token{mulToken, "*", 2, true};
    case powToken:
      return Token{powToken, "^", 3, false};
    case unMinusToken:
      return Token{unMinusToken, "~", 3, false};
    case unPlusToken:
      return Token{unPlusToken, "+", 3, false};
    case numberToken:
      return Token{numberToken, string, 0, true, value};
    case modToken:
      return Token{modToken, "mod", 2, true};
    case cosToken:
      return Token{cosToken, "cos", 0, true};
    case sinToken:
      return Token{sinToken, "sin", 0, true};
    case tanToken:
      return Token{tanToken, "tan", 0, true};
    case acosToken:
      return Token{acosToken, "acos", 0, true};
    case asinToken:
      return Token{asinToken, "asin", 0, true};
    case atanToken:
      return Token{atanToken, "atan", 0, true};
    case sqrtToken:
      return Token{sqrtToken, "sqrt", 0, true};
    case lnToken:
      return Token{lnToken, "ln", 0, true};
    case logToken:
      return Token{logToken, "log", 0, true};
    case xToken:
      return Token{xToken, "x", 0, true};
    case piToken:
      return Token{piToken, "pi", 0, true, Token::PI};
    case eToken:
      return Token{eToken, "e", 0, true, Token::E};
    case unknownToken:
    default:
      return Token{unknownToken, "", 0, false};
  }
}

bool Token::IsConst() const noexcept {
  return type == numberToken || type == xToken || type == eToken ||
         type == piToken;
}

bool Token::IsFunc() const noexcept {
  return type == cosToken || type == sinToken || type == tanToken ||
         type == acosToken || type == asinToken || type == atanToken ||
         type == sqrtToken || type == lnToken || type == logToken;
}

bool Token::IsOperator() const noexcept {
  return type == addToken || type == subToken || type == divToken ||
         type == mulToken || type == powToken || type == unMinusToken ||
         type == unPlusToken || type == modToken;
}

bool Token::IsUnary() const noexcept {
  return type == unMinusToken || type == unPlusToken || IsFunc();
}

bool Token::IsBinary() const noexcept {
  return type == addToken || type == subToken || type == divToken ||
         type == mulToken || type == powToken || type == modToken;
}

}