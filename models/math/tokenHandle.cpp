//
// Created by bulatruslanovich on 23/02/2024.
//

#include "tokenHandle.h"

#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <charconv>

#include "../../helpers/stringHelper.h"

namespace s21 {

/**
 * \brief Парсит строку на токены.
 * \param str Строка для парсинга.
 * \return Список токенов, полученных в результате парсинга строки.
 */
std::list<Token> tokenHandle::ParseTokens(const std::string& str) {
  std::list<Token> tokenList;
  std::string lowerStr{str};

  std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                 [](unsigned char c) { return std::tolower(c); });

  std::string_view view{lowerStr};

  stringHelper::TrimLeft(view);

  while (!view.empty()) {
    if (std::isdigit(view.front())) {
      auto [token, newView] = GetNumTokenFromStr(view);
      view = newView; // ? auto [token, view] = GetNumTokenFromStr(view); не работает
      tokenList.push_back(token);
    } else {
      auto [token, newView] = GetNoNumTokenFromStr(view);
      if (token.type == unknownToken) {
        throw std::invalid_argument("Unknown token in parser");
      }

      view = newView;
      tokenList.push_back(token);
    }

    stringHelper::TrimLeft(view);
  }

  return tokenList;
}

/**
 * \brief Корректирует список токенов, добавляя недостающие скобки в выражениях.
 * \param list Список токенов для корректировки.
 */
void tokenHandle::FixTokenList(std::list<Token>& list) {
  tokenHandle::FixUnOperationTokensList(list); // Корректирует унарные операции в списке

  int lbracketCount = 0; // Счётчик открывающих скобок
  int isOpen = 0; // Показывает, открыта ли текущая скобка

  auto tokenBegin = list.begin();
  auto tokenEnd = list.end();

  for (int i = 0; tokenBegin != tokenEnd; ++i, ++tokenBegin) {
    Token check = *tokenBegin;
    Token prevCheck;

    // Получаем предыдущий токен в списке
    if (i > 0) {
      auto tmp = tokenBegin;
      prevCheck = *(--tmp);
    }

    // Подсчёт открытых и закрытых скобок
    if (lbracketCount > 0 && check.type == lBracketToken) {
      ++isOpen;
    }

    if (lbracketCount > 0 && check.type == rBracketToken) {
      --isOpen;
    }

    // Автоматическое добавление открывающей скобки после функции, если пользователь забыл
    if (i > 0 && prevCheck.IsFunc() && check.type != lBracketToken) {
      list.insert(tokenBegin, Token::MakeToken(lBracketToken)); // Вставляем открывающую скобку

      ++lbracketCount; // Увеличиваем счётчик открывающих скобок
      continue;
    }

    // Закрывает автоматически открытые скобки: sin(sin(sin(sin(x + ))))
    if (lbracketCount > 0 && !check.IsFunc() && check.type != lBracketToken && !isOpen) {
      // Добавляем недостающие закрывающие скобки
      while (lbracketCount > 0) {
        list.insert(tokenBegin, Token::MakeToken(rBracketToken));
        --lbracketCount;
      }
    }
  }

  // Добавляем недостающие закрывающие скобки в конце списка, если они остались
  if (lbracketCount > 0) {
    while (lbracketCount > 0) {
      list.insert(tokenBegin, Token::MakeToken(rBracketToken));
      --lbracketCount;
    }
  }
}


/**
 * \brief Корректирует список токенов, присваивая унарные операции токенам, которые ранее были определены как бинарные.
 * \param list Список токенов для корректировки.
 *
 * В начале работы программы токены операций не различаются на унарные или бинарные.
 * Эта функция сканирует список токенов и определяет, является ли текущий токен унарной операцией,
 * основываясь на его позиции в выражении и предыдущих токенах.
 * Если текущий токен должен быть унарной операцией, но ранее был определен как бинарный оператор или
 * расположен после открывающей скобки, его тип токена изменяется на соответствующий унарный оператор.
 */
