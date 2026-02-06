#include "ProgrammerModeWidget.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QApplication>
#include <QKeyEvent>
#include <QRegularExpressionValidator>

namespace calc {

ProgrammerModeWidget::ProgrammerModeWidget(QWidget* parent)
    : QWidget(parent), currentBase_(NumberBase::Decimal), currentValue_(0) {
    setupUI();
    createBaseSelector();
    createButtons();
    updateButtonStates();
}

void ProgrammerModeWidget::setupUI() {
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    
    // Дисплей основной
    display_ = new QLineEdit(this);
    display_->setReadOnly(true);
    display_->setAlignment(Qt::AlignRight);
    display_->setMinimumHeight(80);
    display_->setPlaceholderText("0");
    QFont displayFont = display_->font();
    displayFont.setPointSize(32);
    displayFont.setBold(true);
    display_->setFont(displayFont);
    display_->setReadOnly(false); // Разрешить ввод с клавиатуры
    
    // Начальный валидатор для Decimal
    QRegularExpression rx("[0-9+\\-*/%^().,\\s&|^~<>]+");
    display_->setValidator(new QRegularExpressionValidator(rx, this));
    
    connect(display_, &QLineEdit::textChanged, this, &ProgrammerModeWidget::onDisplayTextChanged);
    connect(display_, &QLineEdit::returnPressed, this, &ProgrammerModeWidget::onEqualsClicked);
    mainLayout->addWidget(display_);
    
    // Панель отображения в разных системах
    auto* baseDisplaysWidget = new QWidget(this);
    baseDisplaysWidget->setObjectName("baseDisplays");
    auto* baseDisplaysLayout = new QVBoxLayout(baseDisplaysWidget);
    baseDisplaysLayout->setSpacing(2);
    baseDisplaysLayout->setContentsMargins(5, 5, 5, 5);
    
    QFont smallFont;
    smallFont.setPointSize(13);
    smallFont.setBold(true);
    
    hexDisplay_ = new QLabel("HEX: 0", this);
    hexDisplay_->setFont(smallFont);
    hexDisplay_->setObjectName("baseDisplay");
    baseDisplaysLayout->addWidget(hexDisplay_);
    
    decDisplay_ = new QLabel("DEC: 0", this);
    decDisplay_->setFont(smallFont);
    decDisplay_->setObjectName("baseDisplay");
    baseDisplaysLayout->addWidget(decDisplay_);
    
    octDisplay_ = new QLabel("OCT: 0", this);
    octDisplay_->setFont(smallFont);
    octDisplay_->setObjectName("baseDisplay");
    baseDisplaysLayout->addWidget(octDisplay_);
    
    binDisplay_ = new QLabel("BIN: 0", this);
    binDisplay_->setFont(smallFont);
    binDisplay_->setObjectName("baseDisplay");
    baseDisplaysLayout->addWidget(binDisplay_);
    
    mainLayout->addWidget(baseDisplaysWidget);
}

void ProgrammerModeWidget::createBaseSelector() {
    auto* mainLayout = qobject_cast<QVBoxLayout*>(layout());
    if (!mainLayout) return;
    
    auto* baseSelectorWidget = new QWidget(this);
    auto* baseSelectorLayout = new QHBoxLayout(baseSelectorWidget);
    baseSelectorLayout->setSpacing(5);
    
    auto* baseGroup = new QButtonGroup(this);
    
    hexButton_ = new QPushButton("HEX", this);
    hexButton_->setCheckable(true);
    hexButton_->setObjectName("baseButton");
    hexButton_->setMinimumHeight(35);
    baseGroup->addButton(hexButton_);
    baseSelectorLayout->addWidget(hexButton_);
    
    decButton_ = new QPushButton("DEC", this);
    decButton_->setCheckable(true);
    decButton_->setChecked(true);
    decButton_->setObjectName("baseButton");
    decButton_->setMinimumHeight(35);
    baseGroup->addButton(decButton_);
    baseSelectorLayout->addWidget(decButton_);
    
    octButton_ = new QPushButton("OCT", this);
    octButton_->setCheckable(true);
    octButton_->setObjectName("baseButton");
    octButton_->setMinimumHeight(35);
    baseGroup->addButton(octButton_);
    baseSelectorLayout->addWidget(octButton_);
    
    binButton_ = new QPushButton("BIN", this);
    binButton_->setCheckable(true);
    binButton_->setObjectName("baseButton");
    binButton_->setMinimumHeight(35);
    baseGroup->addButton(binButton_);
    baseSelectorLayout->addWidget(binButton_);
    
    mainLayout->addWidget(baseSelectorWidget);
    
    connect(hexButton_, &QPushButton::clicked, [this]() { onBaseChanged(NumberBase::Hexadecimal); });
    connect(decButton_, &QPushButton::clicked, [this]() { onBaseChanged(NumberBase::Decimal); });
    connect(octButton_, &QPushButton::clicked, [this]() { onBaseChanged(NumberBase::Octal); });
    connect(binButton_, &QPushButton::clicked, [this]() { onBaseChanged(NumberBase::Binary); });
    
    // Сетка кнопок
    auto* buttonGrid = new QWidget(this);
    auto* gridLayout = new QGridLayout(buttonGrid);
    gridLayout->setSpacing(4);
    gridLayout->setObjectName("buttonGrid");
    mainLayout->addWidget(buttonGrid);
}

void ProgrammerModeWidget::createButtons() {
    auto* gridLayout = findChild<QGridLayout*>("buttonGrid");
    if (!gridLayout) return;
    
    // Строка 0: Битовые операции
    auto* andBtn = new CalculatorButton("AND", CalculatorButton::ButtonType::Operator, this);
    andBtn->setValue(" AND ");
    auto* orBtn = new CalculatorButton("OR", CalculatorButton::ButtonType::Operator, this);
    orBtn->setValue(" OR ");
    auto* xorBtn = new CalculatorButton("XOR", CalculatorButton::ButtonType::Operator, this);
    xorBtn->setValue(" XOR ");
    auto* notBtn = new CalculatorButton("NOT", CalculatorButton::ButtonType::Operator, this);
    notBtn->setValue("NOT ");
    
    gridLayout->addWidget(andBtn, 0, 0);
    gridLayout->addWidget(orBtn, 0, 1);
    gridLayout->addWidget(xorBtn, 0, 2);
    gridLayout->addWidget(notBtn, 0, 3);
    
    connect(andBtn, &QPushButton::clicked, this, &ProgrammerModeWidget::onButtonClicked);
    connect(orBtn, &QPushButton::clicked, this, &ProgrammerModeWidget::onButtonClicked);
    connect(xorBtn, &QPushButton::clicked, this, &ProgrammerModeWidget::onButtonClicked);
    connect(notBtn, &QPushButton::clicked, this, &ProgrammerModeWidget::onButtonClicked);
    
    // Строка 1: Сдвиги и операции
    auto* lshiftBtn = new CalculatorButton("<<", CalculatorButton::ButtonType::Operator, this);
    lshiftBtn->setValue(" << ");
    auto* rshiftBtn = new CalculatorButton(">>", CalculatorButton::ButtonType::Operator, this);
    rshiftBtn->setValue(" >> ");
    auto* ceBtn = new CalculatorButton("CE", CalculatorButton::ButtonType::Special, this);
    auto* clearBtn = new CalculatorButton("C", CalculatorButton::ButtonType::Special, this);
    
    gridLayout->addWidget(lshiftBtn, 1, 0);
    gridLayout->addWidget(rshiftBtn, 1, 1);
    gridLayout->addWidget(ceBtn, 1, 2);
    gridLayout->addWidget(clearBtn, 1, 3);
    
    connect(lshiftBtn, &QPushButton::clicked, this, &ProgrammerModeWidget::onButtonClicked);
    connect(rshiftBtn, &QPushButton::clicked, this, &ProgrammerModeWidget::onButtonClicked);
    connect(ceBtn, &QPushButton::clicked, this, &ProgrammerModeWidget::onClearClicked);
    connect(clearBtn, &QPushButton::clicked, this, &ProgrammerModeWidget::onClearClicked);
    
    // Строка 2: A, B, C, D (для HEX)
    for (int i = 0; i < 4; ++i) {
        auto* btn = new CalculatorButton(QString(QChar('A' + i)), CalculatorButton::ButtonType::Digit, this);
        gridLayout->addWidget(btn, 2, i);
        digitButtons_.append(btn);
        connect(btn, &QPushButton::clicked, this, &ProgrammerModeWidget::onButtonClicked);
    }
    
    // Строка 3: E, F, ⌫, /
    auto* eBtn = new CalculatorButton("E", CalculatorButton::ButtonType::Digit, this);
    auto* fBtn = new CalculatorButton("F", CalculatorButton::ButtonType::Digit, this);
    auto* backBtn = new CalculatorButton("⌫", CalculatorButton::ButtonType::Special, this);
    auto* divBtn = new CalculatorButton("/", CalculatorButton::ButtonType::Operator, this);
    
    gridLayout->addWidget(eBtn, 3, 0);
    gridLayout->addWidget(fBtn, 3, 1);
    gridLayout->addWidget(backBtn, 3, 2);
    gridLayout->addWidget(divBtn, 3, 3);
    
    digitButtons_.append(eBtn);
    digitButtons_.append(fBtn);
    connect(eBtn, &QPushButton::clicked, this, &ProgrammerModeWidget::onButtonClicked);
    connect(fBtn, &QPushButton::clicked, this, &ProgrammerModeWidget::onButtonClicked);
    connect(backBtn, &QPushButton::clicked, this, &ProgrammerModeWidget::onBackspaceClicked);
    connect(divBtn, &QPushButton::clicked, this, &ProgrammerModeWidget::onButtonClicked);
    
    // Строка 4-6: Цифры 7-9, 4-6, 1-3
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            int digit = 7 - row * 3 + col;
            auto* btn = new CalculatorButton(QString::number(digit), CalculatorButton::ButtonType::Digit, this);
            gridLayout->addWidget(btn, 4 + row, col);
            digitButtons_.append(btn);
            connect(btn, &QPushButton::clicked, this, &ProgrammerModeWidget::onButtonClicked);
        }
        
