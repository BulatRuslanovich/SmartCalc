//
// Created by bulatruslanovich on 25/02/2024.
//

#include "creditModel.h"

#include <cmath>

#include "../../helpers/mathHelper.h"
namespace s21 {

/**
     * @brief Конструктор по умолчанию.
     *
     * Инициализирует объект `CreditModel` с начальными значениями.
     * @note Этот конструктор не бросает исключений (noexcept).
 */
CreditModel::CreditModel() noexcept
    : _sum(0),
      _months(0),
      _interest(0),
      _type(Annuity),
      _isCalc(false),
      paymentMin(0),
      paymentMax(0),
      overPayment(0),
      totalPayment(0) {}

/**
     * @brief Выполняет расчет параметров кредита.
     *
     * Метод `CalculateCredit()` осуществляет расчет параметров кредита,
     * используя текущие значения суммы кредита, срока и процентной ставки.
 */
void CreditModel::CalculateCredit() {
  ResetResults();
  CheckModel();
  CalculateHandle();
}


/**
     * @brief Сбрасывает результаты расчета кредита.
     *
     * Метод `ResetResults()` сбрасывает результаты предыдущего расчета,
     * устанавливая все параметры платежей и переплаты в нулевые значения.
     * @note Этот метод не бросает исключений (noexcept).
 */
void CreditModel::ResetResults() noexcept {
  paymentMax = 0;
  paymentMin = 0;
  overPayment = 0;
  totalPayment = 0;
}

/**
     * @brief Выполняет расчет параметров кредита в зависимости от типа кредита.
     *
     * Метод `CalculateHandle()` осуществляет расчет параметров кредита
     * в зависимости от типа кредита (Аннуитетный или Дифференцированный).
     * @note Этот метод не бросает исключений (noexcept).
 */
void CreditModel::CalculateHandle() noexcept {
  if (_type == Annuity) {
    double totalPay = 0;
    double overPay = 0;
    double monthInterest = _interest / (100 * 12);
    double monthPay =
        _sum * (monthInterest +
                monthInterest / (std::pow(monthInterest + 1, _months) - 1));
    // x = S * (P + P / ((1 + P)^N - 1))
    // Здесь S — сумма займа, P — 1/100 доля процентной ставки (в месяц), N — срок кредитования (в месяцах).
    totalPay = monthPay * _months;
    overPay = totalPay - _sum;

    paymentMin = monthPay;
    paymentMax = monthPay;
    overPayment = overPay;
    totalPayment = totalPay;
  } else if (_type == Differentiated) {
    double monthPayMin = _sum + 2 * _sum * (maxCreditInterest / (12 * 100));
    double monthPayMax = 0;
    double totalPay = 0;
    double payMain = _sum / _months;
    double monthInterest = _interest / (100 * 12);
    double tempSum = _sum;

    for (int i = 0; i < _months; ++i) {
      double payAdd = tempSum * monthInterest;
      tempSum -= payMain;
      double monthPay = mathHelper::Round(payMain + payAdd, 2);
      totalPay += monthPay;

      if (monthPay > monthPayMax) {
        monthPayMax = monthPay;
      }

      if (monthPay < monthPayMin) {
        monthPayMin = monthPay;
      }
    }

    overPayment = totalPay - _sum;
    paymentMin = monthPayMin;
    paymentMax = monthPayMax;
    totalPayment = totalPay;
  }

  _isCalc = true;
}

/**
     * @brief Проверяет корректность данных модели кредита.
     *
     * Метод `CheckModel()` проверяет корректность введенных данных модели кредита,
     * включая сумму, срок и процентную ставку.
     * Если какой-либо из параметров некорректен, метод выбрасывает исключение.
 */
void CreditModel::CheckModel() const {
  if (!CheckSum()) {
    throw s21::FinanceSumException("Incorrect sum");
  }

  if (!CheckSum()) {
    throw s21::FinanceMonthException("Incorrect month");
  }

  if (!CheckSum()) {
    throw s21::FinanceInterestException("Incorrect interest");
  }
}

/**
     * @brief Проверяет корректность введенной суммы кредита.
     * @return true если сумма кредита корректна, в противном случае - false.
 */
bool CreditModel::CheckSum() const noexcept {
  return !(_sum < minCreditSum || _sum > maxCreditSum || std::isinf(_sum) ||
           std::isnan(_sum));
}

/**
     * @brief Проверяет корректность введенного срока кредита.
     * @return true если срок кредита корректен, в противном случае - false.
 */
bool CreditModel::CheckMonth() const noexcept {
  return !(_months < minCreditMonths || _months > maxCreditMonths);
}

/**
     * @brief Проверяет корректность введенной процентной ставки кредита.
     * @return true если процентная ставка кредита корректна, в противном случае - false.
 */
bool CreditModel::CheckInterest() const noexcept {
  return !(_interest < minCreditInterest || _interest > maxCreditInterest ||
           std::isnan(_interest) || std::isinf(_interest));
}

}