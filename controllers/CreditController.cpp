//
// Created by bulatruslanovich on 25/02/2024.
//

#include "CreditController.h"

namespace s21 {
CreditController::CreditController(ICreditModel *model) : _model(model) {}

void CreditController::SetCreditSum(double sum) {
  _model->SetCreditSum(sum);
}

void CreditController::SetCreditMonths(int months) {
  _model->SetCreditMonths(months);
}

void CreditController::SetCreditInterest(double interest) {
  _model->SetCreditInterest(interest);
}

void CreditController::SetCreditType(int type) {
  _model->SetCreditType(static_cast<CreditType>(type));
}

void CreditController::CalculateCredit() {
  _model->CalculateCredit();
}

}  // namespace s21