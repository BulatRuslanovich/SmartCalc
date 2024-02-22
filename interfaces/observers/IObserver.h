//
// Created by bulatruslanovich on 22/02/2024.
//

#ifndef SMARTCALC_IOBSERVER_H
#define SMARTCALC_IOBSERVER_H

#include <string>
#include <utility>

namespace s21 {
class IObserverUpdate {
   public:
    virtual ~IObserverUpdate() = default;
    virtual void Update() = 0;
};

class IObserverError {
   public:
    virtual ~IObserverError() = default;
    virtual void Error(const std::pair<std::string, std::string> &error) = 0;
};

}  // namespace s21


#endif  // SMARTCALC_IOBSERVER_H
