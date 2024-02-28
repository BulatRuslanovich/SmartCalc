//
// Created by bulatruslanovich on 28/02/2024.
//

#include <QApplication>

#include "CalcAppContainer.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  s21::CalcAppContainer calculator;
  calculator.Init();

  return app.exec();
}
