//
// Created by bulatruslanovich on 28/02/2024.
//

#include "CalcAppContainer.h"

namespace s21 {
CalcAppContainer::CalcAppContainer() {
  _model = new MasterModel;

  _graphController = new GraphController(_model);
  _graphView = new GraphView(_graphController, _model);

  _mathController = new MathController(_model);
  _mathView = new MathView(_mathController, _model, _graphView);

  _model->AddObserver(static_cast<s21::IObserverGraphError*>(_mathView));

  _creditController = new s21::CreditController(_model);
  _creditView = new CreditView(_creditController, _model);

  _depositController = new s21::DepositController(_model);
  _depositView = new DepositView(_depositController, _model);

  _mainView = new s21::MainView(_mathView, _creditView, _depositView);
}
CalcAppContainer::~CalcAppContainer() {
  _model->RemoveObserver(static_cast<s21::IObserverGraphError*>(_mathView));
  delete _mainView;
  delete _depositController;
  delete _creditController;
  delete _graphController;
  delete _mathController;
  delete _model;
}

void CalcAppContainer::Init() {
  _mainView->show();
}

}  // namespace s21