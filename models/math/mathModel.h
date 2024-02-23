//
// Created by bulatruslanovich on 23/02/2024.
//

#ifndef SMARTCALCV2_MATHMODEL_H
#define SMARTCALCV2_MATHMODEL_H
#include <list>
#include <string>

#include "../calcException.h"
#include "tokenHandle.h"

namespace s21 {

class MathModel final {
 public:
  static constexpr std::size_t maxInputSize = 255;

  MathModel();
  explicit MathModel(const std::string& str);

  MathModel(const MathModel& other) = delete;
  MathModel(MathModel&& other) = delete;
  MathModel& operator=(const MathModel& other) = delete;
  MathModel& operator=(MathModel&& other) = delete;

  ~MathModel() = default;

  void setExpression(const std::string& newExpression);
  void setX(double  newX) {x = newX;};
  void ExpressionAddString(const std::string& str);
  void ExpressionDeleteEnd();

  std::string getExpression() const { return expression; };
  std::string getRpnExpression();
  double getX_Value() const noexcept { return x; };
  double getResult() const noexcept { return result; };
  bool getIsCalc() const noexcept { return isCalc; };

  void Calculate();

 private:
  void InitMathModel();
  void CalcRpn();
  void InitCalcMachine();
  void ResetModelResults() noexcept;
  double CalcX(double x = 0);
  double CalcUnaryOp(double value, TokenType type) const noexcept;
  double CalcBinaryOp(double value1, double value2, TokenType type) const noexcept;

  bool needInit;
  bool isCalc;
  double x;
  std::string expression;
  std::string rpnExpression;
  std::list<Token> tokensList;
  double mathResult;
};

}  // namespace s21

#endif  // SMARTCALCV2_MATHMODEL_H
