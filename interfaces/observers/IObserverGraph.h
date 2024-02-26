//
// Created by bulatruslanovich on 22/02/2024.
//

#ifndef SMARTCALC_IOBSERVERGRAPH_H
#define SMARTCALC_IOBSERVERGRAPH_H

#include "IObserver.h"

namespace s21 {
class IObserverGraphUpdate : public IObserverUpdate {
 public:
  virtual ~IObserverGraphUpdate() = default;
};

class IObserverGraphError : public IObserverError {
 public:
  virtual ~IObserverGraphError() = default;
};
}  // namespace s21
#endif  // SMARTCALC_IOBSERVERGRAPH_H
