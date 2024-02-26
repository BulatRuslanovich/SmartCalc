//
// Created by bulatruslanovich on 23/02/2024.
//

#ifndef SMARTCALCV2_TOKENHANDLE_H
#define SMARTCALCV2_TOKENHANDLE_H
#include <list>
#include <map>
#include <string>
#include <string_view>

#include "token.h"

namespace s21 {

class tokenHandle final {
 public:
  tokenHandle() = delete;
  ~tokenHandle() = delete;

  static std::list<Token> ParseTokens(const std::string& str);
  static void FixTokenList(std::list<Token>& list);
  static void FixUnOperationTokensList(std::list<Token>& list);
  static std::string AddTokenToStr(const std::string& str, const Token& token);
  static std::string AddPointToStr(const std::string& str);
  static std::string RemoveTokenFromStrEnd(const std::string& str);
  static std::string ListToStr(const std::list<Token>& list);

 private:
  static std::pair<Token, std::string_view> GetNoNumTokenFromStr(
      const std::string_view& str);
  static std::pair<Token, std::string_view> GetNumTokenFromStr(
      const std::string_view& str);
  static bool IsLBracketExist(const std::list<Token>& list);
  static void AddUnMinusToList(std::list<Token>& list);
};

class TokenTypeMapper final {
 public:
  static const std::map<std::string, TokenType>& getTypeMap() {
    static const std::map<std::string, TokenType> types_map{
        {"(", lBracketToken}, {")", rBracketToken}, {"+", addToken},
        {"-", subToken},      {"/", divToken},      {"*", mulToken},
        {"^", powToken},      {"~", unMinusToken},  {"%", modToken},
        {"x", xToken},        {"e", eToken},        {"mod", modToken},
        {"cos", cosToken},    {"sin", sinToken},    {"tan", tanToken},
        {"acos", acosToken},  {"asin", asinToken},  {"atan", atanToken},
        {"sqrt", sqrtToken},  {"ln", lnToken},      {"log", logToken},
        {"pi", piToken}};
    return types_map;
  }
};

}  // namespace s21

#endif  // SMARTCALCV2_TOKENHANDLE_H