void tokenHandle::FixUnOperationTokensList(std::list<Token>& list) {
  auto tokenBegin = list.begin();
  auto tokenEnd = list.end();
  Token prevCheck;

  for (int i = 0; tokenBegin != tokenEnd; ++i, ++tokenBegin) {
    Token check = *tokenBegin;

    if (i == 0 || prevCheck.IsOperator() || prevCheck.type == lBracketToken) {
      if (check.type == addToken) {
        *tokenBegin = Token::MakeToken(unPlusToken);
      } else if (check.type == subToken) {
        *tokenBegin = Token::MakeToken(unMinusToken);
      }
    }

    prevCheck = check;
  }
}


/**
 * \brief Добавляет токен к строке выражения и возвращает результат.
 * \param str Исходная строка выражения.
 * \param token Токен, который нужно добавить к выражению.
 * \return Строка выражения после добавления токена.
 */
std::string tokenHandle::AddTokenToStr(const std::string& str, const Token& token) {
  std::list<Token> tokenList = tokenHandle::ParseTokens(str);

  if (tokenList.empty()) {
    if ((token.IsOperator() && token.IsBinary()) || token.type == rBracketToken) {
      throw std::logic_error("Operator error");
    } else if (token.IsFunc()) {
      tokenList.push_back(token);
      tokenList.push_back(Token::MakeToken(lBracketToken));
    } else {
      tokenList.push_back(token);
    }
  } else {
    tokenHandle::FixUnOperationTokensList(tokenList);
    Token prevToken = tokenList.back();

    if (token.type == unMinusToken) {
      tokenHandle::AddUnMinusToList(tokenList);
    } else if (token.type == lBracketToken) { // ...) + * + (
      if (prevToken.IsConst() || prevToken.type == rBracketToken) {
        tokenList.push_back(Token::MakeToken(mulToken));
      }
      
      tokenList.push_back(token);
    } else if (token.IsConst()) {
      if (token.type == numberToken && prevToken.type == numberToken) { // ...4 + 3
        tokenList.back().str.append(token.str);
      } else {
        if (prevToken.IsConst() || prevToken.type == rBracketToken) {
          tokenList.push_back(Token::MakeToken(mulToken));
        }

        tokenList.push_back(token);
      }
    } else if (token.IsFunc()) {
      if (prevToken.IsConst() || prevToken.type == rBracketToken) {
        tokenList.push_back(Token::MakeToken(mulToken));
      }
      tokenList.push_back(token);
      tokenList.push_back(Token::MakeToken(lBracketToken));
    } else if (token.IsOperator()) {
      // -* +* (*
      if (prevToken.type == unMinusToken || prevToken.type == unPlusToken || prevToken.type ==lBracketToken) {
        throw std::logic_error("Operator error");
      } else if (prevToken.IsOperator()) {
        tokenList.pop_back();
        tokenList.push_back(token); //TODO: узнать, должен ли он вести себя так при двух операторах подряд. ТК хрень получается какая то
      } else {
        tokenList.push_back(token);
      }
    } else if (token.type == rBracketToken) {
      // () шоб таких не было приколов
      if (prevToken.type == lBracketToken) {
        throw std::logic_error("Operator error");
        // ...)) и таких прЕколов не должно быть (вот за*бусь писать тесты для этого)
      } else if (!IsLBracketExist(tokenList)) {
        throw std::logic_error("Operator error");
      } else if (prevToken.IsOperator()) {
        // ...-)
        if (prevToken.type == unMinusToken || prevToken.type == unPlusToken) {
          throw std::logic_error("Operator error");
        } else {
          tokenList.pop_back();
          tokenList.push_back(token); //TODO: хотите сказать он должен хавать ...*)
        }
      } else {
        tokenList.push_back(token);
      }
    } else {
      tokenList.push_back(token);
    }
  }

  std::string result = tokenHandle::ListToStr(tokenList);
  std::replace(result.begin(), result.end(), '~', '-');
  return result;
}

/**
 * \brief Добавляет десятичную точку к числу в конце строки выражения и возвращает результат.
 * \param str Исходная строка выражения.
 * \return Строка выражения после добавления десятичной точки.
 */
