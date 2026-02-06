#pragma once

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPropertyAnimation>
#include "CalculatorMode.hpp"

namespace calc {

/**
 * @brief Всплывающее меню выбора режима калькулятора в стиле Windows
 */
class ModeMenu : public QWidget {
    Q_OBJECT

public:
    explicit ModeMenu(QWidget* parent = nullptr);
    
    /**
     * @brief Показать/скрыть меню
     */
    void toggle();
    
    /**
     * @brief Установить текущий режим
     */
    void setCurrentMode(CalculatorMode mode);
    
    /**
     * @brief Получить текущий режим
     */
    CalculatorMode currentMode() const { return currentMode_; }

signals:
    /**
     * @brief Сигнал изменения режима
     */
    void modeChanged(CalculatorMode mode);

private slots:
    void onStandardClicked();
    void onScientificClicked();
    void onProgrammerClicked();

private:
    void setupUI();
    void updateSelection();
    
    CalculatorMode currentMode_;
    bool isVisible_;
    
    QPushButton* standardButton_;
    QPushButton* scientificButton_;
    QPushButton* programmerButton_;
    
    QPropertyAnimation* slideAnimation_;
};

} // namespace calc
