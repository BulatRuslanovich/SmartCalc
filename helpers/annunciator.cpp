//
// Created by bulatruslanovich on 23/02/2024.
//

#include "annunciator.h"
#include <algorithm>
namespace s21 {
void Annunciator::AddObserver(s21::IObserverUpdate* observer) {
    updateObservers.push_back(observer);
}
void Annunciator::RemoveObserver(IObserverUpdate* observer) {
    updateObservers.remove(observer);
}
void Annunciator::AddObserver(s21::IObserverError* observer){
    errorObservers.push_back(observer);
}
void Annunciator::RemoveObserver(IObserverError* observer) {
    errorObservers.remove(observer);
}

void Annunciator::UpdateObservers() {
    std::for_each(updateObservers.cbegin(), updateObservers.cend(), [](IObserverUpdate* item) {item.Update()})
}

void Annunciator::ErrorObservers(const std::pair<std::string, std::string>& error) {
    std::for_each(errorObservers.cbegin(), errorObservers.cend(), [&error](IObserverError* item) {item->Error(error)})
}

}  // namespace s21