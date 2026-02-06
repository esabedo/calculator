#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QGridLayout>
#include "CalculatorMode.hpp"
#include "CalculatorButton.hpp"

namespace calc {

/**
 * @brief Виджет обычного режима калькулятора
 */
class StandardModeWidget : public QWidget {
    Q_OBJECT

public:
    explicit StandardModeWidget(QWidget* parent = nullptr);
    
    /**
     * @brief Получить дисплей
     */
    QLineEdit* display() const { return display_; }

signals:
    void evaluateRequested();

protected slots:
    void onButtonClicked();
    void onClearClicked();
    void onClearEntryClicked();
    void onBackspaceClicked();
    void onEqualsClicked();

private:
    void setupUI();
    void createButtons();
    
    QLineEdit* display_;
};

} // namespace calc
