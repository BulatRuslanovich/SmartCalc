//
// Created by bulatruslanovich on 25/02/2024.
//

#ifndef SMARTCALCV2_DEPOSITMODEL_H
#define SMARTCALCV2_DEPOSITMODEL_H

#include <vector>

#include "../financeException.h"

namespace s21 {

class DepositModel {
 public:
  struct DepositOperation {
    DepositOperation() noexcept : year(0), month(0), day(0), amount(0) {}
    DepositOperation(int year, int month, int day, double amount) noexcept
        : year(year), month(month), day(day), amount(amount) {}
    // Год операции
    int year;
    // Месяц операции
    int month;
    // День операции
    int day;
    // Сумма операции. Отрицательная - снятие, Положительная - пополнение
    double amount;
  };

  enum PayPeriodDepositType {
    Day,
    Month,
    End,
  };

  // Минимальная сумма депозита
  const double minDepositSum = 0.01;
  // Максимальная сумма депозита
  const double maxDepositSum = 9999999999.99;
  // Минимальный срок депозита
  const int minDepositMonths = 1;
  // Максимальный срок депозита
  const int maxDepositMonths = 600;
  // Минимальная процентная ставка депозита
  const double minDepositInterest = 0.01;
  // Максимальная процентная ставка депозита
  const double maxDepositInterest = 999.0;
  // Минимальная процентная ставка налога
  const double minDepositTax = 0.0;
  // Максимальная процентная ставка налога
  const double maxDepositTax = 100.0;
  // Минимальный год в датах
  const int minDepositYear = 1900;
  // Максимальный год в датах
  const int maxDepositYear = 2100;

  DepositModel() noexcept;
  DepositModel(const DepositModel& other) = delete;
  DepositModel(DepositModel&& other) = delete;
  DepositModel& operator=(const DepositModel& other) = delete;
  DepositModel& operator=(DepositModel&& other) = delete;
  ~DepositModel() = default;

  void setSum(double newSum) noexcept {
    _sum = newSum;
    _isCalc = false;
  }

  void setMonths(int newMonths) noexcept {
    _months = newMonths;
    _isCalc = false;
  }

  void setInterest(double newInterest) noexcept {
    _interest = newInterest;
    _isCalc = false;
  }

  void setTax(double newTax) noexcept {
    _tax = newTax;
    _isCalc = false;
  }

  void setPayPeriod(PayPeriodDepositType newPayPeriod) noexcept {
    payPeriod = newPayPeriod;
    _isCalc = false;
  }
  void setCapitalization(bool newCapitalization) noexcept {
    _capitalization = newCapitalization;
    _isCalc = false;
  }
  void setStartDate(int year, int month, int day) noexcept {
    yearStart = year;
    monthStart = month;
    dayStart = day;
    _isCalc = false;
  }

  void AddOperation(int year, int month, int day, double amount) {
    operationsList.emplace_back(
        year, month, day, amount);
    _isCalc = false;
  }

  [[nodiscard]] double getSum() const noexcept { return _sum; }
  [[nodiscard]] int getMonths() const noexcept { return _months; }
  [[nodiscard]] double getInterest() const noexcept { return _interest; }
  [[nodiscard]] double getTax() const noexcept { return _tax; }
  [[nodiscard]] PayPeriodDepositType getPayPeriod() const noexcept { return payPeriod; }

  [[nodiscard]] bool getCapitalization() const noexcept { return _capitalization; }
  [[nodiscard]] int getYearStart() const noexcept { return yearStart; }
  [[nodiscard]] int getMonthStart() const noexcept { return monthStart; }
  [[nodiscard]] int getDayStart() const noexcept { return dayStart; }

  [[nodiscard]] double getInterestAmount() const noexcept { return interestAmount; }
  [[nodiscard]] double getDepositAmount() const noexcept { return depositAmount; }
  [[nodiscard]] double getTaxAmount() const noexcept { return taxAmount; }
  [[nodiscard]] bool getIsCalc() const noexcept { return _isCalc; }

  void CalculateDeposit();
  void ClearOperationsList() noexcept {
    operationsList.clear();
    _isCalc = false;
  }

 private:
  void ResetModelResults() noexcept;
  void CheckModelData() const;
  [[nodiscard]] bool CheckSum() const noexcept;
  [[nodiscard]] bool CheckSum(double sum) const noexcept;
  [[nodiscard]] bool CheckMonth() const noexcept;
  [[nodiscard]] bool CheckInterest() const noexcept;
  [[nodiscard]] bool CheckDate(int year, int month, int day) const noexcept;
  void CheckOperations() const;
  void CalculateHandle();
  void OperationsHandle(int year, int month, int day, double& total_sum) const;

  // Сумма депозита
  double _sum;
  // Период депозита
  int _months;
  // Процентная ставка депозита
  double _interest;
  // Процентная ставка налога на депозит
  double _tax;
  // Периодичность выплат
  PayPeriodDepositType payPeriod;
  // Капитализация процентов нв депозите
  bool _capitalization;
  // Дата начала срока депозита
  int yearStart;
  int monthStart;
  int dayStart;
  // Информация о пополнениях и снятиях
  std::vector<DepositOperation> operationsList;

  // Результаты расчета
  // Флаг, посчитано ли значение модели
  bool _isCalc;
  // Начисленные проценты
  double interestAmount;
  // Сумма на вкладе к концу срока
  double depositAmount;
  // Сумма налога
  double taxAmount;
};

}  // namespace s21

#endif  // SMARTCALCV2_DEPOSITMODEL_H
