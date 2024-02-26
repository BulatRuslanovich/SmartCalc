//
// Created by bulatruslanovich on 25/02/2024.
//

#ifndef SMARTCALCV2_CREDITVIEW_H
#define SMARTCALCV2_CREDITVIEW_H

#include <QWidget>

#include "../interfaces/ICreditController.h"
#include "../interfaces/ICreditModelPublic.h"
#include "../interfaces/observers/IObserverCredit.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class CreditView;
}
QT_END_NAMESPACE

namespace s21 {

class CreditView final : public QWidget,
                         public IObserverCreditUpdate,
                         public IObserverCreditError {
  Q_OBJECT

 public:
  explicit CreditView(ICreditController* controller, ICreditModelPublic* model,
                      QWidget* parent = nullptr);
  CreditView(const CreditView& other) = delete;
  CreditView(CreditView&& other) = delete;
  CreditView& operator=(const CreditView& other) = delete;
  CreditView& operator=(CreditView&& other) = delete;
  ~CreditView();

  void Update() override;
  void Error(const std::pair<std::string, std::string>& error) override;

 private:
  void SetupCreditButtons();
  void UpdateInputFieldsFromModel();
  void UpdateResultFromModel();
  void ResetResult();
  void SetCreditSumError(const QString& error);
  void SetCreditMonthsError(const QString& error);
  void SetCreditInterestError(const QString& error);

  void ResetCreditSumError();
  void ResetCreditMonthsError();
  void ResetCreditInterestError();
  void ResetAllErrors();

  Ui::CreditView* _ui;
  ICreditController* _controller;
  ICreditModelPublic* _model;

 private slots:
  void HandleCreditButton();
  void HandleCreditSumChanged();
  void HandleCreditMonthsChanged();
  void HandleCreditInterestChanged();
};

}  // namespace s21

#endif  // SMARTCALCV2_CREDITVIEW_H
