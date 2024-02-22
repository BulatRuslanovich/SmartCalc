//
// Created by bulatruslanovich on 22/02/2024.
//

#ifndef SMARTCALC_IMATHMODELPUBLIC_H
#define SMARTCALC_IMATHMODELPUBLIC_H

#include "IDepositModelPublic.h"

namespace s21 {

class IDepositModel : public IDepositModelPublic {
   public:
    virtual ~IDepositModel() = default;

    virtual void SetDepositSum(double sum) = 0;
    virtual void SetDepositPeriod(int months) = 0;
    virtual void SetDepositInterest(double interest) = 0;
    virtual void SetDepositTax(double tax) = 0;
    virtual void SetDepositPeriodicityType(DepositType type) = 0;
    virtual void SetDepositCapitalization(bool capitalization) = 0;
    virtual void SetDepositDateStart(int year, int month, int day) = 0;
    virtual void AddDepositOperationReplenishment(int year, int month, int day,
                                                  double amount) = 0;
    virtual void AddDepositOperationWithdrawal(int year, int month, int day,
                                               double amount) = 0;

    virtual void ClearDepositOperationsList() = 0;
    virtual void CalculateDeposit() = 0;

};


}

#endif  // SMARTCALC_IMATHMODELPUBLIC_H
