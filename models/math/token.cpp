//
// Created by bulatruslanovich on 23/02/2024.
//

#include "token.h"

#include <limits>

namespace s21 {
/**
 * \brief Конструктор по умолчанию для класса Token.
 * Инициализирует объект типом unknownToken, пустой строкой, приоритетом 0,
 * направлением left2right и значением 0.
 */
Token::Token()
    : type(unknownToken), str(""), priority(0), left2right(true), value(0) {}

/**
 * \brief Конструктор с параметрами для класса Token.
 * Инициализирует объект заданными значениями типа, строки, приоритета,
 * направления и значения.
 * \param type Тип токена.
 * \param str Строковое представление токена.
 * \param priority Приоритет токена.
 * \param left2right Направление ассоциативности оператора.
 * \param value Значение токена (по умолчанию 0).
 */
Token::Token(TokenType type, const std::string &str, int priority,
             bool left2right, double value)
    : type(type),
      str(str),
      priority(priority),
      left2right(left2right),
      value(value) {}

/**
 * \brief Конструктор с параметрами для класса Token без значения.
 * Инициализирует объект заданными значениями типа, строки, приоритета и
 * направления.
 * Значение устанавливается в NaN (не число).
 * \param type Тип токена.
 * \param str Строковое представление токена.
 * \param priority Приоритет токена.
 * \param left2right Направление ассоциативности оператора.
 */
Token::Token(TokenType type, const std::string &str, int priority,
             bool left2right)
    : type(type),
      str(str),
      priority(priority),
      left2right(left2right),
      value(std::numeric_limits<double>::quiet_NaN()) {}

/**
 * \brief Создает токен с заданным типом, значением и строковым представлением.
 * \param type Тип токена.
 * \param value Значение токена (для чисел).
 * \param string Строковое представление токена.
 * \return Созданный токен.
 */
Token Token::MakeToken(TokenType type, double value,
                       const std::string &string) {
  switch (type) {
    // Возвращает токен в зависимости от типа
    // и устанавливает соответствующее значение и строку
  }
}

/**
 * \brief Проверяет, является ли токен константой.
 * \return True, если токен является константой, иначе False.
 */
bool Token::IsConst() const noexcept {
  return type == numberToken || type == xToken || type == eToken ||
         type == piToken;
}

/**
 * \brief Проверяет, является ли токен функцией.
 * \return True, если токен является функцией, иначе False.
 */
bool Token::IsFunc() const noexcept {
  return type == cosToken || type == sinToken || type == tanToken ||
         type == acosToken || type == asinToken || type == atanToken ||
         type == sqrtToken || type == lnToken || type == logToken;
}

/**
 * \brief Проверяет, является ли токен оператором.
 * \return True, если токен является оператором, иначе False.
 */
bool Token::IsOperator() const noexcept {
  return type == addToken || type == subToken || type == divToken ||
         type == mulToken || type == powToken || type == unMinusToken ||
         type == unPlusToken || type == modToken;
}

/**
 * \brief Проверяет, является ли токен унарным.
 * \return True, если токен является унарным, иначе False.
 */
bool Token::IsUnary() const noexcept {
  return type == unMinusToken || type == unPlusToken || IsFunc();
}

/**
 * \brief Проверяет, является ли токен бинарным оператором.
 * \return True, если токен является бинарным оператором, иначе False.
 */
bool Token::IsBinary() const noexcept {
  return type == addToken || type == subToken || type == divToken ||
         type == mulToken || type == powToken || type == modToken;
}

}  // namespace s21