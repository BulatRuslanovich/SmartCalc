//
// Created by bulatruslanovich on 25/02/2024.
//

#ifndef SMARTCALCV2_DEPOSITCONTROLLER_H
#define SMARTCALCV2_DEPOSITCONTROLLER_H

#include "../interfaces/IDepositController.h"
#include "../interfaces/IDepositModel.h"

namespace s21 {

class DepositController final : public IDepositController {
 public:
  DepositController() = delete;
  explicit DepositController(IDepositModel* model);
  DepositController(const DepositController& other) = delete;
  DepositController(DepositController&& other) = delete;
  DepositController& operator=(const DepositController& other) = delete;
  DepositController& operator=(DepositController&& other) = delete;
  ~DepositController() override = default;

  void SetDepositSum(double sum) override;
  void SetDepositPeriod(int months) override;
  void SetDepositInterest(double interest) override;
  void SetDepositTax(double tax) override;
  void SetDepositPeriodicityType(int type) override;
  void SetDepositCapitalization(bool capitalization) override;
  void SetDepositDateStart(int year, int month, int day) override;
  void AddDepositOperationReplenishment(int year, int month, int day,
                                        double amount) override;
  void AddDepositOperationWithdrawal(int year, int month, int day,
                                     double amount) override;

  void ClearDepositOperationsList() override;
  void CalculateDeposit() override;

 private:
  IDepositModel* _model;
};

}  // namespace s21

#endif  // SMARTCALCV2_DEPOSITCONTROLLER_H