        // Операторы справа
        QString ops[] = {"*", "-", "+"};
        auto* opBtn = new CalculatorButton(ops[row], CalculatorButton::ButtonType::Operator, this);
        gridLayout->addWidget(opBtn, 4 + row, 3);
        connect(opBtn, &QPushButton::clicked, this, &ProgrammerModeWidget::onButtonClicked);
    }
    
    // Строка 7: 0, %, =
    auto* zeroBtn = new CalculatorButton("0", CalculatorButton::ButtonType::Digit, this);
    auto* modBtn = new CalculatorButton("%", CalculatorButton::ButtonType::Operator, this);
    auto* equalBtn = new CalculatorButton("=", CalculatorButton::ButtonType::Special, this);
    
    gridLayout->addWidget(zeroBtn, 7, 0, 1, 2);
    gridLayout->addWidget(modBtn, 7, 2);
    gridLayout->addWidget(equalBtn, 7, 3);
    
    digitButtons_.append(zeroBtn);
    connect(zeroBtn, &QPushButton::clicked, this, &ProgrammerModeWidget::onButtonClicked);
    connect(modBtn, &QPushButton::clicked, this, &ProgrammerModeWidget::onButtonClicked);
    connect(equalBtn, &QPushButton::clicked, this, &ProgrammerModeWidget::onEqualsClicked);
}

