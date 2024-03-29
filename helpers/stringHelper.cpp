//
// Created by bulatruslanovich on 23/02/2024.
//

#include "stringHelper.h"

#include <cctype>

namespace s21::stringHelper {
/**
 * \brief Удаляет все ведущие пробельные символы из строки.
 * \param str Ссылка на строку типа std::string_view, из которой нужно удалить
 * пробелы. \note Функция выполняет изменение строки на месте. \note Не
 * выбрасывает исключения.
 */
void TrimLeft(std::string_view& str) noexcept {
  std::size_t i = 0;

  for (; i < str.size() && std::isspace(str[i]); ++i) {
  }

  str.remove_prefix(i);
}
}  // namespace s21::stringHelper