std::string tokenHandle::AddPointToStr(const std::string& str) {
  std::list<Token> tokenList = tokenHandle::ParseTokens(str);

  if (!tokenList.empty()) {
    Token prevToken = tokenList.back();

    if (prevToken.type == numberToken) {
      if (prevToken.str.find('.') == std::string::npos) {
        tokenList.back().str.append(".");
      }
    } else if (prevToken.IsConst() || prevToken.type == rBracketToken) {
      tokenList.push_back(Token::MakeToken(mulToken));
      tokenList.push_back(Token::MakeToken(numberToken, 0, "0."));
    } else {
      tokenList.push_back(Token::MakeToken(numberToken, 0, "0."));
    }
  } else {
    tokenList.push_back(Token::MakeToken(numberToken, 0, "0."));
  }

  std::string result = tokenHandle::ListToStr(tokenList);
  return  result;
}

/**
 * \brief Удаляет последний токен из строки выражения и возвращает результат.
 * \param str Исходная строка выражения.
 * \return Строка выражения после удаления последнего токена.
 */
std::string tokenHandle::RemoveTokenFromStrEnd(const std::string& str) {
  std::list<Token> tokenList = tokenHandle::ParseTokens(str);

  if (!tokenList.empty()) {
    Token lastToken = tokenList.back();

    if (lastToken.type == numberToken) {
      tokenList.back().str.erase(--(tokenList.back().str.end())); //-- для того чтобы указывать на ластовый, тк end() указывает на элемент после ластового

      // если число было из одного символа
      if (tokenList.back().str.empty()) {
        tokenList.pop_back();
      }
    } else {
      tokenList.pop_back();

      if (lastToken.type == lBracketToken) {
        if (!tokenList.empty()) {
          Token afterLastToken = tokenList.back();

          // если затереть ...sin( то синус тоже должен отлететь
          if (afterLastToken.IsFunc()) {
            tokenList.pop_back();
          }
        }
      }
    }
  }

  std::string result = tokenHandle::ListToStr(tokenList);
  return result;
}

/**
 * \brief Преобразует список токенов в строку.
 * \param list Список токенов.
 * \return Строка, содержащая токены из списка, разделенные пробелами.
 *
 * Эта функция преобразует список токенов в строку, разделяя каждый токен пробелом.
 * Результирующая строка содержит все токены из списка, объединенные в одну строку.
 */
std::string tokenHandle::ListToStr(const std::list<Token>& list) {
  std::string result;
  auto tokenBegin = list.begin();
  auto tokenEnd = list.end();
  int lastNum = static_cast<int>(list.size() - 1);
  // Выражения (int) и static_cast<int> используются для приведения типов
  // в C++ и выполняют аналогичную функцию, но есть некоторые
  // различия в их применении:
  //
  // (int) expression: Это старый стиль приведения типов,
  // известный как C-style cast. Он просто приводит выражение
  // expression к типу int. Вместо явного указания типа, как
  // это делает static_cast, он использует синтаксис скобок и
  // типа внутри скобок для определения типа приведения.
  //
  // static_cast<int>(expression): Это новый стиль приведения
  // типов в C++, который выполняет явное преобразование типа.
  // Он говорит компилятору явно преобразовать выражение
  // expression к типу int. В отличие от C-style cast,
  // static_cast более точен и позволяет лучше контролировать
  // преобразование типов.
  //
  // Различия между ними:
  //
  // static_cast обеспечивает более явное и строгое
  // преобразование типов, что делает код более читаемым и понятным.
  //( int) может привести к неоднозначности и
  // нежелательному поведению в некоторых случаях,
  // например, если используется для приведения
  // указателя к целочисленному типу.
  // В общем случае рекомендуется использовать
  // static_cast, так как он предоставляет больше
  // контроля и явно указывает на операцию преобразования типа в коде.

  for (int i = 0; tokenBegin != tokenEnd; ++tokenBegin, ++i) {
    result.append((*tokenBegin).str);

    if (i != lastNum) {
      result.append(" ");
    }
  }

  return  result;
}

/**
 * \brief Получает токен из строки без учета числовых значений.
 * \param str Строка, из которой извлекается токен.
 * \return Пара, содержащая токен и оставшуюся часть строки после извлечения токена.
 *
 * Эта функция извлекает токен из строки, игнорируя числовые значения.
 * Она ищет совпадение начала строки с ключами в таблице соответствия типов токенов.
 * Если найдено совпадение, функция создает токен соответствующего типа и возвращает его вместе
 * с оставшейся частью строки после извлечения токена.
 * Если совпадение не найдено, функция возвращает пустой токен и оставшуюся часть входной строки.
 */
