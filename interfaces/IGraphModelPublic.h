//
// Created by bulatruslanovich on 22/02/2024.
//

#ifndef SMARTCALC_IGRAPHMODELPUBLIC_H
#define SMARTCALC_IGRAPHMODELPUBLIC_H

#include <string>
#include <vector>

#include "observers/IObserverGraph.h"

namespace s21 {
class IGraphModelPublic {
 public:
  virtual ~IGraphModelPublic() = default;

  virtual double GetXBegin() = 0;
  virtual double GetXEnd() = 0;
  virtual double GetYBegin() = 0;
  virtual double GetYEnd() = 0;
  virtual bool GetYScaleAuto() = 0;
  virtual const std::vector<std::pair<double, double>>& GetPoints() = 0;

  virtual void AddObserver(IObserverGraphUpdate* observer) = 0;
  virtual void RemoveObserver(IObserverGraphUpdate* observer) = 0;
  virtual void AddObserver(IObserverGraphError* observer) = 0;
  virtual void RemoveObserver(IObserverGraphError* observer) = 0;
};
}  // namespace s21

#endif  // SMARTCALC_IGRAPHMODELPUBLIC_H
