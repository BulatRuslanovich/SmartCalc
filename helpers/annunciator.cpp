//
// Created by bulatruslanovich on 23/02/2024.
//

#include "annunciator.h"

#include <algorithm>
namespace s21 {
/**
 * \brief Добавляет наблюдателя для обновлений в списке наблюдателей.
 * \param observer Указатель на объект, реализующий интерфейс IObserverUpdate.
 */
void Annunciator::AddObserver(s21::IObserverUpdate* observer) {
  updateObservers.push_back(observer);
}

/**
 * \brief Удаляет наблюдателя для обновлений из списка наблюдателей.
 * \param observer Указатель на объект, реализующий интерфейс IObserverUpdate.
 */
void Annunciator::RemoveObserver(IObserverUpdate* observer) {
  updateObservers.remove(observer);
}

/**
 * \brief Добавляет наблюдателя для ошибок в списке наблюдателей.
 * \param observer Указатель на объект, реализующий интерфейс IObserverError.
 */
void Annunciator::AddObserver(s21::IObserverError* observer) {
  errorObservers.push_back(observer);
}

/**
 * \brief Удаляет наблюдателя для ошибок из списка наблюдателей.
 * \param observer Указатель на объект, реализующий интерфейс IObserverError.
 */
void Annunciator::RemoveObserver(IObserverError* observer) {
  errorObservers.remove(observer);
}

/**
 * \brief Уведомляет всех наблюдателей об обновлении.
 */
void Annunciator::UpdateObservers() {
  std::for_each(updateObservers.cbegin(), updateObservers.cend(),
                [](IObserverUpdate* item) { item->Update(); });
}

/**
 * \brief Уведомляет всех наблюдателей об ошибке.
 * \param error Пара строк, представляющая тип и описание ошибки.
 */
void Annunciator::ErrorObservers(
    const std::pair<std::string, std::string>& error) {
  std::for_each(errorObservers.cbegin(), errorObservers.cend(),
                [&error](IObserverError* item) { item->Error(error); });
}

}  // namespace s21