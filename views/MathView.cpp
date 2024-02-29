//
// Created by bulatruslanovich on 27/02/2024.
//

#include "MathView.h"
#include <QMessageBox>

#include "../ui/ui_MathView.h"
#include "ViewHelpers.h"

namespace s21 {

MathView::MathView(IMathController* controller,
                   IMathModelPublic* model, GraphView* graphView,
                   QWidget* parent)
    : QWidget(parent),
      _ui(new Ui::MathView),
      _controller(controller),
      _model(model),
      _graphView(graphView) {
  _ui->setupUi(this);
  _graphView->setParent(this);
  _graphView->setWindowFlags(Qt::Window);
  SetupButtons();
  _model->AddObserver(static_cast<IObserverMathUpdate*>(this));
  _model->AddObserver(static_cast<IObserverMathError*>(this));
}

MathView::~MathView() {
  _model->RemoveObserver(static_cast<IObserverMathUpdate*>(this));
  _model->RemoveObserver(static_cast<IObserverMathError*>(this));
  delete _ui;
}

void MathView::Update() { 
  _ui->srcInput->setPlainText(
      QString::fromStdString(_model->GetExpression()));

  if (_model->IsCalc()) {
    _ui->xEdit->setText(ViewHelpers::DoubleToQStr(_model->GetX(), 7));
    SetCalcResult(_model->GetResult());
  } else {
    SetCalcResult("");
  }
  
  _ui->srcInput->setFocus();
  QTextCursor cursor = _ui->srcInput->textCursor();
  cursor.movePosition(QTextCursor::End);
  _ui->srcInput->setTextCursor(cursor);
}

void MathView::Error(const std::pair<std::string, std::string>& error) {
  auto errorText = QString::fromStdString(error.second);
  if (error.first == "input") {
    SetInputValueError(errorText);
  } else if (error.first == "wrong_operator") {
    QMessageBox::critical(0, "Ошибка", errorText);
  } else if (error.first == "graphCalculation") {
    SetInputValueError(errorText);
  } else {
    QMessageBox::critical(0, "Неизвестная ошибка", errorText);
  }
}

void MathView::SetupButtons() {
  // Слот для всех кнопок в калькуляторе
  connect(_ui->calcButton, SIGNAL(clicked()), this, SLOT(HandleCalcButton()));
  connect(_ui->graphButton, SIGNAL(clicked()), this,
          SLOT(HandleGraphButton()));
  connect(_ui->modButton, SIGNAL(clicked()), this, SLOT(HandleButtonMod()));
  connect(_ui->divButton, SIGNAL(clicked()), this, SLOT(HandleButtonDiv()));
  connect(_ui->mulButton, SIGNAL(clicked()), this, SLOT(HandleButtonMul()));
  connect(_ui->subButton, SIGNAL(clicked()), this, SLOT(HandleButtonSub()));
  connect(_ui->addButton, SIGNAL(clicked()), this, SLOT(HandleButtonAdd()));
  connect(_ui->clearButton, SIGNAL(clicked()), this,
          SLOT(HandleClearButton()));
  connect(_ui->lbracketButton, SIGNAL(clicked()), this,
          SLOT(HandleButtonLBracket()));
  connect(_ui->rbracketButton, SIGNAL(clicked()), this,
          SLOT(HandleButtonRBracket()));
  connect(_ui->powButton, SIGNAL(clicked()), this, SLOT(HandleButtonPow()));
  connect(_ui->cosButton, SIGNAL(clicked()), this, SLOT(HandleButtonCos()));
  connect(_ui->sinButton, SIGNAL(clicked()), this, SLOT(HandleButtonSin()));
  connect(_ui->tanButton, SIGNAL(clicked()), this, SLOT(HandleButtonTan()));
  connect(_ui->sqrtButton, SIGNAL(clicked()), this, SLOT(HandleButtonSqrt()));
  connect(_ui->acosButton, SIGNAL(clicked()), this, SLOT(HandleButtonAcos()));
  connect(_ui->atanButton, SIGNAL(clicked()), this, SLOT(HandleButtonAtan()));
  connect(_ui->asinButton, SIGNAL(clicked()), this, SLOT(HandleButtonAsin()));
  connect(_ui->lnButton, SIGNAL(clicked()), this, SLOT(HandleButtonLn()));
  connect(_ui->piButton, SIGNAL(clicked()), this, SLOT(HandleButtonPi()));
  connect(_ui->eButton, SIGNAL(clicked()), this, SLOT(HandleButtonE()));
  connect(_ui->xButton, SIGNAL(clicked()), this, SLOT(HandleButtonX()));
  connect(_ui->logButton, SIGNAL(clicked()), this, SLOT(HandleButtonLog()));
  connect(_ui->num7Button, SIGNAL(clicked()), this, SLOT(HandleButtonNum7()));
  connect(_ui->num8Button, SIGNAL(clicked()), this, SLOT(HandleButtonNum8()));
  connect(_ui->num9Button, SIGNAL(clicked()), this, SLOT(HandleButtonNum9()));
  connect(_ui->num4Button, SIGNAL(clicked()), this, SLOT(HandleButtonNum4()));
  connect(_ui->num5Button, SIGNAL(clicked()), this, SLOT(HandleButtonNum5()));
  connect(_ui->num6Button, SIGNAL(clicked()), this, SLOT(HandleButtonNum6()));
  connect(_ui->num1Button, SIGNAL(clicked()), this, SLOT(HandleButtonNum1()));
  connect(_ui->num2Button, SIGNAL(clicked()), this, SLOT(HandleButtonNum2()));
  connect(_ui->num3Button, SIGNAL(clicked()), this, SLOT(HandleButtonNum3()));
  connect(_ui->num0Button, SIGNAL(clicked()), this, SLOT(HandleButtonNum0()));
  connect(_ui->unMinusButton, SIGNAL(clicked()), this,
          SLOT(HandleButtonUnaryMinus()));
  connect(_ui->pointButton, SIGNAL(clicked()), this,
          SLOT(HandlePointButton()));
  connect(_ui->backspaceButton, SIGNAL(clicked()), this,
          SLOT(HandleBackspaceButton()));
  connect(_ui->pow2Button, SIGNAL(clicked()), this, SLOT(HandleButtonPow2()));
  
  // Слоты для обработки изменений в текстовых полях калькулятора
  connect(_ui->xEdit, SIGNAL(textChanged(QString)), this,
          SLOT(HandleXEditChanged()));
  connect(_ui->srcInput, SIGNAL(textChanged()), this,
          SLOT(HandleInputChanged()));
}

QString MathView::GetViewMathExpression() {
  return _ui->srcInput->toPlainText().remove('\n');
}

void MathView::SetCalcResult(double result) {
  _ui->resultEdit->setText(ViewHelpers::DoubleToQStr(result, 7));
}

void MathView::SetCalcResult(const QString& result) {
  _ui->resultEdit->setText(result);
}

void MathView::SetXValueError(const QString& errorText) {
  SetCalcResult("ERROR");
  _ui->xEdit->setStyleSheet("border: 1px solid red; color: red;");
  QMessageBox::critical(0, "Ошибка", errorText);
}

void MathView::SetInputValueError(const QString& errorText) {
  SetCalcResult("ERROR");
  _ui->srcInput->setStyleSheet("border: 1px solid red; color: red;");
  QMessageBox::critical(0, "Ошибка", errorText);
}

void MathView::ResetXValueError() { _ui->xEdit->setStyleSheet(""); }

void MathView::ResetInputValueError() { _ui->srcInput->setStyleSheet(""); }

void MathView::ResetAllErrors() {
  ResetXValueError();
  ResetInputValueError();
}

void MathView::HandleCalcButton() {
  ResetAllErrors();

  bool xConvertOk;
  double xValue =
      _ui->xEdit->text().replace(",", ".").toDouble(&xConvertOk);
  xValue = QString::number(xValue, 'f', 7).toDouble();

  _controller->SetExpression(GetViewMathExpression());

  if (xConvertOk) {
    _controller->SetX(xValue);
    _controller->CalculateX();
  } else {
    SetXValueError("Поле \"x =\" содержит некорректное значение!");
  }

  if (_graphView->isVisible()) {
    _graphView->UpdateGraphWindow(GetViewMathExpression());
  }
}

void MathView::HandleGraphButton() {
  _controller->SetExpression(GetViewMathExpression());
  _graphView->UpdateGraphWindow(GetViewMathExpression());
}

void MathView::HandleClearButton() { _ui->srcInput->clear(); }

void MathView::HandlePointButton() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd(".");
}

