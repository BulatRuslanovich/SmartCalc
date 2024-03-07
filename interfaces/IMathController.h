//
// Created by bulatruslanovich on 22/02/2024.
//

#ifndef SMARTCALC_IMATHCONTROLLER_H
#define SMARTCALC_IMATHCONTROLLER_H

#include <QString>

namespace s21 {
class IMathController {
 public:
  virtual ~IMathController() = default;

  virtual void SetExpression(const QString& str) = 0;
  virtual void SetX(double x) = 0;
  virtual void SetRadDegFactor(double value) = 0;
  virtual void ExpressionAdd(const char* add) = 0;
  virtual void ExpressionEraseEnd() = 0;

  virtual void CalculateX() = 0;
};
}  // namespace s21

#endif  // SMARTCALC_IMATHCONTROLLER_H
