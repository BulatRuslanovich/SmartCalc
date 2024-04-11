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
      _tax(13),
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
 * используя текущие значения суммы, срока, процентной ставки и других
 * параметров.
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
 * Метод CheckModelData() проверяет корректность введенных данных модели
 * депозита, включая сумму, срок, процентную ставку, налоги и операции с
 * депозитом. Если какой-либо параметр некорректен, метод выбрасывает
 * исключение.
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

//  if (!CheckTax()) {
//    throw s21::FinanceTaxException("Incorrect tax");
//  }

  CheckOperations();
}

/**
 * @brief Проверяет корректность введенной суммы депозита.
 *
 * Метод CheckSum() проверяет корректность введенной суммы депозита.
 * @return true, если сумма депозита корректна, в противном случае - false.
 * @note Этот метод не бросает исключений (noexcept).
 */
bool DepositModel::CheckSum() const noexcept { return CheckSum(_sum); }

/**
 * @brief Проверяет корректность введенной суммы депозита.
 *
 * Метод CheckSum() проверяет корректность переданной суммы депозита.
 * @param sum Сумма депозита.
 * @return true, если сумма депозита корректна, в противном случае - false.
 * @note Этот метод не бросает исключений (noexcept).
 */
bool DepositModel::CheckSum(double sum) const noexcept {
  return !(sum < minDepositSum || sum > maxDepositSum || std::isinf(sum) ||
           std::isnan(sum));
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
 * Метод CheckInterest() проверяет корректность введенной процентной ставки
 * депозита.
 * @return true, если процентная ставка депозита корректна, в противном случае -
 * false.
 * @note Этот метод не бросает исключений (noexcept).
 */
bool DepositModel::CheckInterest() const noexcept {
  return !(_interest < minDepositInterest || _interest > maxDepositInterest ||
           std::isinf(_interest) || std::isnan(_interest));
}

/**
 * @brief Проверяет корректность введенного налога для депозита.
 *
 * Метод CheckTax() проверяет корректность введенного налога для депозита.
 * @return true, если налог для депозита корректен, в противном случае - false.
 * @note Этот метод не бросает исключений (noexcept).
 */
//bool DepositModel::CheckTax() const noexcept {
//  return !(_tax < minDepositTax || _tax > maxDepositTax || std::isinf(_tax) ||
//           std::isnan(_tax));
//}

/**
 * @brief Проверяет корректность даты операции с депозитом.
 *
 * Метод CheckDate() проверяет корректность введенной даты операции с депозитом.
 * @return true, если дата операции корректна, в противном случае - false.
 * @note Этот метод не бросает исключений (noexcept).
 */
bool DepositModel::CheckDate(int year, int month, int day) const noexcept {
  return !(year < minDepositYear || year > maxDepositYear || month < 1 ||
           month > 12 || day < 1 || day > 31 ||
           day > mathHelper::GetDaysCount(month, year));
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
    for (const auto& i : operationsList) {
      if (!CheckSum(std::fabs(i.amount))) {
        if (std::signbit(i.amount)) {
          throw s21::FinanceDateOperationMinusException(
              "Incorrect operation sum");
        } else {
          throw s21::FinanceDateOperationPlusException(
              "Incorrect operation sum");
        }
      }

      if (!CheckDate(i.year, i.month, i.day)) {
        if (std::signbit(i.amount)) {
          throw s21::FinanceDateOperationMinusException(
              "Incorrect operation sum");
        } else {
          throw s21::FinanceDateOperationPlusException(
              "Incorrect operation sum");
        }
      }
    }
  }
}

/**
 * @brief Выполняет расчет вклада.
 *
 * Метод производит расчет параметров вклада, таких как общая сумма выплаты
 * процентов, общая сумма вклада и общий налог на доход. Результаты записываются
 * в соответствующие поля класса.
 *
 * @throws s21::FinanceDateStartException Если указана некорректная дата начала
 * вклада.
 * @throws s21::FinanceOperationNoMoneyException Если на вкладе недостаточно
 * средств для проведения операции.
 */
