//
// Created by bulatruslanovich on 25/02/2024.
//

#include "ViewHelpers.h"

#include <QCalendarWidget>

#include "../helpers/mathHelper.h"

namespace s21 {
namespace ViewHelpers {
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
  dateEdit->setDisplayFormat("dd.MM.yyyy");

  dateEdit->setCalendarPopup(true);
  dateEdit->calendarWidget()->setLocale(
      QLocale("ru_RU"));
  QDate date = QDate::currentDate();
  dateEdit->setDate(
      date);
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
  for (QObject *obj : parent->children()) {
    auto *layout = qobject_cast<QLayout *>(obj);

    if (layout != nullptr) {
      if (layout->indexOf(widget) > -1) {
        return layout;
      }

      else if (!layout->children().isEmpty()) {
        layout = FindParentLayout(widget, layout);

        if (layout != nullptr) {
          return layout;
        }
      }
    }
  }

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
  if (widget->parentWidget() != nullptr) {
    if (widget->parentWidget()->layout() != nullptr) {
      return FindParentLayout(widget, widget->parentWidget()->layout());
    }
  }

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
  return QString::fromStdString(mathHelper::RemoveTrailingZeros(
      QString::number(num, 'f', static_cast<int>(precision)).toStdString()));

}


}
}  // namespace s21::ViewHelpers
