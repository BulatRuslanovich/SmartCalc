//
// Created by bulatruslanovich on 27/02/2024.
//

#include "DepositView.h"

#include <QDateEdit>
#include <QMessageBox>

#include "../ui/ui_DepositView.h"
#include "ViewHelpers.h"

namespace s21 {

DepositView::DepositView(IDepositController *controller,
                         IDepositModelPublic *model, QWidget *parent)
    : QWidget(parent),
      _ui(new Ui::DepositView),
      _controller(controller),
      _model(model) {

  _ui->setupUi(this);
  ViewHelpers::SetupDateEdit(_ui->depositStartDateEdit);
  _ui->depositTypesCombo->setCurrentIndex(1);
  SetupDepositButtons();
  _model->AddObserver(static_cast<IObserverDepositUpdate *>(this));
  _model->AddObserver(static_cast<IObserverDepositError *>(this));
  ResetResult();
}

DepositView::~DepositView() { delete _ui; }

void DepositView::Update() {
  UpdateInputFieldsFromModel();
  UpdateResultFromModel();
}

void DepositView::Error(const std::pair<std::string, std::string> &error) {
  UpdateResultFromModel();
  auto errorText = QString::fromStdString(error.second);
  if (error.first == "sum") {
    SetDepositSumError(errorText);
  } else if (error.first == "months") {
    SetDepositMonthsError(errorText);
  } else if (error.first == "interest") {
    SetDepositInterestError(errorText);
//  } else if (error.first == "tax") {
//    SetDepositTaxError(errorText);
  } else if (error.first == "date_start") {
    SetDepositDateStartError(errorText);
  } else if (error.first == "replenishment_date") {
    SetDepositAddMoneyError(errorText);
  } else if (error.first == "replenishment_sum") {
    SetDepositAddMoneyError(errorText);
  } else if (error.first == "withdrawal_date") {
    SetDepositSubMoneyError(errorText);
  } else if (error.first == "withdrawal_sum") {
    SetDepositSubMoneyError(errorText);
  } else {
    QMessageBox::critical(0, "Ошибка", errorText);
  }
}

void DepositView::SetupDepositButtons() {
  // Слот для обработки "Рассчитать" в депозитном калькуляторе
  connect(_ui->depositCalcButton, SIGNAL(clicked()), this,
          SLOT(HandleDepositButton()));
  // Слоты для добавления и удаления строк в пополнениях/снятиях депозитного
  // калькулятора
  connect(_ui->addAddingButton, SIGNAL(clicked()), this,
          SLOT(AddOperationReplenishment()));
  connect(_ui->addSubingButton, SIGNAL(clicked()), this,
          SLOT(AddOperationWithdrawal()));
  // Слоты для обработки изменений в текстовых полях депозитного калькулятора
  connect(_ui->depositSumEdit, SIGNAL(textChanged(QString)), this,
          SLOT(HandleDepositSumChanged()));
  connect(_ui->depositMonthEdit, SIGNAL(textChanged(QString)), this,
          SLOT(HandleDepositMonthsChanged()));
  connect(_ui->depositInterestEdit, SIGNAL(textChanged(QString)), this,
          SLOT(HandleDepositInterestChanged()));
//  connect(_ui->depositTaxEdit, SIGNAL(textChanged(QString)), this,
//          SLOT(HandleDepositTaxChanged()));
  connect(_ui->depositStartDateEdit, SIGNAL(dateChanged(const QDate)), this,
          SLOT(HandleDepositDateStartChanged()));
}

void DepositView::UpdateInputFieldsFromModel() {
  // Обновляем данные для расчета по данным модели
  _ui->depositSumEdit->setText(
      ViewHelpers::DoubleToQStr(_model->GetDepositSum(), 2));

  int months = _model->GetDepositMonths();
  if (_ui->depositPeriodBox->currentIndex() == 1) {
    months = months / 12;
  }

  _ui->depositMonthEdit->setText(QString::number(months));
  _ui->depositInterestEdit->setText(ViewHelpers::DoubleToQStr(_model->GetDepositInterest(), 2));

//  _ui->depositTaxEdit->setText(ViewHelpers::DoubleToQStr(_model->GetTax(), 2));
 

  bool capitalization = _model->GetDepositCapitalizationType();

  if (capitalization) {
    _ui->depositCapitalizationCheckBox->setChecked(true);
  } else {
    _ui->depositCapitalizationCheckBox->setChecked(false);
  }

  IDepositType type =_model->GetDepositType();

  if (type == Day) {
    _ui->depositTypesCombo->setCurrentIndex(0);
  } else if (type == Month) {
    _ui->depositTypesCombo->setCurrentIndex(1);
  } else {
    _ui->depositTypesCombo->setCurrentIndex(2);
  }
}

void DepositView::UpdateResultFromModel() {
  // Обновляем результаты расчета по данным модели
  if (_model->IsDepositCalc()) {
    _ui->depositResult1->setText(
        QString::number(_model->GetInterestPayment(), 'f', 2));
    _ui->depositResult2->setText(
        QString::number(_model->GetTaxPayment(), 'f', 2));
    _ui->depositResult3->setText(
        QString::number(_model->GetTotalPayment(), 'f', 2));
    _ui->depositResult4->setText(QString::number(_model->GetInterestPayment() - _model->GetTaxPayment(), 'f', 2));
  } else {
    ResetResult();
  }
}

void DepositView::ResetResult() {
  _ui->depositResult1->setText("-");
  _ui->depositResult2->setText("-");
  _ui->depositResult3->setText("-");
  _ui->depositResult4->setText("-");
}

bool DepositView::OperationsReplenishmentHandle() {
  for (int i = 0; i < _ui->depositAddMoneyLayout->count(); i++) {
    bool isCorrectAddMoney;

    QHBoxLayout *line = qobject_cast<QHBoxLayout *>(_ui->depositAddMoneyLayout->itemAt(i)->layout());
    QLineEdit *lineLineEdit = qobject_cast<QLineEdit *>(line->itemAt(0)->widget());
    QDateEdit *lineDateEdit = qobject_cast<QDateEdit *>(line->itemAt(1)->widget());

    int year = lineDateEdit->date().toString("yyyy").toInt(&isCorrectAddMoney);
    
    if (!isCorrectAddMoney) {
      SetDepositAddMoneyError(
          "Дата пополнения содержит некорректное значение!");
      
      return false;
    }

    int month =lineDateEdit->date().toString("MM").toInt(&isCorrectAddMoney);

    if (!isCorrectAddMoney) {
      SetDepositAddMoneyError("Дата пополнения содержит некорректное значение!");
      
      return false;
    }

    int day = lineDateEdit->date().toString("dd").toInt(&isCorrectAddMoney);

    if (!isCorrectAddMoney) {
      SetDepositAddMoneyError(
          "Дата пополнения содержит некорректное значение!");
      return false;
    }

    double sum = lineLineEdit->text().replace(",", ".").toDouble(&isCorrectAddMoney);

    if (!isCorrectAddMoney) {
      SetDepositAddMoneyError(
          "Сумма пополнения содержит некорректное значение!");
      return false;
    }

    if (sum <= 0.0) {
      SetDepositAddMoneyError("Сумма пополнения должна быть больше 0!");
      return false;
    }

    sum = QString::number(sum, 'f', 2).toDouble();
    _controller->AddDepositOperationReplenishment(year, month, day, sum);
  }

  return true;
}

bool DepositView::OperationsWithdrawalHandle() {
  for (int i = 0; i < _ui->depositSubMoneyLayout->count(); i++) {
    bool isCorrectAddMoney;

    QHBoxLayout *line = qobject_cast<QHBoxLayout *>(
        _ui->depositSubMoneyLayout->itemAt(i)->layout());
    QLineEdit *lineLineEdit =
        qobject_cast<QLineEdit *>(line->itemAt(0)->widget());
    QDateEdit *lineDateEdit =
        qobject_cast<QDateEdit *>(line->itemAt(1)->widget());

    int year =
        lineDateEdit->date().toString("yyyy").toInt(&isCorrectAddMoney);

    if (!isCorrectAddMoney) {
      SetDepositSubMoneyError("Дата снятия содержит некорректное значение!");
      return false;
    }

    int month =
        lineDateEdit->date().toString("MM").toInt(&isCorrectAddMoney);

    if (!isCorrectAddMoney) {
      SetDepositSubMoneyError("Дата снятия содержит некорректное значение!");
      return false;
    }

    int day =
        lineDateEdit->date().toString("dd").toInt(&isCorrectAddMoney);

    if (!isCorrectAddMoney) {
      SetDepositSubMoneyError("Дата снятия содержит некорректное значение!");
      return false;
    }

    double sum = lineLineEdit->text().replace(",", ".").toDouble(
        &isCorrectAddMoney);

    if (!isCorrectAddMoney) {
      SetDepositSubMoneyError("Сумма снятия содержит некорректное значение!");
      return false;
    }

    if (sum <= 0.0) {
      SetDepositSubMoneyError("Сумма снятия должна быть больше 0!");
      return false;
    }

    sum = QString::number(sum, 'f', 2).toDouble();
    _controller->AddDepositOperationWithdrawal(year, month, day, sum);
  }

  return true;
}

void DepositView::AddOperationLine(QBoxLayout *layout) {
  QHBoxLayout *line = new QHBoxLayout;

  line->addWidget(new QLineEdit);
  QDateEdit *dateEdit = new QDateEdit;
  ViewHelpers::SetupDateEdit(dateEdit);
  line->addWidget(dateEdit);
  QPushButton *button = new QPushButton;
  QPixmap pix(":/images/delete.png");
  QIcon icon(pix);
  button->resize(QSize(25, 25));
  button->setIcon(icon);
  button->setIconSize(QSize(16, 16));
  line->addWidget(button);

  layout->addLayout(line);

  connect(button, SIGNAL(clicked()), this, SLOT(DeleteOperationLine()));
}

void DepositView::SetDepositSumError(const QString &errorText) {
  ResetResult();
  _ui->depositSumEdit->setStyleSheet("border: 1px solid red; color: red;");
  _ui->depositSumEdit->setFocus();
  QMessageBox::critical(0, "Ошибка", errorText);
}

void DepositView::SetDepositMonthsError(const QString &errorText) {
  ResetResult();
  _ui->depositMonthEdit->setStyleSheet("border: 1px solid red; color: red;");
  _ui->depositMonthEdit->setFocus();
  QMessageBox::critical(0, "Ошибка", errorText);
}

void DepositView::SetDepositInterestError(const QString &errorText) {
  ResetResult();
  _ui->depositInterestEdit->setStyleSheet(
      "border: 1px solid red; color: red;");
  _ui->depositInterestEdit->setFocus();
  QMessageBox::critical(0, "Ошибка", errorText);
}

//void DepositView::SetDepositTaxError(const QString &errorText) {
//  ResetResult();
//  _ui->depositTaxEdit->setStyleSheet("border: 1px solid red; color: red;");
//  _ui->depositTaxEdit->setFocus();
//  QMessageBox::critical(0, "Ошибка", errorText);
//}

void DepositView::SetDepositDateStartError(const QString &errorText) {
  ResetResult();
  _ui->depositStartDateEdit->setStyleSheet(
      "border: 1px solid red; color: red;");
  _ui->depositStartDateEdit->setFocus();
  QMessageBox::critical(0, "Ошибка", errorText);
}

void DepositView::SetDepositAddMoneyError(const QString &errorText) {
  ResetResult();
  QMessageBox::critical(0, "Ошибка", errorText);
}

void DepositView::SetDepositSubMoneyError(const QString &errorText) {
  ResetResult();
  QMessageBox::critical(0, "Ошибка", errorText);
}

void DepositView::ResetDepositSumError() {
  _ui->depositSumEdit->setStyleSheet("");
}

void DepositView::ResetDepositMonthsError() {
  _ui->depositMonthEdit->setStyleSheet("");
}

void DepositView::ResetDepositInterestError() {
  _ui->depositInterestEdit->setStyleSheet("");
}

//void DepositView::ResetDepositTaxError() {
//  _ui->depositTaxEdit->setStyleSheet("");
//}

void DepositView::ResetDepositDateStartError() {
  _ui->depositStartDateEdit->setStyleSheet("");
}

void DepositView::ResetAllErrors() {
  ResetDepositSumError();
  ResetDepositMonthsError();
  ResetDepositInterestError();
//  ResetDepositTaxError();
  ResetDepositDateStartError();
}

void DepositView::HandleDepositButton() {
  ResetAllErrors();

  bool isCorrectInput;

  double sum =
      _ui->depositSumEdit->text().replace(",", ".").toDouble(&isCorrectInput);
  
  if (!isCorrectInput) {
    SetDepositSumError("Поле \"Сумма вклада\" содержит некорректное значение!");
    return;
  }

  int months = _ui->depositMonthEdit->text().toInt(&isCorrectInput);
  
  if (!isCorrectInput) {
    SetDepositMonthsError(
        "Поле \"Срок размещения\" содержит некорректное значение!");
    return;
  }

  double interest =
      _ui->depositInterestEdit->text().replace(",", ".").toDouble(
          &isCorrectInput);
  
  if (!isCorrectInput) {
    SetDepositInterestError(
        "Поле \"Процентная ставка\" содержит некорректное значение!");
    return;
  }

//  double tax =
//      _ui->depositTaxEdit->text().replace(",", ".").toDouble(&isCorrectInput);
//
//  if (!isCorrectInput) {
//    SetDepositTaxError(
//        "Поле \"Налоговая ставка\" содержит некорректное значение!");
//    return;
//  }

  int deposit_year =
      _ui->depositStartDateEdit->date().toString("yyyy").toInt(
          &isCorrectInput);
  
  if (!isCorrectInput) {
    SetDepositDateStartError(
        "Поле \"Начало срока\" содержит некорректное значение!");
    return;
  }

  int deposit_month =
      _ui->depositStartDateEdit->date().toString("MM").toInt(&isCorrectInput);
  
  if (!isCorrectInput) {
    SetDepositDateStartError(
        "Поле \"Начало срока\" содержит некорректное значение!");
    return;
  }

  int deposit_day =
      _ui->depositStartDateEdit->date().toString("dd").toInt(&isCorrectInput);
  
  if (!isCorrectInput) {
    SetDepositDateStartError(
        "Поле \"Начало срока\" содержит некорректное значение!");
    return;
  }

  IDepositType type;
  bool capitalization;

  if (_ui->depositPeriodBox->currentIndex() == 1) {
    months = months * 12;
  }

  if (_ui->depositTypesCombo->currentIndex() == 0) {
    type = Day;
  } else if (_ui->depositTypesCombo->currentIndex() == 1) {
    type = Month;
  } else {
    type = End;
  }
  
  capitalization = _ui->depositCapitalizationCheckBox->isChecked();

  _controller->ClearDepositOperationsList();

  bool success = OperationsReplenishmentHandle();
  
  if (success) {
    success = OperationsWithdrawalHandle();
  }

  if (!success) {
    return;
  }

  sum = QString::number(sum, 'f', 2).toDouble();
  months = QString::number(months, 'f', 2).toDouble();
  interest = QString::number(interest, 'f', 2).toDouble();
//  tax = QString::number(tax, 'f', 2).toDouble();

  _controller->SetDepositSum(sum);
  _controller->SetDepositPeriod(months);
  _controller->SetDepositInterest(interest);
//  _controller->SetDepositTax(tax);
  _controller->SetDepositPeriodicityType(type);
  _controller->SetDepositCapitalization(capitalization);
  _controller->SetDepositDateStart(deposit_year, deposit_month, deposit_day);
  _controller->CalculateDeposit();
}

void DepositView::AddOperationReplenishment() {
  AddOperationLine(_ui->depositAddMoneyLayout);
}

void DepositView::AddOperationWithdrawal() {
  AddOperationLine(_ui->depositSubMoneyLayout);
}

void DepositView::DeleteOperationLine() {
  QPushButton *deleteButton = qobject_cast<QPushButton *>(sender());

  QHBoxLayout *line = qobject_cast<QHBoxLayout *>(ViewHelpers::FindParentLayout(deleteButton));

  QLineEdit *deleteLineEdit =
      qobject_cast<QLineEdit *>(line->itemAt(0)->widget());
  QDateEdit *deleteLineDate =
      qobject_cast<QDateEdit *>(line->itemAt(1)->widget());

  deleteButton->hide();
  deleteLineEdit->hide();
  deleteLineDate->hide();

  delete deleteButton;
  delete deleteLineEdit;
  delete deleteLineDate;
  delete line;
}

void DepositView::HandleDepositSumChanged() { ResetDepositSumError(); }

void DepositView::HandleDepositMonthsChanged() { ResetDepositMonthsError(); }

void DepositView::HandleDepositInterestChanged() {
  ResetDepositInterestError();
}

//void DepositView::HandleDepositTaxChanged() { ResetDepositTaxError(); }

void DepositView::HandleDepositDateStartChanged() {
  ResetDepositDateStartError();
}

}  // namespace s21