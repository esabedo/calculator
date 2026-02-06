#include "CalculatorButton.hpp"
#include <QStyle>
#include <QVariant>

namespace calc {

CalculatorButton::CalculatorButton(const QString& text, ButtonType type, QWidget* parent)
    : QPushButton(text, parent), type_(type), value_(text) {
    updateStyle();
    setMinimumSize(60, 50);
    setFocusPolicy(Qt::StrongFocus);
}

void CalculatorButton::setType(ButtonType type) {
    type_ = type;
    updateStyle();
}

void CalculatorButton::setValue(const QString& value) {
    value_ = value;
}

void CalculatorButton::updateStyle() {
    QString styleClass;
    
    switch (type_) {
        case ButtonType::Digit:
            setProperty("buttonType", QVariant(QString("digit")));
            break;
        case ButtonType::Operator:
            setProperty("buttonType", QVariant(QString("operator")));
            break;
        case ButtonType::Function:
            setProperty("buttonType", QVariant(QString("function")));
            break;
        case ButtonType::Constant:
            setProperty("buttonType", QVariant(QString("constant")));
            break;
        case ButtonType::Special:
            setProperty("buttonType", QVariant(QString("special")));
            break;
        case ButtonType::Bracket:
            setProperty("buttonType", QVariant(QString("bracket")));
            break;
    }
    
    // Обновить стиль
    style()->unpolish(this);
    style()->polish(this);
}

} // namespace calc
