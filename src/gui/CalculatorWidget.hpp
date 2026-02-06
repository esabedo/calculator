#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QString>
#include <memory>
#include "CalculatorButton.hpp"

namespace calc {

/**
 * @brief Главный виджет калькулятора
 */
class CalculatorWidget : public QWidget {
    Q_OBJECT

public:
    explicit CalculatorWidget(QWidget* parent = nullptr);

private slots:
    void onButtonClicked();
    void onEqualClicked();
    void onClearClicked();
    void onClearEntryClicked();
    void onBackspaceClicked();

private:
    void setupUI();
    void createButtons();
    void connectSignals();
    void addToHistory(const QString& expression, const QString& result);
    QString evaluateExpression(const QString& expression);
    
    // UI компоненты
    QLineEdit* display_;
    QListWidget* history_;
    QGridLayout* buttonLayout_;
    
    // Кнопки
    QMap<QString, CalculatorButton*> buttons_;
    
    // Текущее выражение
    QString currentExpression_;
};

} // namespace calc
