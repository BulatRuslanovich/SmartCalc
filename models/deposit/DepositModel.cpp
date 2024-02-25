//
// Created by bulatruslanovich on 25/02/2024.
//

#include "DepositModel.h"

#include <cmath>

#include "../../helpers/mathHelper.h"

namespace s21 {

/**
 * @brief Конструктор класса DepositModel.
 *
 * Инициализирует объект класса DepositModel начальными значениями.
 * @note Этот конструктор не бросает исключений (noexcept).
 */
DepositModel::DepositModel() noexcept
    : _sum(0),
      _months(0),
      _interest(0),
      _tax(0),
      payPeriod(),
      _capitalization(false),
      yearStart(0),
      monthStart(0),
      dayStart(0),
      operationsList({}),
      _isCalc(false),
      interestAmount(0),
      depositAmount(0),
      taxAmount(0) {}

/**
 * @brief Выполняет расчет параметров депозита.
 *
 * Метод CalculateDeposit() осуществляет расчет параметров депозита,
 * используя текущие значения суммы, срока, процентной ставки и других параметров.
 */
void DepositModel::CalculateDeposit() {
  ResetModelResults();
  CheckModelData();
  CalculateHandle();
}

/**
 * @brief Сбрасывает результаты модели депозита.
 */
void DepositModel::ResetModelResults() noexcept {
  interestAmount = 0;
  depositAmount = 0;
  taxAmount = 0;
}

/**
 * @brief Проверяет корректность введенных данных модели депозита.
 *
 * Метод CheckModelData() проверяет корректность введенных данных модели депозита,
 * включая сумму, срок, процентную ставку, налоги и операции с депозитом.
 * Если какой-либо параметр некорректен, метод выбрасывает исключение.
 */
void DepositModel::CheckModelData() const {
  if (!CheckSum()) {
    throw s21::FinanceSumException("Incorrect sum");
  }

  if (!CheckMonth()) {
    throw s21::FinanceMonthException("Incorrect month");
  }

  if (!CheckInterest()) {
    throw s21::FinanceInterestException("Incorrect interest");
  }

  if (!CheckTax()) {
    throw s21::FinanceTaxException("Incorrect tax");
  }

  CheckOperations();
}

/**
 * @brief Проверяет корректность введенной суммы депозита.
 *
 * Метод CheckSum() проверяет корректность введенной суммы депозита.
 * @return true, если сумма депозита корректна, в противном случае - false.
 * @note Этот метод не бросает исключений (noexcept).
 */
bool DepositModel::CheckSum() const noexcept {
  return CheckSum(_sum);
}

/**
 * @brief Проверяет корректность введенной суммы депозита.
 *
 * Метод CheckSum() проверяет корректность переданной суммы депозита.
 * @param sum Сумма депозита.
 * @return true, если сумма депозита корректна, в противном случае - false.
 * @note Этот метод не бросает исключений (noexcept).
 */
bool DepositModel::CheckSum(double sum) const noexcept {
  return !(sum < minDepositSum || sum > maxDepositSum || std::isinf(sum) || std::isnan(sum));
}

/**
 * @brief Проверяет корректность введенного срока депозита.
 *
 * Метод CheckMonth() проверяет корректность введенного срока депозита.
 * @return true, если срок депозита корректен, в противном случае - false.
 * @note Этот метод не бросает исключений (noexcept).
 */
bool DepositModel::CheckMonth() const noexcept {
  return !(_months < minDepositMonths || _months > maxDepositMonths);
}

/**
 * @brief Проверяет корректность введенной процентной ставки депозита.
 *
 * Метод CheckInterest() проверяет корректность введенной процентной ставки депозита.
 * @return true, если процентная ставка депозита корректна, в противном случае - false.
 * @note Этот метод не бросает исключений (noexcept).
 */
bool DepositModel::CheckInterest() const noexcept {
  return !(_interest < minDepositInterest || _interest > maxDepositInterest || std::isinf(_interest) || std::isnan(_interest));
}

/**
 * @brief Проверяет корректность введенного налога для депозита.
 *
 * Метод CheckTax() проверяет корректность введенного налога для депозита.
 * @return true, если налог для депозита корректен, в противном случае - false.
 * @note Этот метод не бросает исключений (noexcept).
 */
bool DepositModel::CheckTax() const noexcept {
  return !(_tax < minDepositTax || _tax > maxDepositTax || std::isinf(_tax) || std::isnan(_tax));
}

/**
 * @brief Проверяет корректность даты операции с депозитом.
 *
 * Метод CheckDate() проверяет корректность введенной даты операции с депозитом.
 * @return true, если дата операции корректна, в противном случае - false.
 * @note Этот метод не бросает исключений (noexcept).
 */
bool DepositModel::CheckDate(int year, int month, int day) const noexcept {
  return !(year < minDepositYear || year > maxDepositYear || month < 1 || month > 12 || day < 1 || day > 31 || day > mathHelper::GetDaysCount(month, year));
}

/**
 * @brief Проверяет корректность всех операций с депозитом.
 *
 * Метод CheckOperations() проверяет корректность всех операций с депозитом,
 * включая сумму операции и дату операции.
 * Если какая-либо операция некорректна, метод выбрасывает исключение.
 */
void DepositModel::CheckOperations() const {
  if (!operationsList.empty()) {
    for (const auto & i : operationsList) {
      if (!CheckSum(std::fabs(i.amount))) {
        if (std::signbit(i.amount)) {
          throw s21::FinanceDateOperationMinusException("Incorrect operation sum");
        } else {
          throw s21::FinanceDateOperationPlusException("Incorrect operation sum");
        }
      }

      if (!CheckDate(i.year, i.month, i.day)) {
        if (std::signbit(i.amount)) {
          throw s21::FinanceDateOperationMinusException("Incorrect operation sum");
        } else {
          throw s21::FinanceDateOperationPlusException("Incorrect operation sum");
        }
      }
    }
  }
}

void DepositModel::CalculateHandle() {}
void DepositModel::OperationsHandle(int year, int month, int day,
                                    double& total_sum) const {}
}  // namespace s21