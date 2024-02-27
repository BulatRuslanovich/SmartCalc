//
// Created by bulatruslanovich on 27/02/2024.
//

#ifndef SMARTCALCV2_DEPOSITVIEW_H
#define SMARTCALCV2_DEPOSITVIEW_H

#include <QBoxLayout>
#include <QWidget>

#include "../interfaces/IDepositController.h"
#include "../interfaces/IDepositModelPublic.h"
#include "../interfaces/observers/IObserverDeposit.h"

namespace Ui {
class DepositView;
}

namespace s21 {

class DepositView final : public QWidget,
                          public IObserverDepositError,
                          public IObserverDepositUpdate {
  Q_OBJECT

 public:
  explicit DepositView(IDepositController* controller,
                       IDepositModelPublic* model,
                       QWidget* parent = nullptr);
  
  DepositView(const DepositView& other) = delete;
  DepositView(DepositView&& other) = delete;
  DepositView& operator=(const DepositView& other) = delete;
  DepositView& operator=(DepositView&& other) = delete;
  ~DepositView();

  void Update() override;
  void Error(const std::pair<std::string, std::string>& error) override;

 private:
  void SetupDepositButtons();
  void UpdateInputFieldsFromModel();
  void UpdateResultFromModel();
  void ResetResult();
  bool OperationsReplenishmentHandle();
  bool OperationsWithdrawalHandle();
  void AddOperationLine(QBoxLayout* layout);

  void SetDepositSumError(const QString& errorText);
  void SetDepositMonthsError(const QString& errorText);
  void SetDepositInterestError(const QString& errorText);
  void SetDepositTaxError(const QString& errorText);
  void SetDepositDateStartError(const QString& errorText);
  void SetDepositAddMoneyError(const QString& errorText);
  void SetDepositSubMoneyError(const QString& errorText);

  void ResetDepositSumError();
  void ResetDepositMonthsError();
  void ResetDepositInterestError();
  void ResetDepositTaxError();
  void ResetDepositDateStartError();
  void ResetAllErrors();

  Ui::DepositView* _ui;
  IDepositController* _controller;
  IDepositModelPublic* _model;

 private slots:
  void HandleDepositButton();
  void AddOperationReplenishment();
  void AddOperationWithdrawal();
  void DeleteOperationLine();

  void HandleDepositSumChanged();
  void HandleDepositMonthsChanged();
  void HandleDepositInterestChanged();
  void HandleDepositTaxChanged();
  void HandleDepositDateStartChanged();
};

}  // namespace s21

#endif  // SMARTCALCV2_DEPOSITVIEW_H
