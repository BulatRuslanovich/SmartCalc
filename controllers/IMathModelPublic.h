//
// Created by bulatruslanovich on 22/02/2024.
//

#ifndef SMARTCALC_IMATHMODELPUBLIC_H
#define SMARTCALC_IMATHMODELPUBLIC_H

#include <string>

#include "observers/IObserverMath.h"

namespace s21 {
class IMathModelPublic {
   public:
    virtual ~IMathModelPublic() = default;

    virtual double GetResult() = 0;
    virtual double GetX() = 0;
    virtual std::string GetExpression() = 0;
    virtual bool IsCalc() = 0;

    virtual void AddObserver(IObserverMathUpdate* observer) = 0;
    virtual void RemoveObserver(IObserverMathUpdate* observer) = 0;
    virtual void AddObserver(IObserverMathError* observer) = 0;
    virtual void RemoveObserver(IObserverMathError* observer) = 0;
};
}

#endif  // SMARTCALC_IMATHMODELPUBLIC_H
