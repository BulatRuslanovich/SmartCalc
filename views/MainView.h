//
// Created by bulatruslanovich on 27/02/2024.
//

#ifndef SMARTCALCV2_MAINVIEW_H
#define SMARTCALCV2_MAINVIEW_H

#include <QMainWindow>

#include "CreditView.h"
#include "DepositView.h"
#include "MathView.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainView;
}
QT_END_NAMESPACE

namespace s21 {

class MainView final : public QMainWindow {
  Q_OBJECT

 public:
  MainView(MathView* mathView, CreditView* creditView, DepositView* depositView, QWidget* parent = nullptr);
  ~MainView();

 private:
  Ui::MainView* _ui;
};

}  // namespace s21

#endif  // SMARTCALCV2_MAINVIEW_H
