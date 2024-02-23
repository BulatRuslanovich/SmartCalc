//
// Created by bulatruslanovich on 23/02/2024.
//

#include "mathModel.h"
#include <cmath>
#include <limits>
#include <list>
#include <stack>

#include "token.h"

namespace s21 {
MathModel::MathModel() : MathModel("") {}

MathModel::MathModel(const std::string& str) : needInit(true), isCalc(false), x(0.0), expression(str), rpnExpression(""), tokensList({}), mathResult(std::numeric_limits<double>::quiet_NaN()) {}

void MathModel::setExpression(const std::string& newExpression) {
  if (newExpression != expression) {
    expression = newExpression;
    needInit = true;
    isCalc = false;
  }
}
void MathModel::ExpressionAddString(const std::string& str) {
  std::string res = getExpression();

  if (res.length() > maxInputSize) {
    throw InputTooLongException("Input string too long");
  }

  try {
    if (str == "." ) {
      res = tokenHandle::AddPointToStr(res);
    } else {
      auto list = tokenHandle::ParseTokens(str);

      for (auto& item : list) {
        res = tokenHandle::AddTokenToStr(res, item);
      }
    }
  } catch (std::invalid_argument&) {
    throw InputIncorrectException("Incorrect input");
  } catch (std::logic_error&) {
    throw WrongOperatorException("Operator error");
  }

  if (res.length() > maxInputSize) {
    throw InputTooLongException("Input string too long");
  }

  setExpression(res);
}

void MathModel::ExpressionDeleteEnd() {
  std::string res = getExpression();

  try {
    res = tokenHandle::RemoveTokenFromStrEnd(res);
    // если хрень какую то написали,то удаляем сие творение посимвольно
  } catch (std::invalid_argument&) {
    if (!res.empty()) {
      res.erase(--res.end());
    }
  }

  setExpression(res);
}

std::string MathModel::getRpnExpression() {
  if (needInit == true) {
    InitCalcMachine();
  }

  return rpnExpression;
}


void MathModel::Calculate() {
  ResetModelResults();
  mathResult = CalcX(x);
}

void MathModel::InitMathModel() {
  CalcRpn();
  InitCalcMachine();
  needInit = false;
}
void MathModel::CalcRpn() {
  if (expression.empty()) {
    throw InputZeroLengthException("The input is empty");
  }

  if (expression.length() > maxInputSize) {
    throw InputIncorrectException("The input string is too long");
  }

  std::list<Token> tokenList;

  try {
    tokenList = tokenHandle::ParseTokens(expression);
  } catch (std::invalid_argument&) {
    throw InputIncorrectException("Invalid characters in input string")
  }

  if (tokenList.empty()) {
    throw InputIncorrectException("The input string must contain valid tokens");
  }

  std::stack<Token> tokenStack;
  std::string res;




}
void MathModel::InitCalcMachine() {}
void MathModel::ResetModelResults() noexcept {}
double MathModel::CalcX(double x) { return 0; }
double MathModel::CalcUnaryOp(double value, TokenType type) const noexcept {
  return 0;
}
double MathModel::CalcBinaryOp(double value1, double value2,
                               TokenType type) const noexcept {
  return 0;
}
}  // namespace s21