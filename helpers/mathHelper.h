//
// Created by bulatruslanovich on 23/02/2024.
//

#ifndef SMARTCALC_MATHHELPER_H_
#define SMARTCALC_MATHHELPER_H_

#include <string>

namespace s21 {
namespace mathHelper {
double Round(double value, int precision);
std::string intToString(int value);
std::string doubleToString(double value, int precision = 2);
std::string RemoveTrailingZeros(const std::string& input);
int GetDaysCount(int month, int year);
}
}  // namespace s21::mathHelper

#endif  // SMARTCALC_MATHHELPER_H_
