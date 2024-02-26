//
// Created by bulatruslanovich on 25/02/2024.
//

#include "MathController.h"

namespace s21 {
MathController::MathController(IMathModel *model) : _model(model) {}
void MathController::SetExpression(const QString &str) {
  _model->SetExpression(str.toStdString());  // из view в model
}
void MathController::SetX(double x) { _model->SetX(x); }
void MathController::ExpressionAdd(const char *add) {
  _model->ExpressionAdd(add);
}
void MathController::ExpressionEraseEnd() { _model->ExpressionEraseEnd(); }
void MathController::CalculateX() { _model->CalculateX(); }
}  // namespace s21