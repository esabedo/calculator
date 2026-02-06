#pragma once

#include <QPushButton>
#include <QString>

namespace calc {

/**
 * @brief Кастомная кнопка калькулятора с поддержкой различных типов
 */
class CalculatorButton : public QPushButton {
    Q_OBJECT

public:
    enum class ButtonType {
        Digit,      // Цифры 0-9
        Operator,   // Базовые операции: +, -, *, /, %
        Function,   // Математические функции
        Constant,   // Константы: π, e
        Special,    // Специальные: =, C, CE, Backspace
        Bracket     // Скобки: (, )
    };

    explicit CalculatorButton(const QString& text, ButtonType type, QWidget* parent = nullptr);
    
    ButtonType type() const { return type_; }
    QString value() const { return value_; }
    
    void setType(ButtonType type);
    void setValue(const QString& value);

protected:
    void updateStyle();

private:
    ButtonType type_;
    QString value_;
};

} // namespace calc
