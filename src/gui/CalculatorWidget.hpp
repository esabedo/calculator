#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QString>
#include "CalculatorButton.hpp"

namespace calc {

/**
 * @brief Главный виджет калькулятора с базовой и инженерной панелями
 */
class CalculatorWidget : public QWidget {
    Q_OBJECT

public:
    explicit CalculatorWidget(QWidget* parent = nullptr);

private slots:
    void onButtonClicked();
    void onClearClicked();
    void onClearEntryClicked();
    void onBackspaceClicked();
    void onEqualsClicked();
    void onToggleEngineering();

private:
    void setupUI();
    void createBasicButtons();
    void createEngineeringButtons();
    void evaluateExpression();
    
    QLineEdit* display_;
    QListWidget* history_;
    QWidget* engineeringPanel_;
    QPushButton* toggleEngineeringButton_;
    bool isEngineeringMode_;
};

} // namespace calc
