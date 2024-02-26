//
// Created by bulatruslanovich on 26/02/2024.
//

#ifndef SMARTCALCV2_GRAPHVIEW_H
#define SMARTCALCV2_GRAPHVIEW_H
#include <QDialog>

#include "../interfaces/IGraphController.h"
#include "../interfaces/IGraphModelPublic.h"
#include "../interfaces/observers/IObserverGraph.h"

namespace Ui {
class GraphView;
}

namespace s21 {

class GraphView final : public QDialog,
                        public IObserverGraphUpdate,
                        public IObserverGraphError {
  Q_OBJECT

 public:
  explicit GraphView(IGraphController* controller, IGraphModelPublic* model,
                     QWidget* parent = nullptr);

  GraphView(const GraphView& other) = delete;
  GraphView(GraphView&& other) = delete;
  GraphView& operator=(const GraphView& other) = delete;
  GraphView& operator=(GraphView&& other) = delete;
  ~GraphView();

  void UpdateGraphWindow(const QString& mathExpression);
  void Update() override;
  void Error(const std::pair<std::string, std::string>& error) override;

 private:
  void SetupButtons();
  void GraphPlotConfig(double beginX, double endX, double beginY, double endY);
  void UpdateInputFieldsFromModel();
  void UpdateResultFromModel();
  void ResetResult();
  void ResetInputFields();
  void Draw();
  void SetXBeginError(const QString& error);
  void SetXEndError(const QString& error);
  void SetYBeginError(const QString& error);
  void SetYEndError(const QString& error);
  void ResetXBeginError();
  void ResetXEndError();
  void ResetYBeginError();
  void ResetYEndError();
  void ResetAllErrors();

  Ui::GraphView* _ui;
  IGraphController* _controller;
  IGraphModelPublic* _model;

 private slots:
  void BackButtonHandle();
  void DrawButtonHandle();
  void DefaultSettingsButtonHandle();
};

}  // namespace s21

#endif  // SMARTCALCV2_GRAPHVIEW_H
