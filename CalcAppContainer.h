//
// Created by bulatruslanovich on 28/02/2024.
//

#ifndef SMARTCALCV2_CALCAPPCONTAINER_H
#define SMARTCALCV2_CALCAPPCONTAINER_H

#include "MasterModel.h"
#include "controllers/CreditController.h"
#include "controllers/MathController.h"
#include "controllers/GraphController.h"
#include "controllers/DepositController.h"
#include "views/CreditView.h"
#include "views/DepositView.h"
#include "views/MainView.h"
#include "views/GraphView.h"
#include "views/MainView.h"

namespace s21 {

class CalcAppContainer final {
 public:
  CalcAppContainer();
  CalcAppContainer(const CalcAppContainer& other) = delete;
  CalcAppContainer(CalcAppContainer&& other) = delete;
  CalcAppContainer& operator=(const CalcAppContainer& other) = delete;
  CalcAppContainer& operator=(CalcAppContainer&& other) = delete;
  ~CalcAppContainer();
  
  void Init();

 private:
  MasterModel* _model;
  GraphController* _graphController;
  GraphView* _graphView;
  MathController* _mathController;
  MathView* _mathView;
  CreditController* _creditController;
  CreditView* _creditView;
  DepositController* _depositController;
  DepositView* _depositView;
  MainView* _mainView;
};

}  // namespace s21

#endif  // SMARTCALCV2_CALCAPPCONTAINER_H
