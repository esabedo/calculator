#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include "CalculatorMode.hpp"
#include "ModeMenu.hpp"
#include "StandardModeWidget.hpp"
#include "ScientificModeWidget.hpp"
#include "ProgrammerModeWidget.hpp"

namespace calc {

/**
 * @brief Главный виджет калькулятора в стиле Windows
 */
class CalculatorWidget : public QWidget {
    Q_OBJECT

public:
    explicit CalculatorWidget(QWidget* parent = nullptr);

private slots:
    void onModeChanged(CalculatorMode mode);
    void onMenuButtonClicked();
    void onEvaluateRequested();

private:
    void setupUI();
    void evaluateExpression(const QString& expression);
    QString getModeTitle(CalculatorMode mode) const;
    
    // UI компоненты
    QPushButton* menuButton_;
    QLabel* modeTitle_;
    ModeMenu* modeMenu_;
    QStackedWidget* modeStack_;
    
    // Виджеты режимов
    StandardModeWidget* standardWidget_;
    ScientificModeWidget* scientificWidget_;
    ProgrammerModeWidget* programmerWidget_;
    
    CalculatorMode currentMode_;
};

} // namespace calc
