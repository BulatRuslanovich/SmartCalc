//
// Created by bulatruslanovich on 22/02/2024.
//

#ifndef SMARTCALC_ICREDITCONTROLLER_H
#define SMARTCALC_ICREDITCONTROLLER_H

#include <QString>

namespace s21 {

class ICreditController {
 public:
  virtual ~ICreditController() = default;

  virtual void SetCreditSum(double sum) = 0;
  virtual void SetCreditMonths(int months) = 0;
  virtual void SetCreditInterest(double interest) = 0;
  virtual void SetCreditType(int type) = 0;

  virtual void CalculateCredit() = 0;
};
}  // namespace s21

#endif  // SMARTCALC_ICREDITCONTROLLER_H
