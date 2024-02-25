//
// Created by bulatruslanovich on 22/02/2024.
//

#ifndef SMARTCALC_IGRAPHMODEL_H
#define SMARTCALC_IGRAPHMODEL_H

#include "IGraphModelPublic.h"

namespace s21 {
class IGraphModel : public IGraphModelPublic {
   public:
    virtual ~IGraphModel() = default;

    virtual void SetExpression(const std::string& newExp) = 0;
    virtual void SetBeginX(double newX) = 0;
    virtual void SetEndX(double newX) = 0;
    virtual void SetBeginY(double newY) = 0;
    virtual void SetEndY(double newY) = 0;
    virtual void SetYScaleAuto(bool flag) = 0;

    virtual void CalculateGraph() = 0;
};
}

#endif  // SMARTCALC_IGRAPHMODELPUBLIC_H
