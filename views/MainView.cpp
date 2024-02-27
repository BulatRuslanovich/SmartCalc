//
// Created by bulatruslanovich on 27/02/2024.
//

#include "MainView.h"

namespace s21 {

MainView::MainView(s21::MathView *mathView, s21::CreditView *creditView,
                   s21::DepositView *depositView, QWidget *parent)
    : QMainWindow(parent), _ui(new Ui::MathView) {
  _ui->setupUi(this);

  if (mathView) {
    mathView->setObjectName(QString::fromUtf8("MathView"));
    _ui->verticalLayout_6->addWidget(mathView);
  }

  if (creditView) {
    creditView->setObjectName(QString::fromUtf8("CreditView"));
    _ui->verticalLayout_7->addWidget(creditView);
  }

  if (depositView) {
    mathView->setObjectName(QString::fromUtf8("DepositView"));
    _ui->verticalLayout_8->addWidget(depositView);
  }
}

MainView::~MainView() { delete _ui; }

} // namespace s21