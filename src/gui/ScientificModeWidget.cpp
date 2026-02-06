#include "ScientificModeWidget.hpp"
#include <QVBoxLayout>
#include <QGridLayout>

namespace calc {

ScientificModeWidget::ScientificModeWidget(QWidget* parent)
    : StandardModeWidget(parent) {
    
    // Добавить панель с научными функциями
    auto* mainLayout = qobject_cast<QVBoxLayout*>(layout());
    if (!mainLayout) return;
    
    scientificPanel_ = new QWidget(this);
    auto* sciLayout = new QGridLayout(scientificPanel_);
    sciLayout->setSpacing(4);
    sciLayout->setObjectName("scientificGrid");
    
    // Вставить научную панель перед кнопками
    mainLayout->insertWidget(1, scientificPanel_);
    
    createScientificButtons();
}

void ScientificModeWidget::createScientificButtons() {
    auto* sciLayout = findChild<QGridLayout*>("scientificGrid");
    if (!sciLayout) return;
    
    // Строка 0: Тригонометрические функции
    QStringList trigFuncs = {"sin", "cos", "tan", "asin", "acos", "atan"};
    for (int i = 0; i < trigFuncs.size(); ++i) {
        auto* btn = new CalculatorButton(trigFuncs[i], CalculatorButton::ButtonType::Function, this);
        btn->setValue(trigFuncs[i] + "(");
        sciLayout->addWidget(btn, 0, i);
        connect(btn, &QPushButton::clicked, this, &ScientificModeWidget::onButtonClicked);
    }
    
    // Строка 1: Гиперболические и логарифмы
    QStringList row1Funcs = {"sinh", "cosh", "tanh", "log", "ln", "log10"};
    for (int i = 0; i < row1Funcs.size(); ++i) {
        auto* btn = new CalculatorButton(row1Funcs[i], CalculatorButton::ButtonType::Function, this);
        btn->setValue(row1Funcs[i] + "(");
        sciLayout->addWidget(btn, 1, i);
        connect(btn, &QPushButton::clicked, this, &ScientificModeWidget::onButtonClicked);
    }
    
    // Строка 2: Дополнительные функции
    QStringList row2Funcs = {"exp", "abs", "ceil", "floor", "round", "n!"};
    for (int i = 0; i < row2Funcs.size(); ++i) {
        auto* btn = new CalculatorButton(row2Funcs[i], CalculatorButton::ButtonType::Function, this);
        if (row2Funcs[i] == "n!") {
            btn->setValue("factorial(");
        } else {
            btn->setValue(row2Funcs[i] + "(");
        }
        sciLayout->addWidget(btn, 2, i);
        connect(btn, &QPushButton::clicked, this, &ScientificModeWidget::onButtonClicked);
    }
    
    // Строка 3: Константы и скобки
    auto* piBtn = new CalculatorButton("π", CalculatorButton::ButtonType::Constant, this);
    piBtn->setValue("pi");
    auto* eBtn = new CalculatorButton("e", CalculatorButton::ButtonType::Constant, this);
    eBtn->setValue("e");
    auto* lparenBtn = new CalculatorButton("(", CalculatorButton::ButtonType::Bracket, this);
    auto* rparenBtn = new CalculatorButton(")", CalculatorButton::ButtonType::Bracket, this);
    auto* powBtn = new CalculatorButton("x^y", CalculatorButton::ButtonType::Operator, this);
    powBtn->setValue("^");
    
    sciLayout->addWidget(piBtn, 3, 0);
    sciLayout->addWidget(eBtn, 3, 1);
    sciLayout->addWidget(lparenBtn, 3, 2);
    sciLayout->addWidget(rparenBtn, 3, 3);
    sciLayout->addWidget(powBtn, 3, 4, 1, 2);
    
    connect(piBtn, &QPushButton::clicked, this, &ScientificModeWidget::onButtonClicked);
    connect(eBtn, &QPushButton::clicked, this, &ScientificModeWidget::onButtonClicked);
    connect(lparenBtn, &QPushButton::clicked, this, &ScientificModeWidget::onButtonClicked);
    connect(rparenBtn, &QPushButton::clicked, this, &ScientificModeWidget::onButtonClicked);
    connect(powBtn, &QPushButton::clicked, this, &ScientificModeWidget::onButtonClicked);
}

} // namespace calc