void ProgrammerModeWidget::onButtonClicked() {
    auto* btn = qobject_cast<CalculatorButton*>(sender());
    if (!btn) return;
    
    QString value = btn->value();
    display_->setText(display_->text() + value);
    display_->setFocus();
}

void ProgrammerModeWidget::onClearClicked() {
    display_->clear();
    display_->setPlaceholderText("0");
    currentValue_ = 0;
    updateBaseDisplays();
}

void ProgrammerModeWidget::onBackspaceClicked() {
    QString text = display_->text();
    if (!text.isEmpty()) {
        text.chop(1);
        display_->setText(text);
    }
}

void ProgrammerModeWidget::onEqualsClicked() {
    emit evaluateRequested();
}

void ProgrammerModeWidget::onBaseChanged(NumberBase base) {
    currentBase_ = base;
    updateButtonStates();
    
    // Обновить валидатор
    QString pattern;
    switch (base) {
        case NumberBase::Binary: pattern = "[0-1+\\-*/%^().,\\s&|^~<>]+"; break;
        case NumberBase::Octal: pattern = "[0-7+\\-*/%^().,\\s&|^~<>]+"; break;
        case NumberBase::Decimal: pattern = "[0-9+\\-*/%^().,\\s&|^~<>]+"; break;
        case NumberBase::Hexadecimal: pattern = "[0-9A-Fa-f+\\-*/%^().,\\s&|^~<>]+"; break;
    }
    QRegularExpression rx(pattern);
    display_->setValidator(new QRegularExpressionValidator(rx, this));
    
    // Конвертировать текущее значение в новую систему
    if (!display_->text().isEmpty()) {
        try {
            currentValue_ = NumberConverter::fromString(display_->text(), currentBase_);
            display_->setText(NumberConverter::toString(currentValue_, currentBase_));
            updateBaseDisplays();
        } catch (...) {
            // Игнорировать ошибки конвертации
        }
    }
}

void ProgrammerModeWidget::updateBaseDisplays() {
    hexDisplay_->setText("HEX: " + NumberConverter::toString(currentValue_, NumberBase::Hexadecimal));
    decDisplay_->setText("DEC: " + NumberConverter::toString(currentValue_, NumberBase::Decimal));
    octDisplay_->setText("OCT: " + NumberConverter::toString(currentValue_, NumberBase::Octal));
    binDisplay_->setText("BIN: " + NumberConverter::toString(currentValue_, NumberBase::Binary));
}

void ProgrammerModeWidget::updateButtonStates() {
    int maxDigit = NumberConverter::getMaxDigit(currentBase_);
    
    for (auto* btn : digitButtons_) {
        QString text = btn->text();
        bool enabled = true;
        
        if (text.length() == 1) {
            QChar c = text[0];
            if (c.isDigit()) {
                enabled = c.digitValue() < static_cast<int>(currentBase_);
            } else if (c >= 'A' && c <= 'F') {
                enabled = (currentBase_ == NumberBase::Hexadecimal);
            }
        }
        
        btn->setEnabled(enabled);
    }
}

void ProgrammerModeWidget::onDisplayTextChanged(const QString& text) {
    if (text.isEmpty()) {
        currentValue_ = 0;
        updateBaseDisplays();
        return;
    }
    
    // Пытаемся распарсить текст как число в текущей системе
    // Если это выражение (например "5+5"), парсинг не удастся, 
    // и мы просто не обновляем панели до вычисления
    try {
        currentValue_ = NumberConverter::fromString(text, currentBase_);
        updateBaseDisplays();
    } catch (...) {
        // Игнорируем ошибки (это может быть незаконченное выражение)
    }
}

void ProgrammerModeWidget::keyPressEvent(QKeyEvent* event) {
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
