//
// Created by bulatruslanovich on 25/02/2024.
//

#ifndef SMARTCALCV2_MATHCONTROLLER_H
#define SMARTCALCV2_MATHCONTROLLER_H

#include <QString>

#include "../interfaces/IMathController.h"
#include "../interfaces/IMathModel.h"

namespace s21 {

class MathController : public IMathController {
 public:
  MathController() = delete;
  explicit MathController(IMathModel* model);
  MathController(const MathController& other) = delete;
  MathController(MathController&& other) = delete;
  MathController& operator=(const MathController& other) = delete;
  MathController& operator=(MathController&& other) = delete;
  ~MathController() override = default;

  void SetExpression(const QString& str) override;
  void SetX(double x) override;
  void ExpressionAdd(const char* add) override;
  void ExpressionEraseEnd() override;
  void SetRadDegFactor(double value) override;

  void CalculateX() override;
 private:
  IMathModel* _model;
};

}  // namespace s21

#endif  // SMARTCALCV2_MATHCONTROLLER_H
