//
// Created by bulatruslanovich on 22/02/2024.
//

#ifndef SMARTCALC_IMATHMODELPUBLIC_H
#define SMARTCALC_IMATHMODELPUBLIC_H

#include "ICreditModelPublic.h"

namespace s21 {
class ICreditModel {
   public:
    virtual ~ICreditModel() = default;
    virtual void SetCreditSum(double sum) = 0;
    virtual void SetCreditMonths(int months) = 0;
    virtual void SetCreditInterest(double interest) = 0;
    virtual void SetCreditType(CreditType type) = 0;

    virtual void CalculateCredit() = 0;
};


}

#endif  // SMARTCALC_IMATHMODELPUBLIC_H
