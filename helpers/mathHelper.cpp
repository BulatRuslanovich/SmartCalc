//
// Created by bulatruslanovich on 23/02/2024.
//

#include "mathHelper.h"

#include <cmath>

namespace s21 {

namespace mathHelper {

/**
 * \brief Округляет число с плавающей запятой до заданной точности.
 * \param value Значение, которое требуется округлить.
 * \param precision Количество десятичных знаков, до которых нужно округлить.
 * \return Округленное значение.
 */
double Round(double value, int precision) {
  double k = std::pow(10, precision);
  return (std::round(value * k) / k);
}

/**
 * \brief Преобразует целочисленное значение в строку.
 * \param value Целочисленное значение, которое требуется преобразовать.
 * \return Строковое представление целого числа.
 */
std::string intToString(int value) { return std::to_string(value); }

/**
 * \brief Удаляет завершающие нули из строкового представления числа.
 * \param input Строка, представляющая число.
 * \return Строка без завершающих нулей.
 */
std::string RemoveTrailingZeros(const std::string& input) {
  std::string result{input};
  if (result.find('.') != std::string::npos) {
    result = result.substr(0, result.find_last_not_of('0') + 1);

    if (result.find('.') == result.size() - 1) {
      result = result.substr(0, result.size() - 1);
    }
  }

  return result;
}

/**
 * \brief Преобразует число с плавающей запятой в строку с указанной точностью.
 * \param value Число с плавающей запятой, которое требуется преобразовать.
 * \param precision Количество десятичных знаков, которое должно быть в строке.
 * \return Строковое представление числа с плавающей запятой с указанной
 * точностью.
 */
std::string doubleToString(double value, int precision) {
  double tmp = Round(value, precision);
  std::string result = std::to_string(tmp);
  return RemoveTrailingZeros(result);
}

/**
 * \brief Возвращает количество дней в указанном месяце заданного года.
 * \param month Месяц (1-12).
 * \param year Год.
 * \return Количество дней в указанном месяце заданного года.
 */
int GetDaysCount(int month, int year) {
  return 31 - (102 / month & 1) -
         ((4 / month & 2) >> (!(year % 4) - !(year % 100) + !(year % 400)));
}

}  // namespace s21::mathHelpers
}