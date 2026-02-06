#pragma once

#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QKeyEvent>
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

    /**
     * @brief Обработка нажатий клавиш
     */
    void keyPressEvent(QKeyEvent* event) override;

signals:
    void evaluateRequested();

protected slots:
    void onButtonClicked();
    void onClearClicked();
    void onClearEntryClicked();
    void onBackspaceClicked();
    void onEqualsClicked();
    void onDisplayTextChanged(const QString& text);

protected:
    QLineEdit* display_;
    QLabel* previewLabel_;

private:
    void setupUI();
    void createButtons();
    void calculatePreview(const QString& text);
};

} // namespace calc
