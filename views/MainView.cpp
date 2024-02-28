//
// Created by bulatruslanovich on 27/02/2024.
//

#include "MainView.h"
#include "../ui/ui_MainView.h"
namespace s21 {

MainView::MainView(s21::MathView *mathView, s21::CreditView *creditView,
                   s21::DepositView *depositView, QWidget *parent)
    : QMainWindow(parent), _ui(new Ui::MainView) {
  _ui->setupUi(this);

  if (mathView) {
    mathView->setObjectName(QString::fromUtf8("MathView"));
    _ui->MainCalcWindow->addWidget(mathView);
  }

  if (creditView) {
    creditView->setObjectName(QString::fromUtf8("CreditView"));
    _ui->CreditCalcWindow->addWidget(creditView);
  }

  if (depositView) {
    mathView->setObjectName(QString::fromUtf8("DepositView"));
    _ui->DepositCalcWindow->addWidget(depositView);
  }
}

MainView::~MainView() { delete _ui; }

} // namespace s21