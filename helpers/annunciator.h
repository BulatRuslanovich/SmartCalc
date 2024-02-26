//
// Created by bulatruslanovich on 23/02/2024.
//

#ifndef SMARTCALC_ANNUNCIATOR_H
#define SMARTCALC_ANNUNCIATOR_H
#include <list>

#include "../interfaces/observers/IObserver.h"

namespace s21 {

class Annunciator final {
 public:
  void AddObserver(IObserverUpdate* observer);
  void RemoveObserver(IObserverUpdate* observer);
  void AddObserver(IObserverError* observer);
  void RemoveObserver(IObserverError* observer);

  void UpdateObservers();
  void ErrorObservers(const std::pair<std::string, std::string>& error);

 private:
  std::list<IObserverUpdate*> updateObservers;
  std::list<IObserverError*> errorObservers;
};

}  // namespace s21

#endif  // SMARTCALC_ANNUNCIATOR_H
