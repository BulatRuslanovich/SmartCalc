//
// Created by bulatruslanovich on 22/02/2024.
//

#ifndef SMARTCALC_IDEPOSITMODELPUBLIC_H
#define SMARTCALC_IDEPOSITMODELPUBLIC_H

#include <string>

#include "observers/IObserverDeposit.h"

namespace s21 {
enum IDepositType {
  Day,    // каждый день
  Month,  // раз в месяц
  End,    // в конце срока
};

class IDepositModelPublic {
 public:
  virtual ~IDepositModelPublic() = default;

  virtual double GetInterestPayment() = 0;
  virtual double GetTotalPayment() = 0;
  virtual double GetTaxPayment() = 0;
  virtual bool IsDepositCalc() = 0;
  virtual double GetDepositSum() = 0;
  virtual int GetDepositMonths() = 0;
  virtual double GetDepositInterest() = 0;
  virtual double GetTax() = 0;
  virtual IDepositType GetDepositType() = 0;
  virtual bool GetDepositCapitalizationType() = 0;

  virtual void AddObserver(IObserverDepositUpdate* observer) = 0;
  virtual void RemoveObserver(IObserverDepositUpdate* observer) = 0;
  virtual void AddObserver(IObserverDepositError* observer) = 0;
  virtual void RemoveObserver(IObserverDepositError* observer) = 0;
};
}  // namespace s21

#endif  // SMARTCALC_IDEPOSITMODELPUBLIC_H
