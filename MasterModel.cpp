//
// Created by bulatruslanovich on 27/02/2024.
//

#include "MasterModel.h"

#include "helpers/mathHelper.h"
#include "models/calcException.h"
#include "models/financeException.h"

namespace  s21 {
MasterModel::MasterModel() : mathModel(new MathModel), graphModel(new  GraphModel), creditModel(new CreditModel), depositModel(new DepositModel){}

MasterModel::~MasterModel() {
  delete mathModel;
  delete graphModel;
  delete creditModel;
  delete depositModel;
}

void MasterModel::SetExpression(const std::string& newExp) {
  mathModel->setExpression(newExp);
}

void MasterModel::SetRadDegFactor(double value) {
  mathModel->setRadDegFactor(value);
}

void MasterModel::SetX(double x) {
  mathModel->setX(x);
}
void MasterModel::ExpressionAdd(const char* add) {
  try {
    mathModel->ExpressionAddString(add);
    AnnunciatorUpdateMathObservers();
  } catch (const CalcException&) {
    MathExceptionHandle();
  }
}

void MasterModel::ExpressionDeleteEnd() {
  mathModel->ExpressionDeleteEnd();
  AnnunciatorUpdateMathObservers();
}

void MasterModel::CalculateX() {
  try {
    mathModel->Calculate();
    AnnunciatorUpdateMathObservers();
  } catch (const CalcException&) {
    MathExceptionHandle();
  }
}

double MasterModel::GetResult() {
  return mathModel->getResult();
}

double MasterModel::GetX() {
    return mathModel->getX_Value();
}

std::string MasterModel::GetExpression() {
  return mathModel->getExpression();
}

bool MasterModel::IsCalc() { return mathModel->getIsCalc(); }
void MasterModel::AddObserver(IObserverMathUpdate* observer) {mathAnnunciator.AddObserver(observer); }
void MasterModel::RemoveObserver(IObserverMathUpdate* observer) {mathAnnunciator.RemoveObserver(observer); }
void MasterModel::AddObserver(IObserverMathError* observer) {mathAnnunciator.AddObserver(observer);}
void MasterModel::RemoveObserver(IObserverMathError* observer) {mathAnnunciator.RemoveObserver(observer); }

void MasterModel::SetGraphExpression(const std::string& newExp) {
  graphModel->setGraphExpression(newExp);
}

void MasterModel::SetBeginX(double newX) {
  graphModel->setXBegin(newX);
}

void MasterModel::SetEndX(double newX) {
  graphModel->setXEnd(newX);
}

void MasterModel::SetBeginY(double newY) {
  graphModel->setYBegin(newY);
}
void MasterModel::SetEndY(double newY) {
  graphModel->setYEnd(newY);
}
void MasterModel::SetYScaleAuto(bool flag) {
  graphModel->setYScaleAuto(flag);
}

void MasterModel::CalculateGraph() {
  try {
    graphModel->CalculateGraph();
    AnnunciatorUpdateGraphObservers();
  } catch (const CalcException &) {
    GraphExceptionHandle();
  }

}

double MasterModel::GetXBegin() { return graphModel->getXBegin(); }
double MasterModel::GetXEnd() { return graphModel->getXEnd(); }
double MasterModel::GetYBegin() { return graphModel->getYBegin(); }
double MasterModel::GetYEnd() { return graphModel->getYEnd(); }
bool MasterModel::GetYScaleAuto() { return graphModel->getYScaleAuto(); }
const std::vector<std::pair<double, double>>& MasterModel::GetPoints() {
  return graphModel->getPoints();
}
void MasterModel::AddObserver(IObserverGraphUpdate* observer) { graphAnnunciator.AddObserver(observer); }
void MasterModel::RemoveObserver(IObserverGraphUpdate* observer) {graphAnnunciator.RemoveObserver(observer);}
void MasterModel::AddObserver(IObserverGraphError* observer) {graphAnnunciator.AddObserver(observer);}
void MasterModel::RemoveObserver(IObserverGraphError* observer) {graphAnnunciator.RemoveObserver(observer);}

void MasterModel::SetCreditSum(double sum) {creditModel->setSum(sum);}
void MasterModel::SetCreditMonths(int months) {creditModel->setMonths(months);}
void MasterModel::SetCreditInterest(double interest) {creditModel->setInterest(interest);}

void MasterModel::SetCreditType(ICreditType type) {
  if (type == Annuity) {
    creditModel->setType(CreditModel::Annuity);
  } else {
    creditModel->setType(CreditModel::Differentiated);
  }
}

void MasterModel::CalculateCredit() {
  try {
    creditModel->CalculateCredit();
    AnnunciatorUpdateCreditObservers();
  } catch (const FinanceException &) {
    CreditExceptionHandle();
  }
}
double MasterModel::GetSum() { return creditModel->getSum(); }
int MasterModel::GetMonths() { return creditModel->getMonths(); }
double MasterModel::GetInterest() { return creditModel->getInterest(); }
ICreditType MasterModel::GetType() {
  CreditModel::CreditType res = creditModel->getType();

  if (res == CreditModel::Annuity) {
    return Annuity;
  } else {
    return Differentiated;
  }
}

double MasterModel::GetMin() { return creditModel->getPaymentMin(); }
double MasterModel::GetMax() { return creditModel->getPaymentMax(); }
double MasterModel::GetOver() { return creditModel->getOverPayment(); }
double MasterModel::GetTotal() { return creditModel->getTotalPayment(); }
bool MasterModel::IsCreditCalc() { return creditModel->getIsCalc(); }

void MasterModel::AddObserver(IObserverCreditUpdate* observer) {creditAnnunciator.AddObserver(observer); }
void MasterModel::RemoveObserver(IObserverCreditUpdate* observer) {creditAnnunciator.RemoveObserver(observer); }
void MasterModel::AddObserver(IObserverCreditError* observer) {creditAnnunciator.AddObserver(observer);}
void MasterModel::RemoveObserver(IObserverCreditError* observer) {creditAnnunciator.RemoveObserver(observer); }


void MasterModel::SetDepositSum(double sum) {
  depositModel->setSum(sum);
}
void MasterModel::SetDepositPeriod(int months) {
  depositModel->setMonths(months);
}
void MasterModel::SetDepositInterest(double interest) {
  depositModel->setInterest(interest);
}
void MasterModel::SetDepositTax(double tax) {
  depositModel->setTax(tax);
}
void MasterModel::SetDepositPeriodicityType(IDepositType type) {
  if (type == Day) {
    depositModel->setPayPeriod(DepositModel::Day);
  } else if (type == Month) {
    depositModel->setPayPeriod(DepositModel::Month);
  } else {
    depositModel->setPayPeriod(DepositModel::End);
  }
}

void MasterModel::SetDepositCapitalization(bool capitalization) {
  depositModel->setCapitalization(capitalization);
}

void MasterModel::SetDepositDateStart(int year, int month, int day) {
  depositModel->setStartDate(year, month, day);
}

void MasterModel::AddDepositOperationReplenishment(int year, int month, int day,
                                                   double amount) {
  depositModel->AddOperation(year, month, day, amount);
}

void MasterModel::AddDepositOperationWithdrawal(int year, int month, int day,
                                                double amount) {
  depositModel->AddOperation(year, month, day, -amount);
}

void MasterModel::ClearDepositOperationsList() {
  depositModel->ClearOperationsList();
}

void MasterModel::CalculateDeposit() {
  try {
    depositModel->CalculateDeposit();
    AnnunciatorUpdateDepositObservers();
  } catch (const FinanceException &) {
    DepositExceptionHandle();
  }
}

double MasterModel::GetInterestPayment() { return depositModel->getInterestAmount(); }
double MasterModel::GetTotalPayment() { return depositModel->getDepositAmount(); }
double MasterModel::GetTaxPayment() { return depositModel->getTaxAmount(); }
bool MasterModel::IsDepositCalc() { return depositModel->getIsCalc(); }
double MasterModel::GetDepositSum() { return depositModel->getSum(); }
int MasterModel::GetDepositMonths() { return depositModel->getMonths(); }
double MasterModel::GetDepositInterest() { return depositModel->getInterest(); }
double MasterModel::GetTax() { return depositModel->getTax(); }
IDepositType MasterModel::GetDepositType() {
  DepositModel::PayPeriodDepositType type = depositModel->getPayPeriod();

  if (type == DepositModel::Day) {
    return Day;
  } else if (type == DepositModel::Month) {
    return Month;
  } else {
    return End;
  }
}


bool MasterModel::GetDepositCapitalizationType() { return depositModel->getCapitalization(); }

void MasterModel::AddObserver(IObserverDepositUpdate* observer) {depositAnnunciator.AddObserver(observer); }
void MasterModel::RemoveObserver(IObserverDepositUpdate* observer) {depositAnnunciator.RemoveObserver(observer); }
void MasterModel::AddObserver(IObserverDepositError* observer) {depositAnnunciator.AddObserver(observer);}
void MasterModel::RemoveObserver(IObserverDepositError* observer) {depositAnnunciator.RemoveObserver(observer); }


void MasterModel::MathExceptionHandle() {
  try {
    throw;
  } catch (const InputZeroLengthException &) {
    AnnunciatorErrorMathObservers(
        {"input", "Математическое выражение должно быть заполнено!"});
  } catch (const InputTooLongException &) {
    AnnunciatorErrorMathObservers(
        {"input", "Математическое выражение должно быть длиной менее " +
                      mathHelper::doubleToString(
                          static_cast<int>(MathModel::maxInputSize))});
  } catch (const InputIncorrectException &) {
    AnnunciatorErrorMathObservers(
        {"input", "Математическое выражение некорректно!"});
  } catch (const WrongOperatorException &) {
    AnnunciatorErrorMathObservers(
        {"wrong_operator", "Использован недопустимый оператор!"});
  } catch (const CalcException &) {
    AnnunciatorErrorMathObservers({"other", "Неизвестная ошибка!"});
  }
}

void MasterModel::GraphExceptionHandle() {
  try {
    throw;
  } catch (const WrongXGraphException &) {
    AnnunciatorErrorGraphObservers(
        {"xGraph", "X должно быть от " +
                        mathHelper::doubleToString(graphModel->_minX) +
                        " до " +
                        mathHelper::doubleToString(graphModel->_maxX)});
  } catch (const WrongYGraphException &) {
    AnnunciatorErrorGraphObservers(
        {"yGraph", "Y должно быть от " +
                        mathHelper::doubleToString(graphModel->_minY) +
                        " до " +
                        mathHelper::doubleToString(graphModel->_maxY)});
  } catch (const InputZeroLengthException &) {
    AnnunciatorErrorGraphObservers(
        {"graphCalculation",
         "Ошибка вычисления точек графика - Математическое выражение должно "
         "быть заполнено!"});

  } catch (const InputTooLongException &) {
    AnnunciatorErrorGraphObservers(
        {"graphCalculation",
         "Ошибка вычисления точек графика - Математическое выражение должно "
         "быть длиной менее " +
             mathHelper::doubleToString(
                 static_cast<int>(MathModel::maxInputSize))});
  } catch (const InputIncorrectException &) {
    AnnunciatorErrorGraphObservers({"graphCalculation",
                               "Ошибка вычисления точек графика - "
                               "математическое выражение некорректно!"});
  } catch (const CalcException &) {
    AnnunciatorErrorGraphObservers({"other", "Неизвестная ошибка"});
  }
}

void MasterModel::CreditExceptionHandle() {
  try {
    throw;
  } catch (const FinanceSumException &) {
    AnnunciatorErrorCreditObservers(
        {"sum",
         "Сумма кредита должен быть от " +
             mathHelper::doubleToString(creditModel->minCreditSum) +
             " до " +
             mathHelper::doubleToString(creditModel->maxCreditSum) +
             " (руб.)"});
  } catch (const FinanceMonthException &) {
    AnnunciatorErrorCreditObservers(
        {"months",
         "Срок кредита должен быть от " +
             mathHelper::doubleToString(creditModel->minCreditMonths) +
             " до " +
             mathHelper::doubleToString(creditModel->maxCreditMonths) +
             " (месяцев)"});
  } catch (const FinanceInterestException &) {
    AnnunciatorErrorCreditObservers(
        {"interest",
         "Процентная ставка должна быть от " +
             mathHelper::doubleToString(creditModel->minCreditInterest) +
             " до " +
             mathHelper::doubleToString(creditModel->maxCreditInterest) +
             " (%)"});
  } catch (const FinanceException &) {
    AnnunciatorErrorCreditObservers({"other", "Неизвестная ошибка"});
  }
}

void MasterModel::DepositExceptionHandle() {
  try {
    throw;
  } catch (const FinanceSumException &) {
    AnnunciatorErrorDepositObservers(
        {"sum",
         "Сумма вклада должен быть от " +
             mathHelper::doubleToString(depositModel->minDepositSum) +
             " до " +
             mathHelper::doubleToString(depositModel->minDepositSum) +
             " (руб.)"});
    
  } catch (const FinanceMonthException &) {
    AnnunciatorErrorDepositObservers(
        {"months",
         "Срок вклада должен быть от " +
             mathHelper::doubleToString(depositModel->minDepositMonths) +
             " до " +
             mathHelper::doubleToString(depositModel->maxDepositMonths) +
             " (месяцев)"});
  } catch (const FinanceInterestException &) {
    AnnunciatorErrorDepositObservers(
        {"interest", "Процентная ставка должна быть от " +
                         mathHelper::doubleToString(
                             depositModel->minDepositInterest) +
                         " до " +
                         mathHelper::doubleToString(
                             depositModel->maxDepositInterest) +
                         " (%)"});

  } catch (const FinanceTaxException &) {
    AnnunciatorErrorDepositObservers(
        {"tax",
         "Налоговая ставка должна быть от " +
             mathHelper::doubleToString(depositModel->minDepositTax) +
             " до " +
             mathHelper::doubleToString(depositModel->maxDepositTax) +
             " (%)"});
  } catch (const FinanceDateStartException &) {
    AnnunciatorErrorDepositObservers(
        {"date_start",
         "Некорректная дата вклада (допустимы даты между " +
             mathHelper::doubleToString(depositModel->minDepositYear) +
             " и " +
             mathHelper::doubleToString(depositModel->maxDepositYear) +
             " годами)"});
  } catch (const FinanceDateOperationPlusException &) {
    AnnunciatorErrorDepositObservers(
        {"replenishment_date",
         "Некорректная дата пополнения (допустимы даты между " +
             mathHelper::doubleToString(depositModel->minDepositYear) +
             " до " +
             mathHelper::doubleToString(depositModel->maxDepositYear) +
             " годами)"});
  } catch (const FinanceDateOperationMinusException &) {
    AnnunciatorErrorDepositObservers(
        {"withdrawal_date",
         "Некорректная дата снятия (допустимы даты между " +
             mathHelper::doubleToString(depositModel->minDepositYear) +
             " до " +
             mathHelper::doubleToString(depositModel->maxDepositYear) +
             " годами)"});
  } catch (const FinanceSumOperationPlusException &) {
    AnnunciatorErrorDepositObservers(
        {"replenishment_sum",
         "Сумма пополнения должен быть от " +
             mathHelper::doubleToString(depositModel->minDepositSum) +
             " до " +
             mathHelper::doubleToString(depositModel->maxDepositSum) +
             " (руб.)"});
  } catch (const FinanceSumOperationMinusException &) {
    AnnunciatorErrorDepositObservers(
        {"withdrawal_sum",
         "Сумма снятия должен быть от " +
             mathHelper::doubleToString(depositModel->minDepositSum) +
             " до " +
             mathHelper::doubleToString(depositModel->maxDepositSum) +
             " (руб.)"});
  } catch (const FinanceOperationNoMoneyException &) {
    AnnunciatorErrorDepositObservers(
        {"operations_no_money",
         "Недостаточно денег на счете для совершения операции"});
  } catch (const FinanceException &) {
    AnnunciatorErrorDepositObservers({"other", "Неизвестная ошибка"});
  }
}

void MasterModel::AnnunciatorUpdateMathObservers() {
  mathAnnunciator.UpdateObservers();
}

void MasterModel::AnnunciatorErrorMathObservers(const std::pair<std::string, std::string>& error) {
  mathAnnunciator.ErrorObservers(error);
}

void MasterModel::AnnunciatorUpdateGraphObservers() {
  graphAnnunciator.UpdateObservers();
}

void MasterModel::AnnunciatorErrorGraphObservers(const std::pair<std::string, std::string>& error) {
  graphAnnunciator.ErrorObservers(error);
}

void MasterModel::AnnunciatorUpdateCreditObservers() {
  creditAnnunciator.UpdateObservers();
}

void MasterModel::AnnunciatorErrorCreditObservers(const std::pair<std::string, std::string>& error) {
  creditAnnunciator.ErrorObservers(error);
}

void MasterModel::AnnunciatorUpdateDepositObservers() {
  depositAnnunciator.UpdateObservers();
}
void MasterModel::AnnunciatorErrorDepositObservers(const std::pair<std::string, std::string>& error) {
  depositAnnunciator.ErrorObservers(error);
}

}  // namespace 