//
// Created by bulatruslanovich on 25/02/2024.
//

#include "DepositController.h"

namespace s21 {
DepositController::DepositController(IDepositModel *model) : _model(model) {}

void DepositController::SetDepositSum(double sum) {
  _model->SetDepositSum(sum);
}

void DepositController::SetDepositPeriod(int months) {
  _model->SetDepositPeriod(months);
}

void DepositController::SetDepositInterest(double interest) {
  _model->SetDepositInterest(interest);
}

void DepositController::SetDepositTax(double tax) {
  _model->SetDepositTax(tax);
}

void DepositController::SetDepositPeriodicityType(int type) {
  _model->SetDepositPeriodicityType(static_cast<IDepositType>(type));
}

void DepositController::SetDepositCapitalization(bool capitalization) {
  _model->SetDepositCapitalization(capitalization);
}

void DepositController::SetDepositDateStart(int year, int month, int day) {
  _model->SetDepositDateStart(year, month, day);
}

void DepositController::AddDepositOperationReplenishment(int year, int month,
                                                         int day,
                                                         double amount) {
  _model->AddDepositOperationReplenishment(year, month, day, amount);
}

void DepositController::AddDepositOperationWithdrawal(int year, int month,
                                                      int day, double amount) {
  _model->AddDepositOperationWithdrawal(year, month, day, amount);
}

void DepositController::ClearDepositOperationsList() {
  _model->ClearDepositOperationsList();
}

void DepositController::CalculateDeposit() { _model->CalculateDeposit(); }

}  // namespace s21