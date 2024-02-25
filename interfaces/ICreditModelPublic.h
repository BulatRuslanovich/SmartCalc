//
// Created by bulatruslanovich on 22/02/2024.
//

#ifndef SMARTCALC_ICREDITMODELPUBLIC_H
#define SMARTCALC_ICREDITMODELPUBLIC_H

#include <string>

#include "observers/IObserverCredit.h"

namespace s21 {
enum CreditType {
    Annuity, // Аннуитетный график
    Differentiated, // Дифференцированный график
};

class ICreditModelPublic {
   public:
    virtual ~ICreditModelPublic() = default;

    virtual double GetSum() = 0;
    virtual int GetMonths() = 0;
    virtual double GetInterest() = 0;
    virtual CreditType GetType() = 0;
    virtual double GetMin() = 0;
    virtual double GetMax() = 0;
    virtual double GetOver() = 0;
    virtual double GetTotal() = 0;
    virtual bool IsCalc() = 0;

    virtual void AddObserver(IObserverCreditUpdate* observer) = 0;
    virtual void RemoveObserver(IObserverCreditUpdate* observer) = 0;
    virtual void AddObserver(IObserverCreditError* observer) = 0;
    virtual void RemoveObserver(IObserverCreditError* observer) = 0;
};
}

#endif  // SMARTCALC_ICREDITMODELPUBLIC_H
