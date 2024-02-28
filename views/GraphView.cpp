//
// Created by bulatruslanovich on 26/02/2024.
//

#include "GraphView.h"

#include <vector>

#include "widgets/qcustomplot.h"
 #include "../ui/ui_GraphView.h"
#include "ViewHelpers.h"

namespace s21 {

GraphView::GraphView(IGraphController* controller, IGraphModelPublic* model,
                     QWidget* parent)
    : QDialog(parent),
      _ui(new Ui::GraphView),
      _controller(controller),
      _model(model) {
  _ui->setupUi(this);
  SetupButtons();
  ResetResult();
  _model->AddObserver(static_cast<IObserverGraphUpdate*>(this));
  _model->AddObserver(static_cast<IObserverGraphError*>(this));
}

GraphView::~GraphView() {
  _model->RemoveObserver(static_cast<IObserverGraphUpdate*>(this));
  _model->RemoveObserver(static_cast<IObserverGraphError*>(this));
  delete _ui;
}

void GraphView::UpdateGraphWindow(const QString& mathExpression) {
  _controller->SetExpression(mathExpression);

  if (!isVisible()) {
    move(0, 0);
    show();
  }

  setWindowState(windowState() & (~Qt::WindowMinimized | Qt::WindowActive));
  raise();
  //raise() используется для того, чтобы поднять окно поверх других окон
  // в Z-порядке, что делает его видимым и активным для пользователя.
  Draw();
}

void GraphView::Update() {
  UpdateInputFieldsFromModel();
  UpdateResultFromModel();
}

void GraphView::Error(const std::pair<std::string, std::string>& error) {
  ResetResult();
  UpdateInputFieldsFromModel();

  auto errorText = QString::fromStdString(error.second);
  if (error.first == "xGraph") {
    SetXBeginError("");
    SetXEndError(errorText);
  } else if (error.first == "yGraph") {
    SetYBeginError("");
    SetYEndError(errorText);
  } else if (error.first == "graphCalculation") {
    // ничего не делаем
  } else {
    QMessageBox::critical(0, "Ошибка отрисовки графика", errorText);
  }
}

void GraphView::SetupButtons() {
  connect(_ui->backButton, SIGNAL(clicked()), this, SLOT(BackButtonHandle()));
  connect(_ui->drawButton, SIGNAL(clicked()), this, SLOT(DrawButtonHandle()));
  connect(_ui->defaultSettingsButton, SIGNAL(clicked()), this,
          SLOT(DefaultSettingsButtonHandle()));
}

void GraphView::GraphPlotConfig(double _beginX, double _endX, double _beginY,
                                double _endY) {
  _ui->graphPlot->xAxis->setRange(_beginX, _endX);
  _ui->graphPlot->yAxis->setRange(_beginY, _endY);
  _ui->graphPlot->xAxis->setLabel("X");
  _ui->graphPlot->yAxis->setLabel("Y");
}

void GraphView::UpdateInputFieldsFromModel() {
  // Обновляем данные для расчета по данным модели
  double _beginX = _model->GetXBegin();
  double _endX = _model->GetXEnd();
  double _beginY = _model->GetYBegin();
  double _endY = _model->GetYEnd();

  _ui->graphPlot->clearGraphs();
  GraphPlotConfig(_beginX, _endX, _beginY, _endY);
  _ui->graphPlot->replot();

  _ui->xMinEdit->setText(ViewHelpers::DoubleToQStr(_beginX, 7));
  _ui->xMaxEdit->setText(ViewHelpers::DoubleToQStr(_endX, 7));

  if (_model->GetYScaleAuto()) {
    _ui->yMinEdit->setText("");
    _ui->yMaxEdit->setText("");
  } else {
    _ui->yMinEdit->setText(ViewHelpers::DoubleToQStr(_beginY, 7));
    _ui->yMaxEdit->setText(ViewHelpers::DoubleToQStr(_endY, 7));
  }
}

void GraphView::UpdateResultFromModel() {
  const std::vector<std::pair<double, double>>& points = _model->GetPoints();

  QVector<double> xVec, yVec;

  for (auto& item : points) {
    if (std::isnan(item.second) || std::isinf(item.second)) {
      if (xVec.size() > 0) {
        QCPGraph* graph = _ui->graphPlot->addGraph();
        graph->setData(xVec, yVec);
        _ui->graphPlot->replot();
        xVec.clear();
        yVec.clear();
      }
      continue;
    }

    xVec.push_back(item.first);
    yVec.push_back(item.second);
  }

  QCPGraph* graph = _ui->graphPlot->addGraph();
  graph->setData(xVec, yVec);

  _ui->graphPlot->replot();
}

void GraphView::ResetResult() {
  _ui->graphPlot->clearGraphs();
  _ui->graphPlot->replot();
}

void GraphView::ResetInputFields() {
  _ui->xMinEdit->setText(ViewHelpers::DoubleToQStr(-100.0, 7));
  _ui->xMaxEdit->setText(ViewHelpers::DoubleToQStr(100, 7));
  _ui->yMinEdit->setText("");
  _ui->yMaxEdit->setText("");
}

void GraphView::Draw() {
  ResetAllErrors();

  bool isCorrectInput;
  double xBegin =
      _ui->xMinEdit->text().replace(",", ".").toDouble(&isCorrectInput);

  if (!isCorrectInput) {
    SetXBeginError("Поле \"X от\" содержит некорректное значение!");
    return;
  }

  double xEnd =
      _ui->xMaxEdit->text().replace(",", ".").toDouble(&isCorrectInput);

  if (!isCorrectInput) {
    SetXEndError("Поле \"X до\" содержит некорректное значение!");
    return;
  }

  double yBegin =
      _ui->yMinEdit->text().replace(",", ".").toDouble(&isCorrectInput);

  if (_ui->yMinEdit->text().length() > 0 && !isCorrectInput) {
    SetYBeginError("Поле \"Y от\" содержит некорректное значение!");
    return;
  }

  double yEnd =
      _ui->yMaxEdit->text().replace(",", ".").toDouble(&isCorrectInput);

  if (_ui->yMaxEdit->text().length() > 0 && !isCorrectInput) {
    SetYEndError("Поле \"Y до\" содержит некорректное значение!");
    return;
  }

  xBegin = QString::number(xBegin, 'f', 7).toDouble();
  xEnd = QString::number(xEnd, 'f', 7).toDouble();
  yBegin = QString::number(yBegin, 'f', 7).toDouble();
  yEnd = QString::number(yEnd, 'f', 7).toDouble();

  if (yBegin == yEnd) {
    _controller->SetYScaleAuto(true);
  } else {
    _controller->SetYScaleAuto(false);
  }

  _controller->SetBeginX(xBegin);
  _controller->SetEndX(xEnd);
  _controller->SetBeginY(yBegin);
  _controller->SetEndY(yEnd);

  _controller->CalculateGraph();
}

void GraphView::SetXBeginError(const QString& errorText) {
  ResetResult();
  _ui->xMinEdit->setStyleSheet("border: 1px solid red; color: red;");
  if (!errorText.isEmpty()) {
    QMessageBox::critical(0, "Ошибка отрисовки графика", errorText);
  }
}

void GraphView::SetXEndError(const QString& errorText) {
  ResetResult();
  _ui->xMaxEdit->setStyleSheet("border: 1px solid red; color: red;");
  if (!errorText.isEmpty()) {
    QMessageBox::critical(0, "Ошибка отрисовки графика", errorText);
  }
}

void GraphView::SetYBeginError(const QString& errorText) {
  ResetResult();
  _ui->yMinEdit->setStyleSheet("border: 1px solid red; color: red;");
  if (!errorText.isEmpty()) {
    QMessageBox::critical(0, "Ошибка отрисовки графика", errorText);
  }
}

void GraphView::SetYEndError(const QString& errorText) {
  ResetResult();
  _ui->yMaxEdit->setStyleSheet("border: 1px solid red; color: red;");
  if (!errorText.isEmpty()) {
    QMessageBox::critical(0, "Ошибка отрисовки графика", errorText);
  }
}

void GraphView::ResetXBeginError() { _ui->xMinEdit->setStyleSheet(""); }

void GraphView::ResetXEndError() { _ui->xMaxEdit->setStyleSheet(""); }

void GraphView::ResetYBeginError() { _ui->yMinEdit->setStyleSheet(""); }

void GraphView::ResetYEndError() { _ui->yMaxEdit->setStyleSheet(""); }

void GraphView::ResetAllErrors() {
  ResetXBeginError();
  ResetXEndError();
  ResetYBeginError();
  ResetYEndError();
}

void GraphView::BackButtonHandle() { close(); }

void GraphView::DrawButtonHandle() { Draw(); }

void GraphView::DefaultSettingsButtonHandle() {
  ResetInputFields();
  Draw();
}

}  // namespace s21