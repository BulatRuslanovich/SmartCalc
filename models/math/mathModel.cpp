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
    throw InputIncorrectException("Invalid characters in input string");
  }

  if (tokenList.empty()) {
    throw InputIncorrectException("The input string must contain valid tokens");
  }

  std::stack<Token> tokenStack;
  std::string res;
  tokenHandle::FixTokenList(tokenList);

  auto tokenBegin = tokenList.begin();
  auto tokenEnd = tokenList.end();

  for (; tokenBegin != tokenEnd; ++tokenBegin) {
    Token check = *tokenBegin;

    // +1... плюс в начале игнорируем
    if (check.type == unPlusToken) {
      continue;
    } else if (check.IsConst()) {
      res.append(check.str);
      res.append(" ");
    } else if (check.type == lBracketToken || check.IsFunc()) {
      tokenStack.push(check);
    } else if (check.type == rBracketToken) {
      while (true) {
        if (tokenStack.empty()) {
          throw InputIncorrectException("Left brackets not matched");
        }

        Token lastToken = tokenStack.top();

        if (lastToken.type == lBracketToken) {
          tokenStack.pop();
          break;
        } else {
          AppendStrWithPopStack(res, tokenStack, lastToken.str);
        }
      }

      if (!tokenStack.empty()) {
        Token lastToken = tokenStack.top();

        if (lastToken.IsFunc()) {
          AppendStrWithPopStack(res, tokenStack, lastToken.str);
        }
      }
    } else if (check.IsOperator()) {
      while (true) {
        if (tokenStack.empty()) {
          break;
        }

        Token lastToken = tokenStack.top();

        if ((lastToken.left2right && lastToken.priority >= check.priority) || (!lastToken.left2right && lastToken.priority > check.priority)) {
          AppendStrWithPopStack(res, tokenStack, lastToken.str);
        } else {
          break;
        }
      }
    } else {
      throw InputIncorrectException("Unknown token");
    }
  }

  while (!tokenStack.empty()) {
    Token lastToken = tokenStack.top();

    if (lastToken.type == lBracketToken || lastToken.type == rBracketToken) {
      throw InputIncorrectException("Brackets are not matched");
    }

    AppendStrWithPopStack(res, tokenStack, lastToken.str);
  }

  if (!res.empty() && *(--res.end()) == ' ') {
    res.erase(--res.end());
  }

  rpnExpression = res;
}

void MathModel::AppendStrWithPopStack(std::string& res, std::stack<Token>& stack, std::string& str) { // TODO: может поломаться
  res.append(str);
  res.append(" ");
  stack.pop();
}

void MathModel::InitCalcMachine() {
  try {
    tokensList = tokenHandle::ParseTokens(rpnExpression);
  } catch (std::invalid_argument&) {
    throw InputIncorrectException("Unknown chars in input string");
  }
}

void MathModel::ResetModelResults() noexcept {
  mathResult = std::numeric_limits<double>::quiet_NaN();
}

double MathModel::CalcX(double x) {
  if (needInit) {
    InitCalcMachine();
  }

  double res = 0.0;
  std::stack<Token> tokenStack;
  auto tokenBegin = tokensList.begin();
  auto tokenEnd = tokensList.end();

  for (; tokenBegin != tokenEnd; ++tokenBegin) {
    Token check = *tokenBegin;

    if (check.type == numberToken) {
      tokenStack.push(check);
    } else if (check.type == xToken) {
      tokenStack.push(Token::MakeToken(numberToken, x));
    } else if (check.IsConst()) {
      tokenStack.push(Token::MakeToken(numberToken, check.value));
    } else if (check.IsUnary()) {
      if (!tokenStack.empty()) {
        Token lastToken = tokenStack.top();
        tokenStack.pop();
        double _res = CalcUnaryOp(lastToken.value, check.type);
        tokenStack.push(Token::MakeToken(numberToken, _res));
      } else {
        throw InputIncorrectException("Calculate error");
      }
    } else if (check.IsBinary()) {
      if (!tokenStack.empty()) {
        Token right = tokenStack.top();
        tokenStack.pop();
        if (!tokenStack.empty()) {
          Token left = tokenStack.top();
          tokenStack.pop();
          double _res = CalcBinaryOp(left.value, right.value, check.type);
          tokenStack.push(Token::MakeToken(numberToken, _res));
        } else {
          throw InputIncorrectException("Calculate error");
        }
      } else {
        throw InputIncorrectException("Calculate error");
      }
    } else {
      throw InputIncorrectException("Calculate error");
    }
  }

  if (tokenStack.empty()) {
    throw InputIncorrectException("Calculate error");
  }

  Token lastToken = tokenStack.top();
  tokenStack.pop();

  if (tokenStack.empty()) {
    throw InputIncorrectException("Calculate error");
  } else {
    res = lastToken.value;
  }

  isCalc = true;
  return res;
}
double MathModel::CalcUnaryOp(double value, TokenType type) const noexcept {
  double result = 0.0;
  
  switch (type) {
    case unMinusToken:
      result = 0 - value;
      break;
    case cosToken:
      result = std::cos(value);
      break;
    case sinToken:
      result = std::sin(value);
      break;
    case tanToken:
      result = std::tan(value);
      break;
    case acosToken:
      result = std::acos(value);
      break;
    case asinToken:
      result = std::asin(value);
      break;
    case atanToken:
      result = std::atan(value);
      break;
    case sqrtToken:
      result = std::sqrt(value);
      break;
    case lnToken:
      result = std::log(value);
      break;
    case logToken:
      result = std::log10(value);
      break;
    default:
      break;
  }

  return result;
}
double MathModel::CalcBinaryOp(double value1, double value2,
                               TokenType type) const noexcept {
  double result = 0.0;
  
  switch (type) {
    case addToken:
      result = value1 + value2;
      break;
    case subToken:
      result = value1 - value2;
      break;
    case divToken:
      result = value1 / value2;
      break;
    case mulToken:
      result = value1 * value2;
      break;
    case powToken:
      result = std::pow(value1, value2);
      break;
    case modToken:
      result = std::fmod(value1, value2);
      break;
    default:
      break;
  }

  return result;
}
}  // namespace s21