void DepositModel::CalculateHandle() {
  int startYear = yearStart;
  int startMonth = monthStart;
  int startDay = dayStart;

  if (!CheckDate(startYear, startMonth,
                 startDay)) {
    throw s21::FinanceDateStartException("Incorrect date start");
  }

  double totalInterestPayment = 0;  // Общая сумма выплаты процентов
  double totalInterestPaymentForTax = 0;  // Общая сумма выплаты процентов для налога
  double totalSum = _sum;  // Общая сумма вклада
  double totalTax = 0;         // Общий налог на доход

  double pd = _interest / (100 * 365);
  double pdv =
      _interest / (100 * 366);  // Расчет процентной ставки на високосный день

  int counter = _months;  // Устанавливаем счетчик на количество месяцев

  while (counter >= 0) {
    int numDays = mathHelper::GetDaysCount(
        startMonth, startYear);
    int paymentDays = 0;  // Счетчик дней выплат
    double monthSum = 0;  // Общая сумма за месяц

    for (int day = 1; day <= numDays; ++day) {  // Проходим по всем дням месяца
      if (totalSum < 0) {
        throw s21::FinanceOperationNoMoneyException("No money :(");
      }

      if (counter == _months) {  // Если это первый месяц
        if (day <= startDay) {  // Если еще не наступил день начала
          if (day == startDay) {  // Если сегодня день начала
            OperationsHandle(startYear, startMonth, day,
                             totalSum);  // Обрабатываем операции
          }

          continue;
        }
      }

      if (counter == 0) {      // Если это последний месяц
        if (day > startDay) {  // Если уже прошел день начала
          continue;
        }
      }

      double percentSum;

      if (mathHelper::GetDaysCount(2, startYear) ==
          29) {                       // Если год високосный
        percentSum = pdv * totalSum;  // Рассчитываем процентную сумму
      } else {
        percentSum = pd * totalSum;  // Иначе рассчитываем процентную сумму
      }

      if (payPeriod == Day) {  // Если период выплаты - ежедневно
        percentSum = mathHelper::Round(
            percentSum, 2);  // Округляем сумму до двух знаков после запятой

        if (_capitalization) {  // Если проценты капитализируются
          totalSum += percentSum;  // Добавляем проценты к общей сумме
        }
      }

      monthSum +=
          percentSum;  // Добавляем проценты за день к общей сумме за месяц
      ++paymentDays;  // Увеличиваем количество дней выплат

      OperationsHandle(startYear, startMonth, day,
                       totalSum);  // Обрабатываем операции
    }

    if (payPeriod == Month) {  // Если период выплаты - ежемесячно
      monthSum = mathHelper::Round(
          monthSum,
          2);  // Округляем сумму за месяц до двух знаков после запятой

      if (_capitalization) {  // Если проценты капитализируются
        totalSum += monthSum;  // Добавляем проценты за месяц к общей сумме
      }
    }

    totalInterestPayment +=
        monthSum;  // Добавляем сумму за месяц к общей сумме выплаты процентов
    totalInterestPaymentForTax += monthSum;

    ++startMonth;  // Увеличиваем месяц на один

    if (startMonth == 13) {  // Если месяц стал больше 12
      startMonth = 1;  // Устанавливаем месяц в январь
      ++startYear;     // Увеличиваем год

      if (totalInterestPaymentForTax > 160000) {
        totalTax += mathHelper::Round((totalInterestPaymentForTax - 1000000 * 0.16) * (_tax / 100),
                                     2);
        totalInterestPaymentForTax = 0;
      }
    }

    --counter;  // Уменьшаем счетчик
  }

  if (payPeriod == End) {  // Если период выплаты - в конце срока
    totalInterestPayment = mathHelper::Round(
        totalInterestPayment, 2);  // Округляем общую сумму выплаты процентов до
                                   // двух знаков после запятой

    if (_capitalization) {  // Если проценты капитализируются
      totalSum += totalInterestPayment;  // Добавляем общую сумму выплаты
                                         // процентов к общей сумме
    }
  }

  if (totalInterestPaymentForTax > 160000) {
    totalTax += mathHelper::Round((totalInterestPaymentForTax - 1000000 * 0.16) * (_tax / 100),
                                 2);
  }

  interestAmount =
      totalInterestPayment;  // Записываем общую сумму выплаты процентов
  depositAmount = totalSum;  // Записываем общую сумму вклада
  taxAmount = totalTax;  // Записываем общий налог на доход
  _isCalc = true;  // Устанавливаем флаг завершения расчетов
}

/**
 * @brief Обрабатывает операции в указанную дату.
 *
 * Этот метод ищет операции в списке операций для указанной даты
 * и добавляет их суммы к общей сумме.
 *
 * @param year Год операции.
 * @param month Месяц операции.
 * @param day День операции.
 * @param total_sum Общая сумма, к которой добавляются суммы операций.
 */
void DepositModel::OperationsHandle(int year, int month, int day,
                                    double& total_sum) const {
  if (!operationsList.empty()) {
    for (const auto& item : operationsList) {
      if (year == item.year && month == item.month && day == item.day) {
        total_sum += item.amount;
      }
    }
  }
}

}  // namespace s21