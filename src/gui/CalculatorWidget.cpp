#include "CalculatorWidget.hpp"
#include "../lexer.hpp"
#include "../parser.hpp"
#include "../evaluator.hpp"
#include "../error.hpp"
#include <QLabel>
#include <QMessageBox>
#include <QKeyEvent>
#include <QPushButton>
#include <QFont>
#include <QGroupBox>
#include <sstream>
#include <iomanip>

namespace calc {

CalculatorWidget::CalculatorWidget(QWidget* parent)
    : QWidget(parent) {
    setupUI();
    createButtons();
    connectSignals();
}

void CalculatorWidget::setupUI() {
    auto* mainLayout = new QVBoxLayout(this);
    
    // Дисплей
    display_ = new QLineEdit(this);
    display_->setReadOnly(true);
    display_->setAlignment(Qt::AlignRight);
    display_->setMinimumHeight(60);
    QFont displayFont = display_->font();
    displayFont.setPointSize(18);
    display_->setFont(displayFont);
    display_->setPlaceholderText("0");
    mainLayout->addWidget(display_);
    
    // Контейнер для кнопок и истории
    auto* contentLayout = new QHBoxLayout();
    
    // Сетка кнопок
    auto* buttonWidget = new QWidget(this);
    buttonLayout_ = new QGridLayout(buttonWidget);
    buttonLayout_->setSpacing(5);
    contentLayout->addWidget(buttonWidget, 3);
    
    // История вычислений
    auto* historyGroup = new QGroupBox("История", this);
    auto* historyLayout = new QVBoxLayout(historyGroup);
    history_ = new QListWidget(this);
    history_->setMaximumWidth(250);
    historyLayout->addWidget(history_);
    contentLayout->addWidget(historyGroup, 1);
    
    mainLayout->addLayout(contentLayout);
}

void CalculatorWidget::createButtons() {
    // Строка 0: Специальные функции
    auto* clearBtn = new CalculatorButton("C", CalculatorButton::ButtonType::Special, this);
    auto* ceBtn = new CalculatorButton("CE", CalculatorButton::ButtonType::Special, this);
    auto* backBtn = new CalculatorButton("⌫", CalculatorButton::ButtonType::Special, this);
    auto* divBtn = new CalculatorButton("/", CalculatorButton::ButtonType::Operator, this);
    
    buttonLayout_->addWidget(clearBtn, 0, 0);
    buttonLayout_->addWidget(ceBtn, 0, 1);
    buttonLayout_->addWidget(backBtn, 0, 2);
    buttonLayout_->addWidget(divBtn, 0, 3);
    
    buttons_["C"] = clearBtn;
    buttons_["CE"] = ceBtn;
    buttons_["⌫"] = backBtn;
    buttons_["/"] = divBtn;
    
    // Строка 1: 7, 8, 9, *
    for (int i = 0; i < 3; ++i) {
        QString digit = QString::number(7 + i);
        auto* btn = new CalculatorButton(digit, CalculatorButton::ButtonType::Digit, this);
        buttonLayout_->addWidget(btn, 1, i);
        buttons_[digit] = btn;
    }
    auto* mulBtn = new CalculatorButton("*", CalculatorButton::ButtonType::Operator, this);
    buttonLayout_->addWidget(mulBtn, 1, 3);
    buttons_["*"] = mulBtn;
    
    // Строка 2: 4, 5, 6, -
    for (int i = 0; i < 3; ++i) {
        QString digit = QString::number(4 + i);
        auto* btn = new CalculatorButton(digit, CalculatorButton::ButtonType::Digit, this);
        buttonLayout_->addWidget(btn, 2, i);
        buttons_[digit] = btn;
    }
    auto* subBtn = new CalculatorButton("-", CalculatorButton::ButtonType::Operator, this);
    buttonLayout_->addWidget(subBtn, 2, 3);
    buttons_["-"] = subBtn;
    
    // Строка 3: 1, 2, 3, +
    for (int i = 0; i < 3; ++i) {
        QString digit = QString::number(1 + i);
        auto* btn = new CalculatorButton(digit, CalculatorButton::ButtonType::Digit, this);
        buttonLayout_->addWidget(btn, 3, i);
        buttons_[digit] = btn;
    }
    auto* addBtn = new CalculatorButton("+", CalculatorButton::ButtonType::Operator, this);
    buttonLayout_->addWidget(addBtn, 3, 3);
    buttons_["+"] = addBtn;
    
    // Строка 4: 0, ., =, %
    auto* zeroBtn = new CalculatorButton("0", CalculatorButton::ButtonType::Digit, this);
    auto* dotBtn = new CalculatorButton(".", CalculatorButton::ButtonType::Digit, this);
    auto* equalBtn = new CalculatorButton("=", CalculatorButton::ButtonType::Special, this);
    auto* modBtn = new CalculatorButton("%", CalculatorButton::ButtonType::Operator, this);
    
    buttonLayout_->addWidget(zeroBtn, 4, 0);
    buttonLayout_->addWidget(dotBtn, 4, 1);
    buttonLayout_->addWidget(equalBtn, 4, 2);
    buttonLayout_->addWidget(modBtn, 4, 3);
    
    buttons_["0"] = zeroBtn;
    buttons_["."] = dotBtn;
    buttons_["="] = equalBtn;
    buttons_["%"] = modBtn;
    
    // Строка 5: Скобки и степень
    auto* lparenBtn = new CalculatorButton("(", CalculatorButton::ButtonType::Bracket, this);
    auto* rparenBtn = new CalculatorButton(")", CalculatorButton::ButtonType::Bracket, this);
    auto* powBtn = new CalculatorButton("^", CalculatorButton::ButtonType::Operator, this);
    
    buttonLayout_->addWidget(lparenBtn, 5, 0);
    buttonLayout_->addWidget(rparenBtn, 5, 1);
    buttonLayout_->addWidget(powBtn, 5, 2, 1, 2);
    
    buttons_["("] = lparenBtn;
    buttons_[")"] = rparenBtn;
    buttons_["^"] = powBtn;
    
    // Строка 6-9: Функции
    QStringList functions = {
        "sin", "cos", "tan", "asin",
        "acos", "atan", "sinh", "cosh",
        "tanh", "log", "ln", "log10",
        "exp", "sqrt", "abs", "factorial"
    };
    
    int row = 6;
    int col = 0;
    for (const QString& func : functions) {
        auto* btn = new CalculatorButton(func, CalculatorButton::ButtonType::Function, this);
        btn->setValue(func + "(");
        buttonLayout_->addWidget(btn, row, col);
        buttons_[func] = btn;
        
        col++;
        if (col >= 4) {
            col = 0;
            row++;
        }
    }
    
    // Строка 10: Константы и дополнительные функции
    auto* piBtn = new CalculatorButton("π", CalculatorButton::ButtonType::Constant, this);
    piBtn->setValue("pi");
    auto* eBtn = new CalculatorButton("e", CalculatorButton::ButtonType::Constant, this);
    eBtn->setValue("e");
    auto* ceilBtn = new CalculatorButton("ceil", CalculatorButton::ButtonType::Function, this);
    ceilBtn->setValue("ceil(");
    auto* floorBtn = new CalculatorButton("floor", CalculatorButton::ButtonType::Function, this);
    floorBtn->setValue("floor(");
    
    buttonLayout_->addWidget(piBtn, 10, 0);
    buttonLayout_->addWidget(eBtn, 10, 1);
    buttonLayout_->addWidget(ceilBtn, 10, 2);
    buttonLayout_->addWidget(floorBtn, 10, 3);
    
    buttons_["π"] = piBtn;
    buttons_["e"] = eBtn;
    buttons_["ceil"] = ceilBtn;
    buttons_["floor"] = floorBtn;
    
    // Строка 11: round
    auto* roundBtn = new CalculatorButton("round", CalculatorButton::ButtonType::Function, this);
    roundBtn->setValue("round(");
    buttonLayout_->addWidget(roundBtn, 11, 0, 1, 2);
    buttons_["round"] = roundBtn;
}

void CalculatorWidget::connectSignals() {
    // Подключить все кнопки кроме специальных
    for (auto it = buttons_.begin(); it != buttons_.end(); ++it) {
        CalculatorButton* btn = it.value();
        
        if (btn->text() == "=") {
            connect(btn, &QPushButton::clicked, this, &CalculatorWidget::onEqualClicked);
        } else if (btn->text() == "C") {
            connect(btn, &QPushButton::clicked, this, &CalculatorWidget::onClearClicked);
        } else if (btn->text() == "CE") {
            connect(btn, &QPushButton::clicked, this, &CalculatorWidget::onClearEntryClicked);
        } else if (btn->text() == "⌫") {
            connect(btn, &QPushButton::clicked, this, &CalculatorWidget::onBackspaceClicked);
        } else {
            connect(btn, &QPushButton::clicked, this, &CalculatorWidget::onButtonClicked);
        }
    }
}

void CalculatorWidget::onButtonClicked() {
    auto* btn = qobject_cast<CalculatorButton*>(sender());
    if (!btn) return;
    
    QString value = btn->value();
    currentExpression_ += value;
    display_->setText(currentExpression_);
}

void CalculatorWidget::onEqualClicked() {
    if (currentExpression_.isEmpty()) return;
    
    QString result = evaluateExpression(currentExpression_);
    
    if (!result.startsWith("Ошибка")) {
        addToHistory(currentExpression_, result);
        currentExpression_ = result;
        display_->setText(result);
    } else {
        display_->setText(result);
    }
}

void CalculatorWidget::onClearClicked() {
    currentExpression_.clear();
    display_->clear();
    display_->setPlaceholderText("0");
}

void CalculatorWidget::onClearEntryClicked() {
    // Удалить последнее число или операцию
    if (currentExpression_.isEmpty()) return;
    
    // Простая реализация: удалить все до последнего оператора
    int lastOpIndex = -1;
    for (int i = currentExpression_.length() - 1; i >= 0; --i) {
        QChar c = currentExpression_[i];
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^' || c == '(') {
            lastOpIndex = i;
            break;
        }
    }
    
    if (lastOpIndex >= 0) {
        currentExpression_ = currentExpression_.left(lastOpIndex + 1);
    } else {
        currentExpression_.clear();
    }
    
    display_->setText(currentExpression_);
}

void CalculatorWidget::onBackspaceClicked() {
    if (!currentExpression_.isEmpty()) {
        currentExpression_.chop(1);
        display_->setText(currentExpression_);
    }
}

void CalculatorWidget::addToHistory(const QString& expression, const QString& result) {
    QString historyItem = expression + " = " + result;
    history_->insertItem(0, historyItem);
    
    // Ограничить историю 50 элементами
    while (history_->count() > 50) {
        delete history_->takeItem(history_->count() - 1);
    }
}

QString CalculatorWidget::evaluateExpression(const QString& expression) {
    try {
        std::string expr = expression.toStdString();
        
        Lexer lexer(expr);
        auto tokens = lexer.tokenize();
        
        Parser parser(tokens);
        auto ast = parser.parse();
        
        Evaluator evaluator;
        double result = evaluator.evaluate(ast);
        
        // Форматировать результат
        std::ostringstream oss;
        oss << std::setprecision(10) << result;
        std::string resultStr = oss.str();
        
        // Убрать лишние нули после точки
        if (resultStr.find('.') != std::string::npos) {
            resultStr.erase(resultStr.find_last_not_of('0') + 1, std::string::npos);
            if (resultStr.back() == '.') {
                resultStr.pop_back();
            }
        }
        
        return QString::fromStdString(resultStr);
    } catch (const ParseError& e) {
        return QString("Ошибка: %1").arg(e.what());
    } catch (const EvalError& e) {
        return QString("Ошибка: %1").arg(e.what());
    } catch (const std::exception& e) {
        return QString("Ошибка: %1").arg(e.what());
    }
}

} // namespace calc
