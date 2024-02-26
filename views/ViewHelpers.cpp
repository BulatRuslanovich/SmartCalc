//
// Created by bulatruslanovich on 25/02/2024.
//

#include "ViewHelpers.h"

#include <QCalendarWidget>

#include "../helpers/mathHelper.h"

namespace s21::ViewHelpers {

/**
 * @brief Настраивает виджет редактирования даты.
 *
 * Эта функция устанавливает формат отображения даты, включает всплывающее
 * календарное окно, устанавливает русскую локаль для календаря и устанавливает
 * текущую дату.
 *
 * @param dateEdit Указатель на виджет редактирования даты (QDateEdit).
 */
void SetupDateEdit(QDateEdit *dateEdit) {
  dateEdit->setDisplayFormat("dd.MM.yyyy");  // Устанавливаем формат отображения
                                             // даты на "день.месяц.год"
  dateEdit->setCalendarPopup(true);  // Включаем всплывающее календарное окно
  dateEdit->calendarWidget()->setLocale(
      QLocale("ru_RU"));  // Устанавливаем русскую локаль для календаря
  QDate date = QDate::currentDate();  // Получаем текущую дату
  dateEdit->setDate(
      date);  // Устанавливаем текущую дату в виджете редактирования даты
}

/**
 * @brief Находит родительский макет, содержащий указанный виджет.
 *
 * Эта функция рекурсивно обходит дочерние элементы родительского макета и его
 * дочерних макетов, чтобы найти макет, содержащий указанный виджет.
 *
 * @param widget Указатель на виджет, для которого ищется родительский макет.
 * @param parent Указатель на родительский макет, в котором происходит поиск.
 * @return Указатель на найденный родительский макет, либо nullptr, если макет
 * не найден.
 */
QLayout *FindParentLayout(QWidget *widget, QLayout *parent) {
  // Проходим по всем дочерним элементам родительского макета
  for (QObject *obj : parent->children()) {
    auto *layout = qobject_cast<QLayout *>(obj);  // Преобразуем объект в макет

    // Если объект является макетом
    if (layout != nullptr) {
      // Если макет содержит указанный виджет, возвращаем этот макет
      if (layout->indexOf(widget) > -1) {
        return layout;
      }
      // Если макет содержит другие дочерние элементы, вызываем функцию
      // рекурсивно
      else if (!layout->children().isEmpty()) {
        layout = FindParentLayout(widget, layout);

        // Если родительский макет найден в дочерних элементах, возвращаем его
        if (layout != nullptr) {
          return layout;
        }
      }
    }
  }

  // Если родительский макет не найден, возвращаем nullptr
  return nullptr;
}

/**
 * @brief Находит родительский макет, содержащий указанный виджет.
 *
 * Эта функция рекурсивно обходит родительскую иерархию виджетов, чтобы найти
 * макет, содержащий указанный виджет.
 *
 * @param widget Указатель на виджет, для которого ищется родительский макет.
 * @return Указатель на найденный родительский макет, либо nullptr, если макет
 * не найден.
 */
QLayout *FindParentLayout(QWidget *widget) {
  // Если у виджета есть родительский виджет
  if (widget->parentWidget() != nullptr) {
    // Если у родительского виджета есть макет
    if (widget->parentWidget()->layout() != nullptr) {
      // Вызываем вспомогательную функцию для поиска макета в родительском
      // макете
      return FindParentLayout(widget, widget->parentWidget()->layout());
    }
  }

  // Если родительский макет не найден, возвращаем nullptr
  return nullptr;
}

/**
 * @brief Преобразует число типа double в строку типа QString с указанной
 * точностью.
 *
 * Этот метод принимает число типа double и преобразует его в строку типа
 * QString с указанной точностью. Удаляются лишние нули в конце числа.
 *
 * @param num Число типа double для преобразования.
 * @param precision Точность, количество знаков после запятой.
 * @return Строка типа QString, содержащая преобразованное число.
 */
QString DoubleToQStr(double num, double precision) {
  return QString::fromStdString(mathHelper::RemoveTrailingZero(
      QString::number(num, 'f', precision).toStdString()));
}

}  // namespace s21::ViewHelpers
