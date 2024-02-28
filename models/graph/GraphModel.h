//
// Created by bulatruslanovich on 24/02/2024.
//

#ifndef SMARTCALCV2_GRAPHMODEL_H
#define SMARTCALCV2_GRAPHMODEL_H

#include <vector>

#include "../calcException.h"
#include "../math/mathModel.h"

namespace s21 {

class GraphModel final {
 public:
  const double _minX = -1E6;
  const double _minY = -1E6;
  const double _maxX = 1E6;
  const double _maxY = 1E6;

  GraphModel();
  GraphModel(const GraphModel& other) = delete;
  GraphModel(GraphModel&& other) = delete;
  GraphModel& operator=(const GraphModel& other) = delete;
  GraphModel& operator=(GraphModel&& other) = delete;
  ~GraphModel();

  void setXBegin(double newX) noexcept { xBegin = newX; }
  void setYBegin(double newY) noexcept { yBegin = newY; }
  void setXEnd(double newX) noexcept { xEnd = newX; }
  void setYEnd(double newY) noexcept { yEnd = newY; }

  void setGraphExpression(const std::string& newExpression) {
    mathModel->setExpression(newExpression);
  }

  void setYScaleAuto(bool flag) noexcept { yScaleAuto = flag; }

  double getXBegin() const noexcept { return xBegin; }
  double getYBegin() const noexcept { return yBegin; }
  double getXEnd() const noexcept { return xEnd; }
  double getYEnd() const noexcept { return yEnd; }
  bool getYScaleAuto() const noexcept { return yScaleAuto; }
  const std::vector<std::pair<double, double>>& getPoints() { return _points; }

  void CalculateGraph();

 private:
  const int numPointsAll = 2E4;
  const int numPointsPre = 100;
  const int numOfCalc = 3;

  void ResetModelResults() noexcept;
  void NormalizeXRange() noexcept;
  void NormalizeYRange() noexcept;
  void CheckModelData() const;
  void CalculateHandle();
  void CalculatePoints(double _xBegin, double _xEnd, double numPoints,
                       double& yMin, double& yMax, int numOfCalcPoints);
  bool IsFunctionBreakPoint(double last, double current) const noexcept;
  void SetYRange(double yMin, double yMax) noexcept;

  MathModel* mathModel;
  double xBegin;
  double yBegin;
  double xEnd;
  double yEnd;
  bool yScaleAuto;

  std::vector<std::pair<double, double>> _points;
};

}  // namespace s21

#endif  // SMARTCALCV2_GRAPHMODEL_H
