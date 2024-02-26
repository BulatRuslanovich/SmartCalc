//
// Created by bulatruslanovich on 23/02/2024.
//

#ifndef SMARTCALC_MATHHELPER_H
#define SMARTCALC_MATHHELPER_H

#include <string>

namespace s21::mathHelper {
double Round(double value, int precision);
std::string intToString(int value);
std::string doubleToString(double value, int precision = 2);
std::string RemoveTrailingZero(const std::string& input);
int GetDaysCount(int month, int year);
}  // namespace s21::mathHelper

#endif  // SMARTCALC_MATHHELPER_H
