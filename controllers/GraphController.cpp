//
// Created by bulatruslanovich on 25/02/2024.
//

#include "GraphController.h"

namespace s21 {
GraphController::GraphController(IGraphModel *model) : _model(model) {}

void GraphController::SetExpression(const QString &str) {
  _model->SetGraphExpression(str.toStdString());
}

void GraphController::SetBeginX(double newX) { _model->SetBeginX(newX); }

void GraphController::SetEndX(double newX) { _model->SetEndX(newX); }

void GraphController::SetBeginY(double newY) { _model->SetBeginY(newY); }

void GraphController::SetEndY(double newY) { _model->SetEndY(newY); }

void GraphController::SetYScaleAuto(bool flag) { _model->SetYScaleAuto(flag); }
void GraphController::CalculateGraph() { _model->CalculateGraph(); }

}  // namespace s21