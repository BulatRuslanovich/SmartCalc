//
// Created by bulatruslanovich on 22/02/2024.
//

#ifndef SMARTCALC_IMATHMODEL_H
#define SMARTCALC_IMATHMODEL_H

#include "IMathModelPublic.h"

namespace s21 {
class IMathModel : public IMathModelPublic {
 public:
  virtual ~IMathModel() = default;

  virtual void SetExpression(const std::string& newExp) = 0;
  virtual void SetX(double x) = 0;
  virtual void ExpressionAdd(const char* add) = 0;
  virtual void ExpressionDeleteEnd() = 0;

  virtual void CalculateX() = 0;
};
}  // namespace s21

#endif  // SMARTCALC_IMATHMODELPUBLIC_H
