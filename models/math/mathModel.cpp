//
// Created by bulatruslanovich on 23/02/2024.
//

#include "mathModel.h"

#include <cmath>
#include <limits>
#include <list>
#include <stack>
#include <utility>

#include "token.h"

namespace s21 {
/**
 * \brief Конструктор по умолчанию для класса MathModel.
 */
MathModel::MathModel() : MathModel("") {}

/**
 * \brief Конструктор класса MathModel.
 * \param str Строка с математическим выражением.
 */
MathModel::MathModel(std::string str)
    : needInit(true),
      isCalc(false),
      x(0.0),
      expression(std::move(str)),
      rpnExpression(""),
      tokensList({}),
      mathResult(std::numeric_limits<double>::quiet_NaN()),
      radDegFactor(1) {};

/**
 * \brief Устанавливает новое математическое выражение.
 * \param newExpression Новое математическое выражение.
 */
void MathModel::setExpression(const std::string& newExpression) {
  if (newExpression != expression) {
    expression = newExpression;
    needInit = true;
    isCalc = false;
  }
}

/**
 * \brief Добавляет строку к текущему математическому выражению.
 * \param str Строка, которая добавляется к текущему выражению.
 * \throw InputTooLongException Если длина результата превышает максимально
 * допустимый размер. \throw InputIncorrectException Если ввод некорректен.
 * \throw WrongOperatorException Если произошла ошибка оператора.
 */
void MathModel::ExpressionAddString(const std::string& str) {
  std::string res = getExpression();

  if (res.length() > maxInputSize) {
    throw InputTooLongException("Input string too long");
  }

  try {
    if (str == ".") {
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

/**
 * \brief Удаляет последний токен или символ из текущего математического
 * выражения.
 */
void MathModel::ExpressionDeleteEnd() {
  std::string res = getExpression();

  try {
    res = tokenHandle::RemoveTokenFromStrEnd(res);
  } catch (std::invalid_argument&) {
    if (!res.empty()) {
      res.erase(--res.end());
    }
  }

  setExpression(res);
}

/**
 * \brief Возвращает обратную польскую запись текущего математического
 * выражения.
 */
std::string MathModel::getRpnExpression() {
  if (needInit) {
    InitCalcMachine();
  }

  return rpnExpression;
}

/**
 * \brief Выполняет вычисление математического выражения.
 */
void MathModel::Calculate() {
  ResetModelResults();
  mathResult = CalcX(x);
}

/**
 * \brief Инициализирует математическую модель перед началом вычислений.
 */
void MathModel::InitMathModel() {
  CalcRpn();
  InitCalcMachine();
  needInit = false;
}

/**
 * \brief Вычисляет выражение в обратной польской нотации (RPN).
 * \throw InputZeroLengthException если входная строка пуста.
 * \throw InputIncorrectException если входная строка слишком длинная, содержит
 * недопустимые символы, несоответствующие скобки или неизвестные токены.
 * \throw InputIncorrectException если скобки в выражении не согласованы.
 */
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

        if ((lastToken.left2right && lastToken.priority >= check.priority) ||
            (!lastToken.left2right && lastToken.priority > check.priority)) {
          AppendStrWithPopStack(res, tokenStack, lastToken.str);
        } else {
          break;
        }
      }

      tokenStack.push(check);
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

/**
 * \brief Добавляет строку в результирующее выражение и извлекает токен из
 * стека.
 *
 * \param[in,out] res Результирующее выражение, в которое добавляется строка.
 * \param[in,out] stack Стек токенов.
 * \param[in] str Строка, которая добавляется в результирующее выражение.
 */
void MathModel::AppendStrWithPopStack(
    std::string& res, std::stack<Token>& stack,
    std::string& str) {  // TODO: может поломаться
  res.append(str);
  res.append(" ");
  stack.pop();
}

/**
 * \brief Инициализирует вычислительную машину путем разбора выражения в
 * обратной польской нотации (ОПН). \throws InputIncorrectException если во
 * входной строке встречаются неизвестные символы.
 */
void MathModel::InitCalcMachine() {
  try {
    tokensList = tokenHandle::ParseTokens(rpnExpression);
  } catch (std::invalid_argument&) {
    throw InputIncorrectException("Unknown chars in input string");
  }
}

/**
 * @brief Сбрасывает результаты модели.
 */
void MathModel::ResetModelResults() noexcept {
  mathResult = std::numeric_limits<double>::quiet_NaN();
}

/**
 * @brief Выполняет вычисление выражения для конкретного значения переменной x.
 *
 * @param xValue Значение переменной x, для которого вычисляется выражение.
 * @return Результат вычисления выражения.
 * @throw InputIncorrectException Если происходит ошибка в процессе вычисления
 * выражения.
 */
double MathModel::CalcX(double xValue) {
  if (needInit) {
    InitMathModel();
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
      tokenStack.push(Token::MakeToken(numberToken, xValue));
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

  if (!tokenStack.empty()) {
    throw InputIncorrectException("Calculate error");
  } else {
    res = lastToken.value;
  }

  isCalc = true;
  return res;
}

/**
 * @brief Выполняет унарную операцию над значением.
 *
 * @param value Значение, над которым выполняется операция.
 * @param type Тип унарной операции.
 * @return Результат унарной операции.
 */
double MathModel::CalcUnaryOp(double value, TokenType type) const noexcept {
  double result = 0.0;

  switch (type) {
    case unMinusToken:
      result = 0 - value;
      break;
    case cosToken:
      result = std::cos(value * radDegFactor);
      break;
    case sinToken:
      result = std::sin(value * radDegFactor);
      break;
    case tanToken:
      result = std::tan(value * radDegFactor);
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
    case factorialToken:
      result = MathModel::factorial(value);
      break;
    default:
      break;
  }

  return result;
}


/**
 * @brief Выполняет бинарную операцию над двумя значениями.
 *
 * @param value1 Первое значение.
 * @param value2 Второе значение.
 * @param type Тип бинарной операции.
 * @return Результат бинарной операции.
 */
double MathModel::CalcBinaryOp(double value1, double value2,
                               TokenType type) noexcept {
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

double MathModel::factorial(double value) {
  constexpr auto table = Table<66>();
  assert(value >= 0);
  return value < 66 ? static_cast<double>(table.t[static_cast<int>(value)]) : 0;
}


}  // namespace s21