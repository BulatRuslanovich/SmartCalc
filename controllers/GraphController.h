//
// Created by bulatruslanovich on 25/02/2024.
//

#ifndef SMARTCALCV2_GRAPHCONTROLLER_H
#define SMARTCALCV2_GRAPHCONTROLLER_H

#include <QString>

#include "../interfaces/IGraphController.h"
#include "../interfaces/IGraphModel.h"

namespace s21 {

class GraphController final : public IGraphController {
  GraphController() = delete;
  explicit GraphController(IGraphModel* model);
  GraphController(const GraphController& other) = delete;
  GraphController(GraphController&& other) = delete;
  GraphController& operator=(const GraphController& other) = delete;
  GraphController& operator=(GraphController&& other) = delete;
  ~GraphController() override = default;

  void SetExpression(const QString& str) override;
  void SetBeginX(double newX) override;
  void SetEndX(double newX) override;
  void SetBeginY(double newY) override;
  void SetEndY(double newY) override;
  void SetYScaleAuto(bool flag) override;

  void CalculateGraph() override;

 private:
  IGraphModel* _model;
};

}  // namespace s21

#endif  // SMARTCALCV2_GRAPHCONTROLLER_H
