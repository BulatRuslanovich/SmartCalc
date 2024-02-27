//
// Created by bulatruslanovich on 27/02/2024.
//

#include "MathView.h"
#include <QMessageBox>

//#include "../ui/_uimath_view.h"
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
  _ui->src_input->setPlainText(
      QString::fromStdString(_model->GetExpression()));

  if (_model->IsCalc()) {
    _ui->x_edit->setText(ViewHelpers::DoubleToQStr(_model->GetX(), 7));
    SetCalcResult(_model->GetResult());
  } else {
    SetCalcResult("");
  }
  
  _ui->src_input->setFocus();
  QTextCursor cursor = _ui->src_input->textCursor();
  cursor.movePosition(QTextCursor::End);
  _ui->src_input->setTextCursor(cursor);
}

void MathView::Error(const std::pair<std::string, std::string>& error) {
  auto errorText = QString::fromStdString(error.second);
  if (error.first == "input") {
    SetInputValueError(errorText);
  } else if (error.first == "wrong_operator") {
    QMessageBox::critical(0, "Ошибка", errorText);
  } else if (error.first == "graph_calculation") {
    SetInputValueError(errorText);
  } else {
    QMessageBox::critical(0, "Неизвестная ошибка", errorText);
  }
}

void MathView::SetupButtons() {
  // Слот для всех кнопок в калькуляторе
  connect(_ui->calc_button, SIGNAL(clicked()), this, SLOT(HandleCalcButton()));
  connect(_ui->graph_button, SIGNAL(clicked()), this,
          SLOT(HandleGraphButton()));
  connect(_ui->mod_button, SIGNAL(clicked()), this, SLOT(HandleButtonMod()));
  connect(_ui->div_button, SIGNAL(clicked()), this, SLOT(HandleButtonDiv()));
  connect(_ui->mul_button, SIGNAL(clicked()), this, SLOT(HandleButtonMul()));
  connect(_ui->sub_button, SIGNAL(clicked()), this, SLOT(HandleButtonSub()));
  connect(_ui->add_button, SIGNAL(clicked()), this, SLOT(HandleButtonAdd()));
  connect(_ui->clear_button, SIGNAL(clicked()), this,
          SLOT(HandleClearButton()));
  connect(_ui->lbracket_button, SIGNAL(clicked()), this,
          SLOT(HandleButtonLBracket()));
  connect(_ui->rbracket_button, SIGNAL(clicked()), this,
          SLOT(HandleButtonRBracket()));
  connect(_ui->pow_button, SIGNAL(clicked()), this, SLOT(HandleButtonPow()));
  connect(_ui->cos_button, SIGNAL(clicked()), this, SLOT(HandleButtonCos()));
  connect(_ui->sin_button, SIGNAL(clicked()), this, SLOT(HandleButtonSin()));
  connect(_ui->tan_button, SIGNAL(clicked()), this, SLOT(HandleButtonTan()));
  connect(_ui->sqrt_button, SIGNAL(clicked()), this, SLOT(HandleButtonSqrt()));
  connect(_ui->acos_button, SIGNAL(clicked()), this, SLOT(HandleButtonAcos()));
  connect(_ui->atan_button, SIGNAL(clicked()), this, SLOT(HandleButtonAtan()));
  connect(_ui->asin_button, SIGNAL(clicked()), this, SLOT(HandleButtonAsin()));
  connect(_ui->ln_button, SIGNAL(clicked()), this, SLOT(HandleButtonLn()));
  connect(_ui->pi_button, SIGNAL(clicked()), this, SLOT(HandleButtonPi()));
  connect(_ui->e_button, SIGNAL(clicked()), this, SLOT(HandleButtonE()));
  connect(_ui->x_button, SIGNAL(clicked()), this, SLOT(HandleButtonX()));
  connect(_ui->log_button, SIGNAL(clicked()), this, SLOT(HandleButtonLog()));
  connect(_ui->num7_button, SIGNAL(clicked()), this, SLOT(HandleButtonNum7()));
  connect(_ui->num8_button, SIGNAL(clicked()), this, SLOT(HandleButtonNum8()));
  connect(_ui->num9_button, SIGNAL(clicked()), this, SLOT(HandleButtonNum9()));
  connect(_ui->num4_button, SIGNAL(clicked()), this, SLOT(HandleButtonNum4()));
  connect(_ui->num5_button, SIGNAL(clicked()), this, SLOT(HandleButtonNum5()));
  connect(_ui->num6_button, SIGNAL(clicked()), this, SLOT(HandleButtonNum6()));
  connect(_ui->num1_button, SIGNAL(clicked()), this, SLOT(HandleButtonNum1()));
  connect(_ui->num2_button, SIGNAL(clicked()), this, SLOT(HandleButtonNum2()));
  connect(_ui->num3_button, SIGNAL(clicked()), this, SLOT(HandleButtonNum3()));
  connect(_ui->num0_button, SIGNAL(clicked()), this, SLOT(HandleButtonNum0()));
  connect(_ui->unary_minus_button, SIGNAL(clicked()), this,
          SLOT(HandleButtonUnaryMinus()));
  connect(_ui->point_button, SIGNAL(clicked()), this,
          SLOT(HandlePointButton()));
  connect(_ui->backspace_button, SIGNAL(clicked()), this,
          SLOT(HandleBackspaceButton()));
  
  // Слоты для обработки изменений в текстовых полях калькулятора
  connect(_ui->x_edit, SIGNAL(textChanged(QString)), this,
          SLOT(HandleXEditChanged()));
  connect(_ui->src_input, SIGNAL(textChanged()), this,
          SLOT(HandleInputChanged()));
}

QString MathView::GetViewMathExpression() {
  return _ui->src_input->toPlainText().remove('\n');
}

void MathView::SetCalcResult(double result) {
  _ui->result_edit->setText(ViewHelpers::DoubleToQStr(result, 7));
}

void MathView::SetCalcResult(const QString& result) {
  _ui->result_edit->setText(result);
}

void MathView::SetXValueError(const QString& errorText) {
  SetCalcResult("ERROR");
  _ui->x_edit->setStyleSheet("border: 1px solid red; color: red;");
  QMessageBox::critical(0, "Ошибка", errorText);
}

void MathView::SetInputValueError(const QString& errorText) {
  SetCalcResult("ERROR");
  _ui->src_input->setStyleSheet("border: 1px solid red; color: red;");
  QMessageBox::critical(0, "Ошибка", errorText);
}

void MathView::ResetXValueError() { _ui->x_edit->setStyleSheet(""); }

void MathView::ResetInputValueError() { _ui->src_input->setStyleSheet(""); }

void MathView::ResetAllErrors() {
  ResetXValueError();
  ResetInputValueError();
}

void MathView::HandleCalcButton() {
  ResetAllErrors();

  bool xConvertOk;
  double xValue =
      _ui->x_edit->text().replace(",", ".").toDouble(&xConvertOk);
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

void MathView::HandleClearButton() { _ui->src_input->clear(); }

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