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
#include <QPropertyAnimation>
#include <sstream>
#include <iomanip>

namespace calc {

CalculatorWidget::CalculatorWidget(QWidget* parent)
    : QWidget(parent), isEngineeringMode_(false) {
    setupUI();
    createBasicButtons();
    createEngineeringButtons();
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
    
    // Левая панель: кнопки
    auto* leftPanel = new QWidget(this);
    auto* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    
    // Кнопка переключения инженерного режима
    toggleEngineeringButton_ = new QPushButton("Инженерный режим ▼", this);
    toggleEngineeringButton_->setCheckable(true);
    toggleEngineeringButton_->setMinimumHeight(35);
    connect(toggleEngineeringButton_, &QPushButton::clicked, this, &CalculatorWidget::onToggleEngineering);
    leftLayout->addWidget(toggleEngineeringButton_);
    
    // Базовая сетка кнопок
    auto* basicWidget = new QWidget(this);
    auto* basicLayout = new QGridLayout(basicWidget);
    basicLayout->setSpacing(5);
    basicLayout->setObjectName("basicLayout");
    leftLayout->addWidget(basicWidget);
    
    // Инженерная панель (скрыта по умолчанию)
    engineeringPanel_ = new QWidget(this);
    auto* engLayout = new QGridLayout(engineeringPanel_);
    engLayout->setSpacing(5);
    engLayout->setObjectName("engineeringLayout");
    engineeringPanel_->setMaximumHeight(0);
    engineeringPanel_->setVisible(false);
    leftLayout->addWidget(engineeringPanel_);
    
    contentLayout->addWidget(leftPanel, 3);
    
    // История вычислений
    auto* historyGroup = new QGroupBox("История", this);
    auto* historyLayout = new QVBoxLayout(historyGroup);
    history_ = new QListWidget(this);
    history_->setMaximumWidth(250);
    historyLayout->addWidget(history_);
    contentLayout->addWidget(historyGroup, 1);
    
    mainLayout->addLayout(contentLayout);
}

void CalculatorWidget::createBasicButtons() {
    auto* basicLayout = findChild<QGridLayout*>("basicLayout");
    if (!basicLayout) return;
    
    // Строка 0: C, CE, ⌫, /
    auto* clearBtn = new CalculatorButton("C", CalculatorButton::ButtonType::Special, this);
    auto* ceBtn = new CalculatorButton("CE", CalculatorButton::ButtonType::Special, this);
    auto* backBtn = new CalculatorButton("⌫", CalculatorButton::ButtonType::Special, this);
    auto* divBtn = new CalculatorButton("/", CalculatorButton::ButtonType::Operator, this);
    
    basicLayout->addWidget(clearBtn, 0, 0);
    basicLayout->addWidget(ceBtn, 0, 1);
    basicLayout->addWidget(backBtn, 0, 2);
    basicLayout->addWidget(divBtn, 0, 3);
    
    connect(clearBtn, &QPushButton::clicked, this, &CalculatorWidget::onClearClicked);
    connect(ceBtn, &QPushButton::clicked, this, &CalculatorWidget::onClearEntryClicked);
    connect(backBtn, &QPushButton::clicked, this, &CalculatorWidget::onBackspaceClicked);
    connect(divBtn, &QPushButton::clicked, this, &CalculatorWidget::onButtonClicked);
    
    // Строка 1: 7, 8, 9, *
    for (int i = 0; i < 3; ++i) {
        auto* btn = new CalculatorButton(QString::number(7 + i), CalculatorButton::ButtonType::Digit, this);
        basicLayout->addWidget(btn, 1, i);
        connect(btn, &QPushButton::clicked, this, &CalculatorWidget::onButtonClicked);
    }
    auto* mulBtn = new CalculatorButton("*", CalculatorButton::ButtonType::Operator, this);
    basicLayout->addWidget(mulBtn, 1, 3);
    connect(mulBtn, &QPushButton::clicked, this, &CalculatorWidget::onButtonClicked);
    
    // Строка 2: 4, 5, 6, -
    for (int i = 0; i < 3; ++i) {
        auto* btn = new CalculatorButton(QString::number(4 + i), CalculatorButton::ButtonType::Digit, this);
        basicLayout->addWidget(btn, 2, i);
        connect(btn, &QPushButton::clicked, this, &CalculatorWidget::onButtonClicked);
    }
    auto* subBtn = new CalculatorButton("-", CalculatorButton::ButtonType::Operator, this);
    basicLayout->addWidget(subBtn, 2, 3);
    connect(subBtn, &QPushButton::clicked, this, &CalculatorWidget::onButtonClicked);
    
    // Строка 3: 1, 2, 3, +
    for (int i = 0; i < 3; ++i) {
        auto* btn = new CalculatorButton(QString::number(1 + i), CalculatorButton::ButtonType::Digit, this);
        basicLayout->addWidget(btn, 3, i);
        connect(btn, &QPushButton::clicked, this, &CalculatorWidget::onButtonClicked);
    }
    auto* addBtn = new CalculatorButton("+", CalculatorButton::ButtonType::Operator, this);
    basicLayout->addWidget(addBtn, 3, 3);
    connect(addBtn, &QPushButton::clicked, this, &CalculatorWidget::onButtonClicked);
    
    // Строка 4: 0, ., =, %
    auto* zeroBtn = new CalculatorButton("0", CalculatorButton::ButtonType::Digit, this);
    auto* dotBtn = new CalculatorButton(".", CalculatorButton::ButtonType::Digit, this);
    auto* equalBtn = new CalculatorButton("=", CalculatorButton::ButtonType::Special, this);
    auto* modBtn = new CalculatorButton("%", CalculatorButton::ButtonType::Operator, this);
    
    basicLayout->addWidget(zeroBtn, 4, 0);
    basicLayout->addWidget(dotBtn, 4, 1);
    basicLayout->addWidget(equalBtn, 4, 2);
    basicLayout->addWidget(modBtn, 4, 3);
    
    connect(zeroBtn, &QPushButton::clicked, this, &CalculatorWidget::onButtonClicked);
    connect(dotBtn, &QPushButton::clicked, this, &CalculatorWidget::onButtonClicked);
    connect(equalBtn, &QPushButton::clicked, this, &CalculatorWidget::onEqualsClicked);
    connect(modBtn, &QPushButton::clicked, this, &CalculatorWidget::onButtonClicked);
    
    // Строка 5: (, ), ^
    auto* lparenBtn = new CalculatorButton("(", CalculatorButton::ButtonType::Bracket, this);
    auto* rparenBtn = new CalculatorButton(")", CalculatorButton::ButtonType::Bracket, this);
    auto* powBtn = new CalculatorButton("^", CalculatorButton::ButtonType::Operator, this);
    
    basicLayout->addWidget(lparenBtn, 5, 0);
    basicLayout->addWidget(rparenBtn, 5, 1);
    basicLayout->addWidget(powBtn, 5, 2, 1, 2);
    
    connect(lparenBtn, &QPushButton::clicked, this, &CalculatorWidget::onButtonClicked);
    connect(rparenBtn, &QPushButton::clicked, this, &CalculatorWidget::onButtonClicked);
    connect(powBtn, &QPushButton::clicked, this, &CalculatorWidget::onButtonClicked);
}

void CalculatorWidget::createEngineeringButtons() {
    auto* engLayout = findChild<QGridLayout*>("engineeringLayout");
    if (!engLayout) return;
    
    // Тригонометрические функции
    QStringList trigFuncs = {"sin", "cos", "tan", "asin", "acos", "atan"};
    for (int i = 0; i < trigFuncs.size(); ++i) {
        auto* btn = new CalculatorButton(trigFuncs[i], CalculatorButton::ButtonType::Function, this);
        btn->setValue(trigFuncs[i] + "(");
        engLayout->addWidget(btn, 0, i);
        connect(btn, &QPushButton::clicked, this, &CalculatorWidget::onButtonClicked);
    }
    
    // Гиперболические функции
    QStringList hypFuncs = {"sinh", "cosh", "tanh"};
    for (int i = 0; i < hypFuncs.size(); ++i) {
        auto* btn = new CalculatorButton(hypFuncs[i], CalculatorButton::ButtonType::Function, this);
        btn->setValue(hypFuncs[i] + "(");
        engLayout->addWidget(btn, 1, i);
        connect(btn, &QPushButton::clicked, this, &CalculatorWidget::onButtonClicked);
    }
    
    // Логарифмические функции
    QStringList logFuncs = {"log", "ln", "log10"};
    for (int i = 0; i < logFuncs.size(); ++i) {
        auto* btn = new CalculatorButton(logFuncs[i], CalculatorButton::ButtonType::Function, this);
        btn->setValue(logFuncs[i] + "(");
        engLayout->addWidget(btn, 1, i + 3);
        connect(btn, &QPushButton::clicked, this, &CalculatorWidget::onButtonClicked);
    }
    
    // Дополнительные функции
    QStringList extraFuncs = {"exp", "sqrt", "abs", "ceil", "floor", "round"};
    for (int i = 0; i < extraFuncs.size(); ++i) {
        auto* btn = new CalculatorButton(extraFuncs[i], CalculatorButton::ButtonType::Function, this);
        btn->setValue(extraFuncs[i] + "(");
        engLayout->addWidget(btn, 2, i);
        connect(btn, &QPushButton::clicked, this, &CalculatorWidget::onButtonClicked);
    }
    
    // Факториал и константы
    auto* factBtn = new CalculatorButton("n!", CalculatorButton::ButtonType::Function, this);
    factBtn->setValue("factorial(");
    auto* piBtn = new CalculatorButton("π", CalculatorButton::ButtonType::Constant, this);
    piBtn->setValue("pi");
    auto* eBtn = new CalculatorButton("e", CalculatorButton::ButtonType::Constant, this);
    eBtn->setValue("e");
    
    engLayout->addWidget(factBtn, 3, 0);
    engLayout->addWidget(piBtn, 3, 1);
    engLayout->addWidget(eBtn, 3, 2);
    
    connect(factBtn, &QPushButton::clicked, this, &CalculatorWidget::onButtonClicked);
    connect(piBtn, &QPushButton::clicked, this, &CalculatorWidget::onButtonClicked);
    connect(eBtn, &QPushButton::clicked, this, &CalculatorWidget::onButtonClicked);
}

void CalculatorWidget::onToggleEngineering() {
    isEngineeringMode_ = !isEngineeringMode_;
    
    // Анимация высоты панели
    QPropertyAnimation* animation = new QPropertyAnimation(engineeringPanel_, "maximumHeight");
    animation->setDuration(300);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    
    if (isEngineeringMode_) {
        engineeringPanel_->setVisible(true);
        animation->setStartValue(0);
        animation->setEndValue(200);
        toggleEngineeringButton_->setText("Инженерный режим ▲");
    } else {
        animation->setStartValue(200);
        animation->setEndValue(0);
        toggleEngineeringButton_->setText("Инженерный режим ▼");
        connect(animation, &QPropertyAnimation::finished, [this]() {
            engineeringPanel_->setVisible(false);
        });
    }
    
    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void CalculatorWidget::onButtonClicked() {
    auto* btn = qobject_cast<CalculatorButton*>(sender());
    if (!btn) return;
    
    QString value = btn->value();
    display_->setText(display_->text() + value);
}

void CalculatorWidget::onEqualsClicked() {
    QString expression = display_->text();
    if (expression.isEmpty()) return;
    
    evaluateExpression();
}

void CalculatorWidget::onClearClicked() {
    display_->clear();
    display_->setPlaceholderText("0");
}

void CalculatorWidget::onClearEntryClicked() {
    QString text = display_->text();
    if (text.isEmpty()) return;
    
    // Удалить последнее число или операцию
    int lastOpIndex = -1;
    for (int i = text.length() - 1; i >= 0; --i) {
        QChar c = text[i];
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^' || c == '(') {
            lastOpIndex = i;
            break;
        }
    }
    
    if (lastOpIndex >= 0) {
        display_->setText(text.left(lastOpIndex + 1));
    } else {
        display_->clear();
    }
}

void CalculatorWidget::onBackspaceClicked() {
    QString text = display_->text();
    if (!text.isEmpty()) {
        text.chop(1);
        display_->setText(text);
    }
}

void CalculatorWidget::evaluateExpression() {
    QString expression = display_->text();
    
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
        
        QString resultQStr = QString::fromStdString(resultStr);
        
        // Добавить в историю
        QString historyItem = expression + " = " + resultQStr;
        history_->insertItem(0, historyItem);
        
        // Ограничить историю 50 элементами
        while (history_->count() > 50) {
            delete history_->takeItem(history_->count() - 1);
        }
        
        display_->setText(resultQStr);
    } catch (const ParseError& e) {
        display_->setText(QString("Ошибка: %1").arg(e.what()));
    } catch (const EvalError& e) {
        display_->setText(QString("Ошибка: %1").arg(e.what()));
    } catch (const std::exception& e) {
        display_->setText(QString("Ошибка: %1").arg(e.what()));
    }
}

} // namespace calc
