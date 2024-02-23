//
// Created by bulatruslanovich on 23/02/2024.
//

#include <cctype>
#include "stringHelper.h"

namespace s21 {
void TrimLeft(std::string_view& str) noexcept {
    std::size_t i = 0;

    for (; i < str.size() && std::isspace(str[i]); ++i) {}

    str.remove_prefix(i);
}
}  // namespace s21