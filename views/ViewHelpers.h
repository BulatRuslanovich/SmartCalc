//
// Created by bulatruslanovich on 25/02/2024.
//

#ifndef SMARTCALCV2_VIEWHELPERS_H
#define SMARTCALCV2_VIEWHELPERS_H
#include <QDateEdit>
#include <QLayout>
#include <QString>


namespace s21::ViewHelpers {
  void SetupDateEdit(QDateEdit *dateEdit);
  QLayout *FindParentLayout(QWidget *widget, QLayout *parent);
  QLayout *FindParentLayout(QWidget *widget);
  QString DoubleToQStr(double num, double precision);
}


#endif  // SMARTCALCV2_VIEWHELPERS_H
