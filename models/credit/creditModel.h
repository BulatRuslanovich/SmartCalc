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

  double getSum() const noexcept { return _sum; };
  double getInterest() const noexcept { return _interest; };
  int getMonths() const noexcept { return _months; };
  CreditType getType() const noexcept { return _type; };
  bool getIsCalc() const noexcept { return _isCalc; };

  double getPaymentMin() const noexcept { return paymentMin; };
  double getPaymentMax() const noexcept { return paymentMax; };
  double getOverPayment() const noexcept { return overPayment; };
  double getTotalPayment() const noexcept { return totalPayment; };

  void CalculateCredit();

 private:
  void ResetResults() noexcept;
  void CalculateHandle() noexcept;
  void CheckModel() const;
  bool CheckSum() const noexcept;
  bool CheckMonth() const noexcept;
  bool CheckInterest() const noexcept;

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

}

#endif  // SMARTCALCV2_CREDITMODEL_H
