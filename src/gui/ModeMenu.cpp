#include "ModeMenu.hpp"
#include <QGraphicsOpacityEffect>

namespace calc {

ModeMenu::ModeMenu(QWidget* parent)
    : QWidget(parent), currentMode_(CalculatorMode::Standard), isVisible_(false) {
    setupUI();
    setVisible(false);
}

void ModeMenu::setupUI() {
    setObjectName("modeMenu");
    setFixedWidth(250);
    
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    
    // Заголовок
    auto* header = new QLabel("Калькулятор", this);
    header->setObjectName("menuHeader");
    header->setMinimumHeight(50);
    header->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    header->setContentsMargins(20, 0, 0, 0);
    QFont headerFont = header->font();
    headerFont.setPointSize(14);
    headerFont.setBold(true);
    header->setFont(headerFont);
    layout->addWidget(header);
    
    // Разделитель
    auto* separator = new QWidget(this);
    separator->setObjectName("menuSeparator");
    separator->setFixedHeight(1);
    layout->addWidget(separator);
    
    // Кнопки режимов
    standardButton_ = new QPushButton("Обычный", this);
    standardButton_->setObjectName("modeButton");
    standardButton_->setMinimumHeight(45);
    standardButton_->setCheckable(true);
    connect(standardButton_, &QPushButton::clicked, this, &ModeMenu::onStandardClicked);
    layout->addWidget(standardButton_);
    
    scientificButton_ = new QPushButton("Инженерный", this);
    scientificButton_->setObjectName("modeButton");
    scientificButton_->setMinimumHeight(45);
    scientificButton_->setCheckable(true);
    connect(scientificButton_, &QPushButton::clicked, this, &ModeMenu::onScientificClicked);
    layout->addWidget(scientificButton_);
    
    programmerButton_ = new QPushButton("Программистский", this);
    programmerButton_->setObjectName("modeButton");
    programmerButton_->setMinimumHeight(45);
    programmerButton_->setCheckable(true);
    connect(programmerButton_, &QPushButton::clicked, this, &ModeMenu::onProgrammerClicked);
    layout->addWidget(programmerButton_);
    
    layout->addStretch();
    
    // Анимация появления
    slideAnimation_ = new QPropertyAnimation(this, "pos");
    slideAnimation_->setDuration(250);
    slideAnimation_->setEasingCurve(QEasingCurve::OutCubic);
    
    updateSelection();
}

void ModeMenu::toggle() {
    isVisible_ = !isVisible_;
    
    if (isVisible_) {
        setVisible(true);
        slideAnimation_->setStartValue(QPoint(-width(), 0));
        slideAnimation_->setEndValue(QPoint(0, 0));
    } else {
        slideAnimation_->setStartValue(QPoint(0, 0));
        slideAnimation_->setEndValue(QPoint(-width(), 0));
        connect(slideAnimation_, &QPropertyAnimation::finished, this, [this]() {
            if (!isVisible_) {
                setVisible(false);
            }
        }, Qt::UniqueConnection);
    }
    
    slideAnimation_->start();
}

void ModeMenu::setCurrentMode(CalculatorMode mode) {
    if (currentMode_ != mode) {
        currentMode_ = mode;
        updateSelection();
        emit modeChanged(mode);
    }
}

void ModeMenu::updateSelection() {
    standardButton_->setChecked(currentMode_ == CalculatorMode::Standard);
    scientificButton_->setChecked(currentMode_ == CalculatorMode::Scientific);
    programmerButton_->setChecked(currentMode_ == CalculatorMode::Programmer);
}

void ModeMenu::onStandardClicked() {
    setCurrentMode(CalculatorMode::Standard);
    toggle(); // Закрыть меню после выбора
}

void ModeMenu::onScientificClicked() {
    setCurrentMode(CalculatorMode::Scientific);
    toggle();
}

void ModeMenu::onProgrammerClicked() {
    setCurrentMode(CalculatorMode::Programmer);
    toggle();
}

} // namespace calc