std::pair<Token, std::string_view> tokenHandle::GetNoNumTokenFromStr(const std::string_view& str) {
  Token token;
  std::string_view newView{str};

  auto typeMap = TokenTypeMapper::getTypeMap();
  for (auto [key, type] : typeMap) {
    if (str.compare(0, key.size(), key) == 0) {
      newView.remove_prefix(key.size());
      token = Token::MakeToken(type);
      return {token, newView};
    }
  }

  return {token, newView};
}

/**
 * \brief Получает числовой токен из строки.
 * \param str Строка, из которой извлекается числовой токен.
 * \return Пара, содержащая числовой токен и оставшуюся часть строки после извлечения токена.
 *
 * Эта функция извлекает числовой токен из начала строки и возвращает его вместе с оставшейся частью строки.
 * Для извлечения числа из строки используется std::from_chars, а для создания токена используется Token::MakeToken.
 * Если извлечение числа не удалось (например, если строка не начинается с числа), функция бросает исключение std::invalid_argument.
 */
std::pair<Token, std::string_view> tokenHandle::GetNumTokenFromStr(const std::string_view& str) {
  Token token;
  std::string_view newView{str};

  double value = 0;
  auto viewStart = newView.data();
  auto viewEnd = newView.data() + newView.size();

  auto [ptr, ec] = std::from_chars(viewStart, viewEnd, value);

  if (ec != std::errc()) {
    throw std::invalid_argument("Invalid parsing from input string");
  }

  std::string::size_type ptrDiff = ptr - viewStart;
  std::string numStr = std::string{newView.substr(0, ptrDiff)};
  token = Token::MakeToken(numberToken, value, numStr);
  newView.remove_prefix(ptrDiff);

  return {token, newView};
}

/**
 * \brief Проверяет, существует ли в списке токенов больше открывающих скобок, чем закрывающих.
 * \param list Список токенов для проверки.
 * \return true, если число открывающих скобок больше числа закрывающих, иначе false.
 *
 * Эта функция проходит по списку токенов и подсчитывает количество открывающих и закрывающих скобок.
 * Если число открывающих скобок больше числа закрывающих, функция возвращает true, иначе false.
 */
bool tokenHandle::IsLBracketExist(const std::list<Token>& list) {
  int left = 0;
  int right = 0;
  auto tokenBegin = list.begin();
  auto tokenEnd = list.end();

  for (; tokenBegin != tokenEnd; ++tokenBegin) {
    if ((*tokenBegin).type == lBracketToken) {
      ++left;
    } else if ((*tokenBegin).type == rBracketToken) {
      ++right;
    }
  }

  return left > right;
}


/**
 * \brief Добавляет унарный минус к списку токенов, если необходимо.
 * \param list Список токенов, к которому нужно добавить унарный минус.
 *
 * Эта функция проверяет, нужно ли добавить унарный минус к списку токенов, и добавляет его при необходимости.
 * В случае, если последний токен списка является правой скобкой, функция добавляет умножение и унарный минус.
 * В противном случае функция анализирует последовательность токенов, чтобы определить, сколько унарных минусов уже присутствует.
 * Если число унарных минусов четное, то добавляется еще один унарный минус.
 */
void tokenHandle::AddUnMinusToList(std::list<Token>& list)  {
  Token lastToken = list.back();

  if (lastToken.type == rBracketToken) {
    list.push_back(Token::MakeToken(mulToken));
    list.push_back(Token::MakeToken(unMinusToken));
  } else {
    bool needRestore = false;
    Token saveToken;
    int unMinusCount = 0;
    while (!list.empty()) {
      Token check = list.back();

      if (check.IsConst()) {
        saveToken = check;
        needRestore = true;
        list.pop_back();
        continue;
      }

      if (check.type == unPlusToken) {
        list.pop_back();
        continue;
      }

      if (check.type == unMinusToken) {
        list.pop_back();
        ++unMinusCount;
        continue;
      }

      break;
    }

    if (unMinusCount % 2 == 0) {
      list.push_back(Token::MakeToken(unMinusToken));
    }

    if (needRestore) {
      list.push_back(saveToken);
    }
  }
}


}  // namespace s21