void MathView::HandleBackspaceButton() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionEraseEnd();
}

void MathView::HandleButtonMod() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("mod");
}

void MathView::HandleButtonDiv() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("/");
}

void MathView::HandleButtonMul() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("*");
}

void MathView::HandleButtonSub() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("-");
}
void MathView::HandleButtonAdd() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("+");
}

void MathView::HandleButtonLBracket() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("(");
}

void MathView::HandleButtonRBracket() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd(")");
}

void MathView::HandleButtonPow() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("^");
}

void MathView::HandleButtonPow2() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("^2");
}

void MathView::HandleButtonCos() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("cos");
}

void MathView::HandleButtonSin() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("sin");
}

void MathView::HandleButtonTan() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("tan");
}

void MathView::HandleButtonSqrt() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("sqrt");
}

void MathView::HandleButtonAcos() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("acos");
}

void MathView::HandleButtonAtan() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("atan");
}

void MathView::HandleButtonAsin() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("asin");
}

void MathView::HandleButtonLn() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("ln");
}

void MathView::HandleButtonPi() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("pi");
}

void MathView::HandleButtonE() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("e");
}

void MathView::HandleButtonX() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("x");
}

void MathView::HandleButtonLog() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("log");
}

void MathView::HandleButtonNum0() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("0");
}

void MathView::HandleButtonNum1() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("1");
}

void MathView::HandleButtonNum2() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("2");
}

void MathView::HandleButtonNum3() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("3");
}

void MathView::HandleButtonNum4() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("4");
}

void MathView::HandleButtonNum5() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("5");
}

void MathView::HandleButtonNum6() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("6");
}

void MathView::HandleButtonNum7() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("7");
}

void MathView::HandleButtonNum8() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("8");
}

void MathView::HandleButtonNum9() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("9");
}

void MathView::HandleButtonUnaryMinus() {
  _controller->SetExpression(GetViewMathExpression());
  _controller->ExpressionAdd("~");
}

void MathView::HandleXEditChanged() { ResetXValueError(); }

void MathView::HandleInputChanged() {
  ResetInputValueError();
  SetCalcResult("");
}

}  // namespace s21