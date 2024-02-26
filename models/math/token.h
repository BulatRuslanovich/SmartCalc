//
// Created by bulatruslanovich on 23/02/2024.
//

#ifndef SMARTCALCV2_TOKEN_H
#define SMARTCALCV2_TOKEN_H

#include <string>

namespace s21 {

enum TokenType {
  unknownToken,   // Неизвестная лексема
  numberToken,    // число
  lBracketToken,  // (
  rBracketToken,  // )
  addToken,       // +
  subToken,       // -
  divToken,       // /
  mulToken,       // *
  powToken,       // ^
  modToken,       // mod или % (остаток от деления)
  unPlusToken,    // Унарный плюс
  unMinusToken,   // Унарный минус
  cosToken,       // cos
  sinToken,       // sin
  tanToken,       // tan
  acosToken,      // acos
  asinToken,      // asin
  atanToken,      // atan
  sqrtToken,      // sqrt
  lnToken,        // ln
  logToken,       // log
  xToken,         // x
  piToken,        // Математическая константа Пи
  eToken,         // Математическая константа Е
};

struct Token final {
 public:
  Token();
  Token(TokenType type, const std::string &str, int priority, bool left2right);
  Token(TokenType type, const std::string &str, int priority, bool left2right,
        double value);

  static Token MakeToken(TokenType type, double value = 0,
                         const std::string &string = "");

  bool IsUnary() const noexcept;
  bool IsBinary() const noexcept;
  bool IsConst() const noexcept;
  bool IsFunc() const noexcept;
  bool IsOperator() const noexcept;

  TokenType type;
  std::string str;
  int priority;
  bool left2right;
  double value;

 private:
  static constexpr double PI = 3.14159265358979323846264338327950288;
  static constexpr double E = 2.71828182845904523536028747135266250;
};

}  // namespace s21

#endif  // SMARTCALCV2_TOKEN_H
