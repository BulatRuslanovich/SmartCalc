//
// Created by bulatruslanovich on 23/02/2024.
//

#ifndef SMARTCALCV2_MATHMODEL_H
#define SMARTCALCV2_MATHMODEL_H
#include <list>
#include <stack>
#include <string>
#include <cassert>
#include <cstdint>

#include "../calcException.h"
#include "tokenHandle.h"

namespace s21 {

class MathModel final {
 public:
  static constexpr std::size_t maxInputSize = 255;

  MathModel();
  explicit MathModel(std::string  str);

  MathModel(const MathModel& other) = delete;
  MathModel(MathModel&& other) = delete;
  MathModel& operator=(const MathModel& other) = delete;
  MathModel& operator=(MathModel&& other) = delete;

  ~MathModel() = default;

  void setExpression(const std::string& newExpression);
  void setX(double newX) { this->x = newX; };
  void setRadDegFactor(double radDegFactor) { this->radDegFactor = radDegFactor; };
  void ExpressionAddString(const std::string& str);
  void ExpressionDeleteEnd();

  [[nodiscard]] std::string getExpression() const { return expression; };
  std::string getRpnExpression();
  [[nodiscard]] double getX_Value() const noexcept { return x; };
  [[nodiscard]] double getResult() const noexcept { return mathResult; };
  [[nodiscard]] bool getIsCalc() const noexcept { return isCalc; };

  void Calculate();

 private:
  void InitMathModel();
  void CalcRpn();
  void InitCalcMachine();
  void ResetModelResults() noexcept;
  double CalcX(double x = 0);
  [[nodiscard]] double CalcUnaryOp(double value, TokenType type) const noexcept;
  [[nodiscard]] static double CalcBinaryOp(double value1, double value2,
                      TokenType type) noexcept;
  static void AppendStrWithPopStack(std::string& res, std::stack<Token>& stack,
                                    std::string& str);
  bool needInit;
  bool isCalc;
  double x;
  std::string expression;
  std::string rpnExpression;
  std::list<Token> tokensList;
  double mathResult;
  double radDegFactor;


  static double factorial(double value) ;
};

template<int N>
struct Table {
  constexpr Table() : t() {
    t[0] = 1;
    for (auto i = 1; i < N; ++i)
      t[i] = t[i - 1] * i;
  }
  std::uint64_t t[N];
};

}  // namespace s21

#endif  // SMARTCALCV2_MATHMODEL_H
