//
// Created by bulatruslanovich on 27/02/2024.
//

#include "DepositView.h"

#include <QDateEdit>
#include <QMessageBox>

//#include "../ui/_uideposit_view.h"
#include "ViewHelpers.h"

namespace s21 {

DepositView::DepositView(IDepositController *controller,
                         IDepositModelPublic *model, QWidget *parent)
    : QWidget(parent),
      _ui(new Ui::DepositView),
      _controller(controller),
      _model(model) {

  _ui->setupUi(this);
  ViewHelpers::SetupDateEdit(_ui->deposit_start_date_edit);
  _ui->deposit_types_combo->setCurrentIndex(1);
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
  auto error_text = QString::fromStdString(error.second);
  if (error.first == "sum") {
    SetDepositSumError(error_text);
  } else if (error.first == "months") {
    SetDepositMonthsError(error_text);
  } else if (error.first == "interest") {
    SetDepositInterestError(error_text);
  } else if (error.first == "tax") {
    SetDepositTaxError(error_text);
  } else if (error.first == "date_start") {
    SetDepositDateStartError(error_text);
  } else if (error.first == "replenishment_date") {
    SetDepositAddMoneyError(error_text);
  } else if (error.first == "replenishment_sum") {
    SetDepositAddMoneyError(error_text);
  } else if (error.first == "withdrawal_date") {
    SetDepositSubMoneyError(error_text);
  } else if (error.first == "withdrawal_sum") {
    SetDepositSubMoneyError(error_text);
  } else {
    QMessageBox::critical(0, "Ошибка", error_text);
  }
}

void DepositView::SetupDepositButtons() {
  // Слот для обработки "Рассчитать" в депозитном калькуляторе
  connect(_ui->deposit_calculate_button, SIGNAL(clicked()), this,
          SLOT(HandleDepositButton()));
  // Слоты для добавления и удаления строк в пополнениях/снятиях депозитного
  // калькулятора
  connect(_ui->addAddingButton, SIGNAL(clicked()), this,
          SLOT(AddOperationReplenishment()));
  connect(_ui->AddSubingButton, SIGNAL(clicked()), this,
          SLOT(AddOperationWithdrawal()));
  // Слоты для обработки изменений в текстовых полях депозитного калькулятора
  connect(_ui->deposit_sum_edit, SIGNAL(textChanged(QString)), this,
          SLOT(HandleDepositSumChanged()));
  connect(_ui->deposit_months_edit, SIGNAL(textChanged(QString)), this,
          SLOT(HandleDepositMonthsChanged()));
  connect(_ui->deposit_interest_edit, SIGNAL(textChanged(QString)), this,
          SLOT(HandleDepositInterestChanged()));
  connect(_ui->deposit_tax_edit, SIGNAL(textChanged(QString)), this,
          SLOT(HandleDepositTaxChanged()));
  connect(_ui->deposit_start_date_edit, SIGNAL(dateChanged(const QDate)), this,
          SLOT(HandleDepositDateStartChanged()));
}

void DepositView::UpdateInputFieldsFromModel() {
  // Обновляем данные для расчета по данным модели
  _ui->deposit_sum_edit->setText(
      ViewHelpers::DoubleToQStr(_model->GetSum(), 2));

  int months = _model->GetMonths();
  if (_ui->deposit_period_box->currentIndex() == 1) {
    months = months / 12;
  }

  _ui->deposit_months_edit->setText(QString::number(months));
  _ui->deposit_interest_edit->setText(ViewHelpers::DoubleToQStr(_model->GetInterest(), 2));

  _ui->deposit_tax_edit->setText(ViewHelpers::DoubleToQStr(_model->GetTax(), 2));
 

  bool capitalization = _model->GetDepositCapitalizationType();

  if (capitalization) {
    _ui->deposit_capitalization_check_box->setChecked(true);
  } else {
    _ui->deposit_capitalization_check_box->setChecked(false);
  }

  DepositType type =_model->GetDepositType();

  if (type == Day) {
    _ui->deposit_types_combo->setCurrentIndex(0);
  } else if (type == Month) {
    _ui->deposit_types_combo->setCurrentIndex(1);
  } else {
    _ui->deposit_types_combo->setCurrentIndex(2);
  }
}

void DepositView::UpdateResultFromModel() {
  // Обновляем результаты расчета по данным модели
  if (_model->IsCalc()) {
    _ui->deposit_result_1->setText(
        QString::number(_model->GetInterestPayment(), 'f', 2));
    _ui->deposit_result_2->setText(
        QString::number(_model->GetTaxPayment(), 'f', 2));
    _ui->deposit_result_3->setText(
        QString::number(_model->GetTotalPayment(), 'f', 2));
  } else {
    ResetResult();
  }
}

void DepositView::ResetResult() {
  _ui->deposit_result_1->setText("-");
  _ui->deposit_result_2->setText("-");
  _ui->deposit_result_3->setText("-");
}

bool DepositView::OperationsReplenishmentHandle() {
  for (int i = 0; i < _ui->deposit_add_money_layout->count(); i++) {
    bool isCorrectAddMoney;

    QHBoxLayout *line = qobject_cast<QHBoxLayout *>(_ui->deposit_add_money_layout->itemAt(i)->layout());
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
  for (int i = 0; i < _ui->deposit_sub_money_layout->count(); i++) {
    bool isCorrectAddMoney;

    QHBoxLayout *line = qobject_cast<QHBoxLayout *>(
        _ui->deposit_sub_money_layout->itemAt(i)->layout());
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
  view_helpers::SetupDateEdit(dateEdit);
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

void DepositView::SetDepositSumError(const QString &error_text) {
  ResetResult();
  _ui->deposit_sum_edit->setStyleSheet("border: 1px solid red; color: red;");
  _ui->deposit_sum_edit->setFocus();
  QMessageBox::critical(0, "Ошибка", error_text);
}

void DepositView::SetDepositMonthsError(const QString &error_text) {
  ResetResult();
  _ui->deposit_months_edit->setStyleSheet("border: 1px solid red; color: red;");
  _ui->deposit_months_edit->setFocus();
  QMessageBox::critical(0, "Ошибка", error_text);
}

void DepositView::SetDepositInterestError(const QString &error_text) {
  ResetResult();
  _ui->deposit_interest_edit->setStyleSheet(
      "border: 1px solid red; color: red;");
  _ui->deposit_interest_edit->setFocus();
  QMessageBox::critical(0, "Ошибка", error_text);
}

void DepositView::SetDepositTaxError(const QString &error_text) {
  ResetResult();
  _ui->deposit_tax_edit->setStyleSheet("border: 1px solid red; color: red;");
  _ui->deposit_tax_edit->setFocus();
  QMessageBox::critical(0, "Ошибка", error_text);
}

void DepositView::SetDepositDateStartError(const QString &error_text) {
  ResetResult();
  _ui->deposit_start_date_edit->setStyleSheet(
      "border: 1px solid red; color: red;");
  _ui->deposit_start_date_edit->setFocus();
  QMessageBox::critical(0, "Ошибка", error_text);
}

void DepositView::SetDepositAddMoneyError(const QString &error_text) {
  ResetResult();
  QMessageBox::critical(0, "Ошибка", error_text);
}

void DepositView::SetDepositSubMoneyError(const QString &error_text) {
  ResetResult();
  QMessageBox::critical(0, "Ошибка", error_text);
}

void DepositView::ResetDepositSumError() {
  _ui->deposit_sum_edit->setStyleSheet("");
}

void DepositView::ResetDepositMonthsError() {
  _ui->deposit_months_edit->setStyleSheet("");
}

void DepositView::ResetDepositInterestError() {
  _ui->deposit_interest_edit->setStyleSheet("");
}

void DepositView::ResetDepositTaxError() {
  _ui->deposit_tax_edit->setStyleSheet("");
}

void DepositView::ResetDepositDateStartError() {
  _ui->deposit_start_date_edit->setStyleSheet("");
}

void DepositView::ResetAllErrors() {
  ResetDepositSumError();
  ResetDepositMonthsError();
  ResetDepositInterestError();
  ResetDepositTaxError();
  ResetDepositDateStartError();
}

void DepositView::HandleDepositButton() {
  ResetAllErrors();

  bool isCorrectInput;

  double sum =
      _ui->deposit_sum_edit->text().replace(",", ".").toDouble(&isCorrectInput);
  
  if (!isCorrectInput) {
    SetDepositSumError("Поле \"Сумма вклада\" содержит некорректное значение!");
    return;
  }

  int months = _ui->deposit_months_edit->text().toInt(&isCorrectInput);
  
  if (!isCorrectInput) {
    SetDepositMonthsError(
        "Поле \"Срок размещения\" содержит некорректное значение!");
    return;
  }

  double interest =
      _ui->deposit_interest_edit->text().replace(",", ".").toDouble(
          &isCorrectInput);
  
  if (!isCorrectInput) {
    SetDepositInterestError(
        "Поле \"Процентная ставка\" содержит некорректное значение!");
    return;
  }

  double tax =
      _ui->deposit_tax_edit->text().replace(",", ".").toDouble(&isCorrectInput);
  
  if (!isCorrectInput) {
    SetDepositTaxError(
        "Поле \"Налоговая ставка\" содержит некорректное значение!");
    return;
  }

  int deposit_year =
      _ui->deposit_start_date_edit->date().toString("yyyy").toInt(
          &isCorrectInput);
  
  if (!isCorrectInput) {
    SetDepositDateStartError(
        "Поле \"Начало срока\" содержит некорректное значение!");
    return;
  }

  int deposit_month =
      _ui->deposit_start_date_edit->date().toString("MM").toInt(&isCorrectInput);
  
  if (!isCorrectInput) {
    SetDepositDateStartError(
        "Поле \"Начало срока\" содержит некорректное значение!");
    return;
  }

  int deposit_day =
      _ui->deposit_start_date_edit->date().toString("dd").toInt(&isCorrectInput);
  
  if (!isCorrectInput) {
    SetDepositDateStartError(
        "Поле \"Начало срока\" содержит некорректное значение!");
    return;
  }

  DepositType type;
  bool capitalization;

  if (_ui->deposit_period_box->currentIndex() == 1) {
    months = months * 12;
  }

  if (_ui->deposit_types_combo->currentIndex() == 0) {
    type = Day;
  } else if (_ui->deposit_types_combo->currentIndex() == 1) {
    type = Month;
  } else {
    type = End;
  }
  
  capitalization = _ui->deposit_capitalization_check_box->isChecked();

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
  tax = QString::number(tax, 'f', 2).toDouble();

  _controller->SetDepositSum(sum);
  _controller->SetDepositPeriod(months);
  _controller->SetDepositInterest(interest);
  _controller->SetDepositTax(tax);
  _controller->SetDepositPeriodicityType(type);
  _controller->SetDepositCapitalization(capitalization);
  _controller->SetDepositDateStart(deposit_year, deposit_month, deposit_day);
  _controller->CalculateDeposit();
}

void DepositView::AddOperationReplenishment() {
  AddOperationLine(_ui->deposit_add_money_layout);
}

void DepositView::AddOperationWithdrawal() {
  AddOperationLine(_ui->deposit_sub_money_layout);
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

void DepositView::HandleDepositTaxChanged() { ResetDepositTaxError(); }

void DepositView::HandleDepositDateStartChanged() {
  ResetDepositDateStartError();
}

}  // namespace s21