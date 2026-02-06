#pragma once

#include "StandardModeWidget.hpp"

namespace calc {

/**
 * @brief Виджет инженерного режима калькулятора
 * Расширяет обычный режим дополнительными функциями
 */
class ScientificModeWidget : public StandardModeWidget {
    Q_OBJECT

public:
    explicit ScientificModeWidget(QWidget* parent = nullptr);

private:
    void createScientificButtons();
    
    QWidget* scientificPanel_;
};

} // namespace calc
