//
// Created by bulatruslanovich on 22/02/2024.
//

#ifndef SMARTCALC_IOBSERVERCREDIT_H
#define SMARTCALC_IOBSERVERCREDIT_H

#include "IObserver.h"

namespace s21 {
class IObserverCreditUpdate : public IObserverUpdate {
   public:
    virual ~IObserverCreditUpdate() = default;
}

class IObserverCreditError : public IObserverError {
   public:
    virual ~IObserverCreditError() = default;
}
#endif  // SMARTCALC_IOBSERVERCREDIT_H
