#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QGridLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QLabel>
#include "NumberConverter.hpp"
#include "CalculatorButton.hpp"

namespace calc {

/**
 * @brief Виджет программистского режима калькулятора
 */
class ProgrammerModeWidget : public QWidget {
    Q_OBJECT

public:
    explicit ProgrammerModeWidget(QWidget* parent = nullptr);
    
    /**
     * @brief Получить дисплей
     */
    QLineEdit* display() const { return display_; }
    
    /**
     * @brief Получить текущую систему счисления
     */
    NumberBase currentBase() const { return currentBase_; }

signals:
    void evaluateRequested();

private slots:
    void onButtonClicked();
    void onClearClicked();
    void onBackspaceClicked();
    void onEqualsClicked();
    void onBaseChanged(NumberBase base);
    void onDisplayTextChanged(const QString& text);

private:
    void setupUI();
    void createBaseSelector();
    void createButtons();
    void updateBaseDisplays();
    void updateButtonStates();
    
    QLineEdit* display_;
    QLabel* hexDisplay_;
    QLabel* decDisplay_;
    QLabel* octDisplay_;
    QLabel* binDisplay_;
    
    QPushButton* hexButton_;
    QPushButton* decButton_;
    QPushButton* octButton_;
    QPushButton* binButton_;
    
    NumberBase currentBase_;
    int64_t currentValue_;
    
    // Кнопки цифр для отключения в зависимости от системы
    QList<QPushButton*> digitButtons_;
};

} // namespace calc
