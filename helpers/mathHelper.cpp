//
// Created by bulatruslanovich on 23/02/2024.
//

#include "mathHelper.h"
#include <cmath>

namespace s21 {
namespace mathHelpers {

double Round(double value, int precision) {
    double k = std::pow(10, precision);
    return (std::round(value * k) / k);
}

double FinanceRound(double value) { return Round(value, 2); }

std::string intToString(int value) { return std::to_string(value); }

std::string doubleToString(double value, int precision) {
    double tmp = Round(value, precision);
    std::string result = std::to_string(tmp);
    return RemoveTrailingZeros(result);
}

// TODO: -0 нужно будет пофиксить
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


int GetDaysCount(int month, int year) {
    return 31 - (102 / month & 1) - ((4 / month & 2) >> (!(year % 4) - !(year % 100) + !(year % 400)));
}

}  // namespace s21