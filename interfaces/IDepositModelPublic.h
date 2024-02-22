//
// Created by bulatruslanovich on 22/02/2024.
//

#ifndef SMARTCALC_IMATHMODELPUBLIC_H
#define SMARTCALC_IMATHMODELPUBLIC_H

#include <string>

#include "observers/IObserverDeposit.h"

namespace s21 {
enum DepositType {
    Day,   // каждый день
    Month, // раз в месяц
    End,   // в конце срока
};

class IDepositModelPublic {
   public:
    virtual ~IDepositModelPublic() = default;

    virtual double GetInterestPayment() = 0;
    virtual double GetTotalPayment() = 0;
    virtual double GetTaxPayment() = 0;
    virtual bool IsCalc() = 0;
    virtual double GetSum() = 0;
    virtual int GetMonths() = 0;
    virtual double GetInterest() = 0;
    virtual double GetTax() = 0;
    virtual DepositType GetDepositType() = 0;
    virtual bool GetDepositCapitalizationType() = 0;

    virtual void AddObserver(IObserverDepositUpdate* observer) = 0;
    virtual void RemoveObserver(IObserverDepositUpdate* observer) = 0;
    virtual void AddObserver(IObserverDepositError* observer) = 0;
    virtual void RemoveObserver(IObserverDepositError* observer) = 0;
};


}

#endif  // SMARTCALC_IMATHMODELPUBLIC_H
