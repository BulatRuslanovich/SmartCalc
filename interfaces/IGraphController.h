//
// Created by bulatruslanovich on 22/02/2024.
//

#ifndef SMARTCALC_IGRAPHCONTROLLER_H
#define SMARTCALC_IGRAPHCONTROLLER_H

#include <QString>

namespace s21 {

class IGraphController {
 public:
  virtual ~IGraphController() = default;

  virtual void SetExpression(const QString &str) = 0;
  virtual void SetBeginX(double newX) = 0;
  virtual void SetEndX(double newX) = 0;
  virtual void SetBeginY(double newY) = 0;
  virtual void SetEndY(double newY) = 0;
  virtual void SetYScaleAuto(bool flag) = 0;

  virtual void CalculateGraph() = 0;
};
}  // namespace s21

#endif  // SMARTCALC_IGRAPHCONTROLLER_H
