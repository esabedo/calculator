#include "StandardModeWidget.hpp"
#include <QVBoxLayout>
#include <QApplication>
#include <sstream>
#include <iomanip>
#include "../lexer.hpp"
#include "../parser.hpp"
#include "../evaluator.hpp"
#include "../error.hpp"

namespace calc {

StandardModeWidget::StandardModeWidget(QWidget* parent)
    : QWidget(parent) {
    setupUI();
    createButtons();
}

void StandardModeWidget::setupUI() {
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    
    // Контейнер дисплея
    auto* displayContainer = new QWidget(this);
    auto* displayLayout = new QVBoxLayout(displayContainer);
    displayLayout->setContentsMargins(0, 0, 0, 0);
    displayLayout->setSpacing(0);
    
    // Дисплей
    display_ = new QLineEdit(this);
    display_->setReadOnly(false); // Разрешить ввод
    display_->setAlignment(Qt::AlignRight);
    display_->setMinimumHeight(60); // Чуть меньше, чтобы место для превью было
    display_->setPlaceholderText("0");
    QFont displayFont = display_->font();
    displayFont.setPointSize(24);
    displayFont.setBold(true);
    display_->setFont(displayFont);
    
    connect(display_, &QLineEdit::textChanged, this, &StandardModeWidget::onDisplayTextChanged);
    connect(display_, &QLineEdit::returnPressed, this, &StandardModeWidget::onEqualsClicked);
    
    displayLayout->addWidget(display_);
    
    // Превью результата
    previewLabel_ = new QLabel("", this);
    previewLabel_->setAlignment(Qt::AlignRight);
    previewLabel_->setObjectName("previewLabel");
    previewLabel_->setFixedHeight(30);
    QFont previewFont = previewLabel_->font();
    previewFont.setPointSize(12);
    previewLabel_->setFont(previewFont);
    previewLabel_->setStyleSheet("color: #888888;"); // Серый цвет по умолчанию
    
    displayLayout->addWidget(previewLabel_);
    
    mainLayout->addWidget(displayContainer);
    
    // Сетка кнопок
    auto* buttonGrid = new QWidget(this);
    auto* gridLayout = new QGridLayout(buttonGrid);
    gridLayout->setSpacing(4);
    gridLayout->setObjectName("buttonGrid");
    mainLayout->addWidget(buttonGrid);
}

void StandardModeWidget::createButtons() {
    auto* gridLayout = findChild<QGridLayout*>("buttonGrid");
    if (!gridLayout) return;
    
    // Строка 0: %, CE, C, ⌫
    auto* percentBtn = new CalculatorButton("%", CalculatorButton::ButtonType::Operator, this);
    auto* ceBtn = new CalculatorButton("CE", CalculatorButton::ButtonType::Special, this);
    auto* clearBtn = new CalculatorButton("C", CalculatorButton::ButtonType::Special, this);
    auto* backBtn = new CalculatorButton("⌫", CalculatorButton::ButtonType::Special, this);
    
    gridLayout->addWidget(percentBtn, 0, 0);
    gridLayout->addWidget(ceBtn, 0, 1);
    gridLayout->addWidget(clearBtn, 0, 2);
    gridLayout->addWidget(backBtn, 0, 3);
    
    connect(percentBtn, &QPushButton::clicked, this, &StandardModeWidget::onButtonClicked);
    connect(ceBtn, &QPushButton::clicked, this, &StandardModeWidget::onClearEntryClicked);
    connect(clearBtn, &QPushButton::clicked, this, &StandardModeWidget::onClearClicked);
    connect(backBtn, &QPushButton::clicked, this, &StandardModeWidget::onBackspaceClicked);
    
    // Строка 1: 1/x, x², √, /
    auto* reciprocalBtn = new CalculatorButton("1/x", CalculatorButton::ButtonType::Function, this);
    reciprocalBtn->setValue("1/(");
    auto* squareBtn = new CalculatorButton("x²", CalculatorButton::ButtonType::Function, this);
    squareBtn->setValue("^2");
    auto* sqrtBtn = new CalculatorButton("√", CalculatorButton::ButtonType::Function, this);
    sqrtBtn->setValue("sqrt(");
    auto* divBtn = new CalculatorButton("/", CalculatorButton::ButtonType::Operator, this);
    
    gridLayout->addWidget(reciprocalBtn, 1, 0);
    gridLayout->addWidget(squareBtn, 1, 1);
    gridLayout->addWidget(sqrtBtn, 1, 2);
    gridLayout->addWidget(divBtn, 1, 3);
    
    connect(reciprocalBtn, &QPushButton::clicked, this, &StandardModeWidget::onButtonClicked);
    connect(squareBtn, &QPushButton::clicked, this, &StandardModeWidget::onButtonClicked);
    connect(sqrtBtn, &QPushButton::clicked, this, &StandardModeWidget::onButtonClicked);
    connect(divBtn, &QPushButton::clicked, this, &StandardModeWidget::onButtonClicked);
    
    // Строка 2: 7, 8, 9, *
    for (int i = 0; i < 3; ++i) {
        auto* btn = new CalculatorButton(QString::number(7 + i), CalculatorButton::ButtonType::Digit, this);
        gridLayout->addWidget(btn, 2, i);
        connect(btn, &QPushButton::clicked, this, &StandardModeWidget::onButtonClicked);
    }
    auto* mulBtn = new CalculatorButton("×", CalculatorButton::ButtonType::Operator, this);
    mulBtn->setValue("*");
    gridLayout->addWidget(mulBtn, 2, 3);
    connect(mulBtn, &QPushButton::clicked, this, &StandardModeWidget::onButtonClicked);
    
    // Строка 3: 4, 5, 6, -
    for (int i = 0; i < 3; ++i) {
        auto* btn = new CalculatorButton(QString::number(4 + i), CalculatorButton::ButtonType::Digit, this);
        gridLayout->addWidget(btn, 3, i);
        connect(btn, &QPushButton::clicked, this, &StandardModeWidget::onButtonClicked);
    }
    auto* subBtn = new CalculatorButton("−", CalculatorButton::ButtonType::Operator, this);
    subBtn->setValue("-");
    gridLayout->addWidget(subBtn, 3, 3);
    connect(subBtn, &QPushButton::clicked, this, &StandardModeWidget::onButtonClicked);
    
    // Строка 4: 1, 2, 3, +
    for (int i = 0; i < 3; ++i) {
        auto* btn = new CalculatorButton(QString::number(1 + i), CalculatorButton::ButtonType::Digit, this);
        gridLayout->addWidget(btn, 4, i);
        connect(btn, &QPushButton::clicked, this, &StandardModeWidget::onButtonClicked);
    }
    auto* addBtn = new CalculatorButton("+", CalculatorButton::ButtonType::Operator, this);
    gridLayout->addWidget(addBtn, 4, 3);
    connect(addBtn, &QPushButton::clicked, this, &StandardModeWidget::onButtonClicked);
    
    // Строка 5: +/-, 0, ., =
    auto* negateBtn = new CalculatorButton("+/−", CalculatorButton::ButtonType::Digit, this);
    negateBtn->setValue("-");
    auto* zeroBtn = new CalculatorButton("0", CalculatorButton::ButtonType::Digit, this);
    auto* dotBtn = new CalculatorButton(".", CalculatorButton::ButtonType::Digit, this);
    auto* equalBtn = new CalculatorButton("=", CalculatorButton::ButtonType::Special, this);
    
    gridLayout->addWidget(negateBtn, 5, 0);
    gridLayout->addWidget(zeroBtn, 5, 1);
    gridLayout->addWidget(dotBtn, 5, 2);
    gridLayout->addWidget(equalBtn, 5, 3);
    
    connect(negateBtn, &QPushButton::clicked, this, &StandardModeWidget::onButtonClicked);
    connect(zeroBtn, &QPushButton::clicked, this, &StandardModeWidget::onButtonClicked);
    connect(dotBtn, &QPushButton::clicked, this, &StandardModeWidget::onButtonClicked);
    connect(equalBtn, &QPushButton::clicked, this, &StandardModeWidget::onEqualsClicked);
}

void StandardModeWidget::onButtonClicked() {
    auto* btn = qobject_cast<CalculatorButton*>(sender());
    if (!btn) return;
    
    QString value = btn->value();
    display_->setText(display_->text() + value);
    display_->setFocus();
}

void StandardModeWidget::onClearClicked() {
    display_->clear();
    display_->setPlaceholderText("0");
    previewLabel_->clear();
}

void StandardModeWidget::onClearEntryClicked() {
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

void StandardModeWidget::onBackspaceClicked() {
    QString text = display_->text();
    if (!text.isEmpty()) {
        text.chop(1);
        display_->setText(text);
    }
}

void StandardModeWidget::onEqualsClicked() {
    emit evaluateRequested();
    previewLabel_->clear();
}

void StandardModeWidget::onDisplayTextChanged(const QString& text) {
    calculatePreview(text);
}

void StandardModeWidget::calculatePreview(const QString& text) {
    if (text.isEmpty()) {
        previewLabel_->clear();
        return;
    }
    
    try {
        Lexer lexer(text.toStdString());
        auto tokens = lexer.tokenize();
        Parser parser(tokens);
        auto ast = parser.parse();
        Evaluator evaluator;
        double result = evaluator.evaluate(ast);
        
        // Форматируем результат
        std::stringstream ss;
        ss << std::defaultfloat << result;
        previewLabel_->setText(QString::fromStdString(ss.str()));
    } catch (...) {
        // Если ошибка парсинга или вычисления, просто не показываем превью
        // или очищаем его, если оно было
        // previewLabel_->clear(); // Можно не очищать, чтобы видеть последний валидный результат, но калькулятор Windows очищает или не показывает
        previewLabel_->clear();
    }
}

void StandardModeWidget::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        onEqualsClicked();
        return;
    }
    
    // Если фокус не на дисплее, перенаправляем ввод в дисплей
    if (display_ && !display_->hasFocus()) {
        display_->setFocus();
        QApplication::sendEvent(display_, event);
        return;
    }
    
    QWidget::keyPressEvent(event);
}

} // namespace calc
