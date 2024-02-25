//
// Created by bulatruslanovich on 25/02/2024.
//

#ifndef SMARTCALCV2_CREDITCONTROLLER_H
#define SMARTCALCV2_CREDITCONTROLLER_H

#include "../interfaces/ICreditController.h"
#include "../interfaces/ICreditModel.h"

namespace s21 {

class CreditController final : public ICreditController {
  CreditController() = delete;
  explicit CreditController(ICreditModel* model);
  CreditController(const CreditController& other) = delete;
  CreditController(CreditController&& other) = delete;
  CreditController& operator=(const CreditController& other) = delete;
  CreditController& operator=(CreditController&& other) = delete;
  ~CreditController() override = default;
  
  void SetCreditSum(double sum) override;
  void SetCreditMonths(int months) override;
  void SetCreditInterest(double interest) override;
  void SetCreditType(int type) override;
  
  void CalculateCredit() override;

 private:
  ICreditModel* _model;
};

}  // namespace s21

#endif  // SMARTCALCV2_CREDITCONTROLLER_H
