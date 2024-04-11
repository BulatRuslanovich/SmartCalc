//
// Created by bulatruslanovich on 25/02/2024.
//

#ifndef SMARTCALCV2_CREDITMODEL_H
#define SMARTCALCV2_CREDITMODEL_H
#include "../financeException.h"

namespace s21 {

class CreditModel final {
 public:
  enum CreditType { Annuity, Differentiated };

  const double minCreditSum = 0.01;
  const double maxCreditSum = 9999999999.99;
  const int minCreditMonths = 1;
  const int maxCreditMonths = 600;
  const double minCreditInterest = 0.01;
  const double maxCreditInterest = 999.0;

  CreditModel() noexcept;
  CreditModel(const CreditModel& other) = delete;
  CreditModel(CreditModel&& other) = delete;
  CreditModel& operator=(const CreditModel& other) = delete;
  CreditModel& operator=(CreditModel&& other) = delete;
  ~CreditModel() = default;

  void setSum(double newSum) noexcept {
    _sum = newSum;
    _isCalc = false;
  }

  void setMonths(int newMonths) {
    _months = newMonths;
    _isCalc = false;
  }

  void setInterest(double newInterest) {
    _interest = newInterest;
    _isCalc = false;
  }

  void setType(CreditType newType) {
    _type = newType;
    _isCalc = false;
  }

  [[nodiscard]] double getSum() const noexcept { return _sum; };
  [[nodiscard]] double getInterest() const noexcept { return _interest; };
  [[nodiscard]] int getMonths() const noexcept { return _months; };
  [[nodiscard]] CreditType getType() const noexcept { return _type; };
  [[nodiscard]] bool getIsCalc() const noexcept { return _isCalc; };

  [[nodiscard]] double getPaymentMin() const noexcept { return paymentMin; };
  [[nodiscard]] double getPaymentMax() const noexcept { return paymentMax; };
  [[nodiscard]] double getOverPayment() const noexcept { return overPayment; };
  [[nodiscard]] double getTotalPayment() const noexcept { return totalPayment; };

  void CalculateCredit();

 private:
  void ResetResults() noexcept;
  void CalculateHandle() noexcept;
  void CheckModel() const;
  [[nodiscard]] bool CheckSum() const noexcept;
  [[nodiscard]] bool CheckMonth() const noexcept;
  [[nodiscard]] bool CheckInterest() const noexcept;

  double _sum;
  int _months;
  double _interest;
  CreditType _type;

  bool _isCalc;
  double paymentMin;
  double paymentMax;
  double overPayment;
  double totalPayment;
};

}  // namespace s21

#endif  // SMARTCALCV2_CREDITMODEL_H
