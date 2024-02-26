//
// Created by bulatruslanovich on 25/02/2024.
//

#include "CreditView.h"

#include <QMessageBox>

#include "ViewHelpers.h"
//#include "../ui/ui_CreditView.h"


namespace s21 {

CreditView::CreditView(ICreditController* controller, ICreditModelPublic* model,
                       QWidget* parent) : QWidget(parent), _ui(new Ui::CreditView), _controller(controller), _model(model) {
  _ui->setupUi(this);
  SetupCreditButtons();
  _model->AddObserver(static_cast<IObserverCreditUpdate*>(this));
  _model->AddObserver(static_cast<IObserverCreditError*>(this));
  ResetResult();
}

CreditView::~CreditView() {
  _model->RemoveObserver(static_cast<IObserverCreditUpdate*>(this));
  _model->RemoveObserver(static_cast<IObserverCreditError*>(this));
  delete _ui;
}

void CreditView::Update() {
  UpdateInputFieldsFromModel();
  UpdateResultFromModel();
}

void CreditView::Error(const std::pair<std::string, std::string>& error) {
  UpdateResultFromModel();

  auto errorText = QString::fromStdString(error.second);

  if (error.first == "sum") {
    SetCreditSumError(errorText);
  } else if (error.first == "months") {
    SetCreditMonthsError(errorText);
  } else if (error.first == "interest") {
    SetCreditInterestError(errorText);
  } else {
    QMessageBox::critical(0, "Ошибка", errorText);
  }
}


void CreditView::SetupCreditButtons() {
  connect(_ui->creditCalcButton, SIGNAL(clicked()), this, SLOT(HandleCreditButton()));

  connect(_ui->creditSumEdit, SIGNAL(textChanged(QString)), this, SLOT(HandleCreditSumChanged()));
  connect(_ui->creditMonthEdit, SIGNAL(textChanged(QString)), this, SLOT(HandleCreditMonthsChanged()));
  connect(_ui->creditInterestEdit, SIGNAL(textChanged(QString)), this, SLOT(HandleCreditInterestChanged()));


}

void CreditView::UpdateInputFieldsFromModel() {
  CreditType type = _model->GetType();

  if (type == Differentiated) {
    _ui->differentiatedRadio->setChecked(true);
  } else {
    _ui->annuityRadio->setChecked(true);
  }

  _ui->creditSumEdit->setText(ViewHelpers::DoubleToQStr(_model->GetSum(), 2));

  int months = _model->GetMonths();

  if (_ui->creditPeriodBox->currentIndex() == 1) {
    months /= 12;
  }

  _ui->creditMonthEdit->setText(QString::number(months));
  _ui->creditInterestEdit->setText(ViewHelpers::DoubleToQStr(_model->GetInterest(), 2));
}

void CreditView::UpdateResultFromModel() {
  if (_model->IsCalc()) {
    double monthPayMin = _model->GetMin();
    double monthPayMax = _model->GetMax();
    double overPay = _model->GetOver();
    double total = _model->GetTotal();

    if (monthPayMax == monthPayMin) {
      _ui->creditRes1->setText(QString::number(monthPayMin, 'f', 2));
    } else {
      _ui->creditRes1->setText(QString::number(monthPayMax, 'f', 2) + "..." +
                               QString::number(monthPayMin, 'f', 2));
    }

    _ui->creditRes2->setText(QString::number(overPay, 'f', 2));
    _ui->creditRes3->setText(QString::number(total, 'f', 2));
  } else {
    ResetResult();
  }
}

void CreditView::ResetResult() {
  _ui->creditRes1->setText("-");
  _ui->creditRes2->setText("-");
  _ui->creditRes3->setText("-");
}

void CreditView::SetCreditSumError(const QString& error) {
  ResetResult();
  _ui->creditSumEdit->setStyleSheet("border: 1px solid red; color: red;");
  QMessageBox::critical(0, "Ошибка", error);
}

void CreditView::SetCreditMonthsError(const QString& error) {
  ResetResult();
  _ui->creditMonthEdit->setStyleSheet("border: 1px solid red; color: red;");
  QMessageBox::critical(0, "Ошибка", error);
}

void CreditView::SetCreditInterestError(const QString& error) {
  ResetResult();
  _ui->creditInterestEdit->setStyleSheet("border: 1px solid red; color: red;");
  QMessageBox::critical(0, "Ошибка", error);
}

void CreditView::ResetCreditSumError() {
  _ui->creditSumEdit->setStyleSheet("");
}

void CreditView::ResetCreditMonthsError() {
  _ui->creditMonthEdit->setStyleSheet("");
}

void CreditView::ResetCreditInterestError() {
  _ui->creditInterestEdit->setStyleSheet("");
}

void CreditView::ResetAllErrors() {
  ResetCreditInterestError();
  ResetCreditMonthsError();
  ResetCreditSumError();
}

void CreditView::HandleCreditButton() {
  ResetResult();

  bool isCorrectInput;

  double sum = _ui->creditSumEdit->text().replace(",", ".").toDouble(&isCorrectInput);

  if (!isCorrectInput) {
    SetCreditSumError("С полем суммы какие приколы 0_0");
    return;
  }

  int months = _ui->creditMonthEdit->text().replace(",", ".").toInt(&isCorrectInput);

  if (!isCorrectInput) {
    SetCreditMonthsError("Полю месяцов не очень хорошо -_-");
    return;
  }

  int interest = _ui->creditInterestEdit->text().replace(",", ".").toDouble(&isCorrectInput);

  if (!isCorrectInput) {
    SetCreditInterestError("Проценты - это от 0 до 100, если что 0_0");
    return;
  }

  sum = QString::number(sum, 'f', 2).toDouble();
  interest = QString::number(interest, 'f', 2).toDouble();
  CreditType type;

  if (_ui->differentiatedRadio->isChecked()) {
    type = Differentiated;
  } else {
    type = Annuity;
  }

  _controller->SetCreditSum(sum);
  _controller->SetCreditMonths(months);
  _controller->SetCreditInterest(interest);
  _controller->SetCreditType(type);

  _controller->CalculateCredit();
}

void CreditView::HandleCreditSumChanged() {
  ResetCreditSumError();
}

void CreditView::HandleCreditMonthsChanged() {
  ResetCreditMonthsError();
}

void CreditView::HandleCreditInterestChanged() {
  ResetCreditInterestError();
}

}  // namespace s21