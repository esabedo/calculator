#include "CalculatorWidget.hpp"
#include "../lexer.hpp"
#include "../parser.hpp"
#include "../evaluator.hpp"
#include "../error.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <sstream>
#include <iomanip>

namespace calc {

CalculatorWidget::CalculatorWidget(QWidget* parent)
    : QWidget(parent), currentMode_(CalculatorMode::Standard) {
    setupUI();
}

void CalculatorWidget::setupUI() {
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // Верхняя панель с кнопкой меню и заголовком
    auto* headerWidget = new QWidget(this);
    headerWidget->setObjectName("calculatorHeader");
    headerWidget->setMinimumHeight(50);
    auto* headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(10, 0, 10, 0);
    
    // Кнопка hamburger menu
    menuButton_ = new QPushButton("☰", this);
    menuButton_->setObjectName("menuButton");
    menuButton_->setFixedSize(40, 40);
    QFont menuFont = menuButton_->font();
    menuFont.setPointSize(18);
    menuButton_->setFont(menuFont);
    connect(menuButton_, &QPushButton::clicked, this, &CalculatorWidget::onMenuButtonClicked);
    headerLayout->addWidget(menuButton_);
    
    // Заголовок режима
    modeTitle_ = new QLabel(getModeTitle(currentMode_), this);
    modeTitle_->setObjectName("modeTitle");
    QFont titleFont = modeTitle_->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    modeTitle_->setFont(titleFont);
    headerLayout->addWidget(modeTitle_);
    
    headerLayout->addStretch();
    
    mainLayout->addWidget(headerWidget);
    
    // Контейнер для меню и контента
    auto* contentContainer = new QWidget(this);
    auto* contentLayout = new QHBoxLayout(contentContainer);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);
    
    // Всплывающее меню
    modeMenu_ = new ModeMenu(this);
    connect(modeMenu_, &ModeMenu::modeChanged, this, &CalculatorWidget::onModeChanged);
    contentLayout->addWidget(modeMenu_);
    
    // Стек виджетов режимов
    modeStack_ = new QStackedWidget(this);
    
    standardWidget_ = new StandardModeWidget(this);
    connect(standardWidget_, &StandardModeWidget::evaluateRequested, 
            this, &CalculatorWidget::onEvaluateRequested);
    modeStack_->addWidget(standardWidget_);
    
    scientificWidget_ = new ScientificModeWidget(this);
    connect(scientificWidget_, &ScientificModeWidget::evaluateRequested, 
            this, &CalculatorWidget::onEvaluateRequested);
    modeStack_->addWidget(scientificWidget_);
    
    programmerWidget_ = new ProgrammerModeWidget(this);
    connect(programmerWidget_, &ProgrammerModeWidget::evaluateRequested, 
            this, &CalculatorWidget::onEvaluateRequested);
    modeStack_->addWidget(programmerWidget_);
    
    contentLayout->addWidget(modeStack_);
    
    mainLayout->addWidget(contentContainer);
    
    // Установить начальный режим
    modeStack_->setCurrentWidget(standardWidget_);
}

void CalculatorWidget::onMenuButtonClicked() {
    modeMenu_->toggle();
}

void CalculatorWidget::onModeChanged(CalculatorMode mode) {
    currentMode_ = mode;
    modeTitle_->setText(getModeTitle(mode));
    
    switch (mode) {
        case CalculatorMode::Standard:
            modeStack_->setCurrentWidget(standardWidget_);
            break;
        case CalculatorMode::Scientific:
            modeStack_->setCurrentWidget(scientificWidget_);
            break;
        case CalculatorMode::Programmer:
            modeStack_->setCurrentWidget(programmerWidget_);
            break;
    }
}

void CalculatorWidget::onEvaluateRequested() {
    QString expression;
    
    // Получить выражение из текущего режима
    switch (currentMode_) {
        case CalculatorMode::Standard:
            expression = standardWidget_->display()->text();
            break;
        case CalculatorMode::Scientific:
            expression = scientificWidget_->display()->text();
            break;
        case CalculatorMode::Programmer:
            expression = programmerWidget_->display()->text();
            break;
    }
    
    if (expression.isEmpty()) return;
    
    evaluateExpression(expression);
}

void CalculatorWidget::evaluateExpression(const QString& expression) {
    QLineEdit* currentDisplay = nullptr;
    
    switch (currentMode_) {
        case CalculatorMode::Standard:
            currentDisplay = standardWidget_->display();
            break;
        case CalculatorMode::Scientific:
            currentDisplay = scientificWidget_->display();
            break;
        case CalculatorMode::Programmer:
            currentDisplay = programmerWidget_->display();
            break;
    }
    
    if (!currentDisplay) return;
    
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
        
        if (currentMode_ == CalculatorMode::Programmer) {
            // Для программистского режима - целое число
            int64_t intResult = static_cast<int64_t>(result);
            oss << intResult;
        } else {
            // Для остальных режимов - с плавающей точкой
            oss << std::setprecision(10) << result;
            std::string resultStr = oss.str();
            
            // Убрать лишние нули после точки
            if (resultStr.find('.') != std::string::npos) {
                resultStr.erase(resultStr.find_last_not_of('0') + 1, std::string::npos);
                if (resultStr.back() == '.') {
                    resultStr.pop_back();
                }
            }
            
            currentDisplay->setText(QString::fromStdString(resultStr));
            return;
        }
        
        currentDisplay->setText(QString::fromStdString(oss.str()));
        
    } catch (const ParseError& e) {
        currentDisplay->setText(QString("Ошибка: %1").arg(e.what()));
    } catch (const EvalError& e) {
        currentDisplay->setText(QString("Ошибка: %1").arg(e.what()));
    } catch (const std::exception& e) {
        currentDisplay->setText(QString("Ошибка: %1").arg(e.what()));
    }
}

QString CalculatorWidget::getModeTitle(CalculatorMode mode) const {
    switch (mode) {
        case CalculatorMode::Standard:
            return "Обычный";
        case CalculatorMode::Scientific:
            return "Инженерный";
        case CalculatorMode::Programmer:
            return "Программистский";
        default:
            return "Калькулятор";
    }
}

} // namespace calc
