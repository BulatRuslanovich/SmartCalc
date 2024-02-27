//
// Created by bulatruslanovich on 27/02/2024.
//

#ifndef SMARTCALCV2_MATHVIEW_H
#define SMARTCALCV2_MATHVIEW_H

#include <QWidget>

#include "../interfaces/IMathController.h"
#include "../interfaces/IMathModelPublic.h"
#include "../interfaces/observers/IObserverMath.h"
#include "GraphView.h"

namespace Ui {
class MathView;
}

namespace s21 {

class MathView final : public QWidget,
                       public IObserverMathError,
                       public IObserverMathUpdate,
                       public IMathModelPublic {
  Q_OBJECT

 public:
  explicit MathView(IMathController* controller,
                    IMathModelPublic* model, GraphView* graphView,
                    QWidget* parent = nullptr);

  MathView(const MathView& other) = delete;
  MathView(MathView&& other) = delete;
  MathView& operator=(const MathView& other) = delete;
  MathView& operator=(MathView&& other) = delete;
  ~MathView();

  void Update() override;
  void Error(const std::pair<std::string, std::string>& error) override;

 private:
  void SetupButtons();
  QString GetViewMathExpression();
  void SetCalcResult(double result);
  void SetCalcResult(const QString& result);
  void SetXValueError(const QString& error_text);
  void SetInputValueError(const QString& error_text);
  void ResetXValueError();
  void ResetInputValueError();
  void ResetAllErrors();

  Ui::MathView* _ui;
  IMathController* _controller;
  IMathModelPublic* _model;
  GraphView* _graphView;

 private slots:
  void HandleCalcButton();
  void HandleGraphButton();

  void HandleClearButton();
  void HandlePointButton();
  void HandleBackspaceButton();
  void HandleButtonMod();
  void HandleButtonDiv();
  void HandleButtonMul();
  void HandleButtonSub();
  void HandleButtonAdd();
  void HandleButtonLBracket();
  void HandleButtonRBracket();
  void HandleButtonPow();
  void HandleButtonCos();
  void HandleButtonSin();
  void HandleButtonTan();
  void HandleButtonSqrt();
  void HandleButtonAcos();
  void HandleButtonAtan();
  void HandleButtonAsin();
  void HandleButtonLn();
  void HandleButtonPi();
  void HandleButtonE();
  void HandleButtonX();
  void HandleButtonLog();
  void HandleButtonNum0();
  void HandleButtonNum1();
  void HandleButtonNum2();
  void HandleButtonNum3();
  void HandleButtonNum4();
  void HandleButtonNum5();
  void HandleButtonNum6();
  void HandleButtonNum7();
  void HandleButtonNum8();
  void HandleButtonNum9();
  void HandleButtonUnaryMinus();

  void HandleXEditChanged();
  void HandleInputChanged();
};

}  // namespace s21

#endif  // SMARTCALCV2_MATHVIEW_H
