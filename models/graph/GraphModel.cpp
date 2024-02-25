//
// Created by bulatruslanovich on 24/02/2024.
//

#include "GraphModel.h"

#include <cmath>
#include <limits>
#include <utility>

#include "../../helpers/mathHelper.h"

namespace s21 {
/**
 * @brief Конструктор по умолчанию класса GraphModel.
 *
 * Инициализирует модель графика с параметрами по умолчанию:
 * - mathModel - указатель на новый объект MathModel;
 * - xBegin - начальное значение оси X (-100 по умолчанию);
 * - xEnd - конечное значение оси X (100 по умолчанию);
 * - yBegin - начальное значение оси Y (0 по умолчанию);
 * - yEnd - конечное значение оси Y (0 по умолчанию);
 * - yScaleAuto - автоматическое масштабирование оси Y (true по умолчанию);
 * - _points - пустой вектор точек графика.
 */
GraphModel::GraphModel() : mathModel(new MathModel), xBegin(-100), xEnd(100), yBegin(0), yEnd(0), yScaleAuto(true), _points({}){}
/**
 * @brief Деструктор класса GraphModel.
 *
 * Освобождает ресурсы, занимаемые моделью графика.
 * Удаляет объект MathModel, на который указывает mathModel.
 */
GraphModel::~GraphModel() {delete mathModel;}

/**
 * @brief Сбрасывает результаты модели графика.
 *
 * Очищает вектор точек `_points`, удаляя все сохраненные точки графика.
 */
void GraphModel::ResetModelResults() noexcept {
  _points.clear();
}

/**
 * @brief Нормализует диапазон значений по оси X.
 *
 * Если начальное значение оси X (`xBegin`) больше конечного значения оси X (`xEnd`),
 * метод меняет их местами с использованием функции `std::swap`.
 */
void GraphModel::NormalizeXRange() noexcept {
  if (xBegin > xEnd) {
    std::swap(xBegin, xEnd);
  }
}

/**
 * @brief Нормализует диапазон значений по оси Y.
 *
 * Если начальное значение оси Y (`yBegin`) больше конечного значения оси Y (`yEnd`),
 * метод меняет их местами с использованием функции `std::swap`.
 */
void GraphModel::NormalizeYRange() noexcept {
  if (yBegin > yEnd) {
    std::swap(yBegin, yEnd);
  }
}

/**
 * @brief Проверяет данные модели графика.
 *
 * @throw WrongXGraphException если диапазон значений по оси X некорректен.
 * @throw WrongYGraphException если диапазон значений по оси Y некорректен и масштаб оси Y не автоматический.
 */
void GraphModel::CheckModelData() const {
  if (xBegin == xEnd || xBegin < _minX || xEnd > _maxX) {
    throw WrongXGraphException("X range error");
  }

  if (!yScaleAuto && (yBegin < _minY || yEnd > _maxY)) {
    throw WrongYGraphException("X range error");
  }
}

/**
 * @brief Вычисляет точки графика и обновляет диапазон значений по оси Y.
 */
void GraphModel::CalculateHandle() {
  double yMin = 0.0;
  double yMax = 0.0;

  CalculatePoints(xBegin, xEnd, numPointsAll, yMin, yMax, numOfCalc);
  SetYRange(yMin, yMax);
}

/**
 * @brief Проверяет, является ли точка разрывом функции.
 *
 * @param last Предыдущая точка.
 * @param current Текущая точка.
 * @return true если текущая точка является разрывом функции, в противном случае - false.
 * @note Этот метод не выбрасывает исключений (noexcept).
 */
bool GraphModel::IsFunctionBreakPoint(double last,
                                      double current) const noexcept {
  return (std::isnan(last) && !std::isnan(current)) || (std::isnan(current) && !std::isnan(last)) || ((last < 0 && current > 0) || (last > 0 && current < 0)) && (std::abs(last - current) > 40) || (!std::isinf(last / current) && std::abs(last / current) > 1E20);
}

/**
 * @brief Устанавливает диапазон значений по оси Y.
 *
 * Если автоматическое масштабирование оси Y (`yScaleAuto` равно true), метод обновляет
 * начальное и конечное значения по оси Y (`yBegin` и `yEnd` соответственно) на основе переданных
 * значений `yMin` и `yMax`. Если `yMin` и `yMax` равны 0.0, метод устанавливает значения -5.0 и 5.0 соответственно.
 * Если `yMin` меньше -1E6, то `yMin` устанавливается равным -1E6. Если `yMax` больше 1E6, то `yMax` устанавливается равным 1E6.
 * Все значения корректируются на 5% вверх, чтобы обеспечить небольшой зазор.
 *
 * @param yMin Минимальное значение по оси Y.
 * @param yMax Максимальное значение по оси Y.
 * @note Если `yScaleAuto` равно false, метод не выполняет никаких действий.
 */
void GraphModel::SetYRange(double yMin, double yMax) noexcept {
  if (yScaleAuto) {
    if (yMin == 0.0 && yMax == 0.0) {
      yMin = -5.0;
      yMin = 5.0;
    } else {
      if (yMin < -1E6) {
        yMin = -1E6;
      } else {
        yMin *= 1.05;
      }

      if (yMax > 1E6) {
        yMax = 1E6;
      } else {
        yMax *= 1.05;
      }
    }

    yBegin = yMin;
    yEnd = yMax;
  }
}

/**
 * @brief Вычисляет точки графика для заданного диапазона значений по оси X.
 *
 * Метод вычисляет точки графика для заданного диапазона значений по оси X.
 * Количество вычисленных точек определяется параметром `numPoints`.
 * Результаты вычислений сохраняются в векторе `_points`.
 *
 * @param _xBegin Начальное значение диапазона по оси X.
 * @param _xEnd Конечное значение диапазона по оси X.
 * @param numPoints Количество точек, которые необходимо вычислить в заданном диапазоне.
 * @param yMin Ссылка на переменную, в которую будет сохранено минимальное значение по оси Y.
 * @param yMax Ссылка на переменную, в которую будет сохранено максимальное значение по оси Y.
 * @param numOfCalcPoints Количество дополнительных вычислений точек, если обнаружен разрыв функции.
 * @note Метод корректирует значения координат для обеспечения точности вычислений.
 */
void GraphModel::CalculatePoints(double _xBegin, double _xEnd, double numPoints,
                                 double& yMin, double& yMax,
                                 int numOfCalcPoints) {
  double step = (_xEnd - _xBegin) / numPoints;
  int points = static_cast<int>(numPoints);

  bool firstIteration = true;
  double yLastVal = 0.0;
  double xLastVal = 0.0;

  for (double xValueTemp = _xBegin; points >= 0; xValueTemp += step) {
    double xValue;

    if (_xEnd - _xBegin > 1 || std::abs(xValueTemp) < (1E-7 / numPointsAll)) {
      xValue = mathHelper::Round(xValueTemp, 7);
    } else {
      xValue = xValueTemp;
    }

    mathModel->setX(xValue);
    mathModel->Calculate();

    double yValue = mathModel->getResult();

    if (std::isinf(yValue)) {
      yValue = std::numeric_limits<double>::quiet_NaN();
    }

    if (!firstIteration && IsFunctionBreakPoint(yLastVal, yValue)) {
      if (numOfCalcPoints > 0) {
        CalculatePoints(xLastVal, xValue, numPointsPre, yMin, yMax, numOfCalcPoints - 1);
      } else {
        (*(--_points.end())).second = std::numeric_limits<double>::quiet_NaN();
      }
    }

    _points.emplace_back(xValue, yValue); // должно быть лучше points.push_back(std::make_pair(xValue, yValue));, тк  не происходит двойное копирвание
    --points;

    if (yValue < yMin && !std::isnan(yValue) && !std::isinf(yValue)) {
      yMin = yValue;
    }

    if (yValue > yMax && !std::isnan(yValue) && !std::isinf(yValue)) {
      yMax = yValue;
    }

    firstIteration = false;
    yLastVal = yValue;
    xLastVal = xValue;
  }
}

/**
 * @brief Вычисляет график.
 *
 * Метод `CalculateGraph()` осуществляет последовательность шагов, необходимых для вычисления графика:
 * 1. Сброс результатов модели с помощью `ResetModelResults()`.
 * 2. Нормализация диапазона значений по оси X с помощью `NormalizeXRange()`.
 * 3. Нормализация диапазона значений по оси Y с помощью `NormalizeYRange()`.
 * 4. Проверка корректности данных модели с помощью `CheckModelData()`.
 * 5. Вычисление точек графика с помощью `CalculateHandle()`.
 *
 * Последовательность этих шагов необходима для подготовки данных и корректного расчета графика.
 */
void GraphModel::CalculateGraph() {
  ResetModelResults();
  NormalizeXRange();
  NormalizeYRange();
  CheckModelData();
  CalculateHandle();
}

}  // namespace s21