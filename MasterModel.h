//
// Created by bulatruslanovich on 27/02/2024.
//

#ifndef SMARTCALCV2_MASTERMODEL_H
#define SMARTCALCV2_MASTERMODEL_H

#include "helpers/annunciator.h"
#include "interfaces/IMathModel.h"
#include "interfaces/IDepositModel.h"
#include "interfaces/IGraphModel.h"
#include "interfaces/ICreditModel.h"
#include "models/credit/creditModel.h" //TODO: поменять название
#include "models/deposit/DepositModel.h"
#include "models/graph/GraphModel.h"
#include "models/math/mathModel.h" //TODO: поменять название

namespace s21 {

class MasterModel final : public IMathModel, public ICreditModel, public IDepositModel, public IGraphModel {
 public:
  MasterModel();
  MasterModel(const MasterModel& other) = delete;
  MasterModel(MasterModel&& other) = delete;
  MasterModel& operator=(const MasterModel& other) = delete;
  MasterModel& operator=(MasterModel&& other) = delete;
  ~MasterModel() override;

  void SetExpression(const std::string& newExp) override;
  void SetX(double x) override;
  void ExpressionAdd(const char* add) override;
  void ExpressionDeleteEnd() override;
  void CalculateX() override;
  double GetResult() override;
  double GetX() override;
  std::string GetExpression() override;
  bool IsCalc() override;
  void AddObserver(IObserverMathUpdate* observer) override;
  void RemoveObserver(IObserverMathUpdate* observer) override;
  void AddObserver(IObserverMathError* observer) override;
  void RemoveObserver(IObserverMathError* observer) override;

  void SetGraphExpression(const std::string& newExp) override;
  void SetBeginX(double newX) override;
  void SetEndX(double newX) override;
  void SetBeginY(double newY) override;
  void SetEndY(double newY) override;
  void SetYScaleAuto(bool flag) override;
  void CalculateGraph() override;
  double GetXBegin() override;
  double GetXEnd() override;
  double GetYBegin() override;
  double GetYEnd() override;
  bool GetYScaleAuto() override;
  const std::vector<std::pair<double, double>>& GetPoints() override;
  void AddObserver(IObserverGraphUpdate* observer) override;
  void RemoveObserver(IObserverGraphUpdate* observer) override;
  void AddObserver(IObserverGraphError* observer) override;
  void RemoveObserver(IObserverGraphError* observer) override;
  
  void SetCreditSum(double sum) override;
  void SetCreditMonths(int months) override;
  void SetCreditInterest(double interest) override;
  void SetCreditType(ICreditType type) override;
  void CalculateCredit() override;
  double GetSum() override;
  int GetMonths() override;
  double GetInterest() override;
  ICreditType GetType() override;
  double GetMin() override;
  double GetMax() override;
  double GetOver() override;
  double GetTotal() override;
  bool IsCreditCalc() override;
  void AddObserver(IObserverCreditUpdate* observer) override;
  void RemoveObserver(IObserverCreditUpdate* observer) override;
  void AddObserver(IObserverCreditError* observer) override;
  void RemoveObserver(IObserverCreditError* observer) override;
  
  void SetDepositSum(double sum) override;
  void SetDepositPeriod(int months) override;
  void SetDepositInterest(double interest) override;
  void SetDepositTax(double tax) override;
  void SetDepositPeriodicityType(IDepositType type) override;
  void SetDepositCapitalization(bool capitalization) override;
  void SetDepositDateStart(int year, int month, int day) override;
  void AddDepositOperationReplenishment(int year, int month, int day,
                                                double amount) override;
  void AddDepositOperationWithdrawal(int year, int month, int day,
                                             double amount) override;
  
  void ClearDepositOperationsList() override;
  void CalculateDeposit() override;
  double GetInterestPayment() override;
  double GetTotalPayment() override;
  double GetTaxPayment() override;
  bool IsDepositCalc() override;
  double GetDepositSum() override;
  int GetDepositMonths() override;
  double GetDepositInterest() override;
  double GetTax() override;
  IDepositType GetDepositType() override;
  bool GetDepositCapitalizationType() override;
  
  void AddObserver(IObserverDepositUpdate* observer) override;
  void RemoveObserver(IObserverDepositUpdate* observer) override;
  void AddObserver(IObserverDepositError* observer) override;
  void RemoveObserver(IObserverDepositError* observer) override;

 
 private:
  MathModel* mathModel;
  GraphModel* graphModel;
  CreditModel* creditModel;
  DepositModel* depositModel;

  Annunciator mathAnnunciator;
  Annunciator graphAnnunciator;
  Annunciator creditAnnunciator;
  Annunciator depositAnnunciator;

  void MathExceptionHandle();
  void GraphExceptionHandle();
  void CreditExceptionHandle();
  void DepositExceptionHandle();

  void AnnunciatorUpdateMathObservers();
  void AnnunciatorErrorMathObservers(const std::pair<std::string, std::string>& error);
  void AnnunciatorUpdateGraphObservers();
  void AnnunciatorErrorGraphObservers(const std::pair<std::string, std::string>& error);
  void AnnunciatorUpdateCreditObservers();
  void AnnunciatorErrorCreditObservers(const std::pair<std::string, std::string>& error);
  void AnnunciatorUpdateDepositObservers();
  void AnnunciatorErrorDepositObservers(const std::pair<std::string, std::string>& error);
};

}  // namespace s21

#endif  // SMARTCALCV2_MASTERMODEL_H
