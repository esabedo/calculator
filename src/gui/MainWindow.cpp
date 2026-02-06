#include "MainWindow.hpp"
#include "CalculatorWidget.hpp"
#include <QMenuBar>
#include <QMessageBox>
#include <QFile>
#include <QApplication>

namespace calc {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), isDarkTheme_(true) {
    setupUI();
    createMenus();
    loadStyleSheet("dark");
}

void MainWindow::setupUI() {
    setWindowTitle("Инженерный калькулятор");
    setMinimumSize(800, 700);
    
    calculator_ = new CalculatorWidget(this);
    setCentralWidget(calculator_);
}

void MainWindow::createMenus() {
    // Меню "Файл"
    QMenu* fileMenu = menuBar()->addMenu("&Файл");
    
    QAction* exitAction = fileMenu->addAction("&Выход");
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);
    
    // Меню "Вид"
    QMenu* viewMenu = menuBar()->addMenu("&Вид");
    
    toggleThemeAction_ = viewMenu->addAction("Переключить тему");
    toggleThemeAction_->setShortcut(QKeySequence("Ctrl+T"));
    connect(toggleThemeAction_, &QAction::triggered, this, &MainWindow::onToggleTheme);
    
    // Меню "Помощь"
    QMenu* helpMenu = menuBar()->addMenu("&Помощь");
    
    QAction* aboutAction = helpMenu->addAction("&О программе");
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAbout);
}

void MainWindow::onAbout() {
    QMessageBox::about(this, "О программе",
        "<h2>Инженерный калькулятор</h2>"
        "<p>Версия 2.0</p>"
        "<p>Полнофункциональный инженерный калькулятор с поддержкой:</p>"
        "<ul>"
        "<li>Базовых арифметических операций (+, -, *, /, %, ^)</li>"
        "<li>Тригонометрических функций (sin, cos, tan, asin, acos, atan)</li>"
        "<li>Гиперболических функций (sinh, cosh, tanh)</li>"
        "<li>Логарифмических функций (log, ln, log10)</li>"
        "<li>Математических констант (π, e)</li>"
        "<li>Дополнительных функций (abs, sqrt, exp, ceil, floor, round, factorial)</li>"
        "</ul>"
        "<p>Создано с использованием C++17 и Qt5</p>"
    );
}

void MainWindow::onToggleTheme() {
    isDarkTheme_ = !isDarkTheme_;
    loadStyleSheet(isDarkTheme_ ? "dark" : "light");
}

void MainWindow::loadStyleSheet(const QString& theme) {
    QString styleSheet;
    
    if (theme == "dark") {
        // Темная тема Discord
        QFile file("styles.qss");
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            styleSheet = QString::fromUtf8(file.readAll());
            file.close();
        }
    } else {
        // Светлая тема
        styleSheet = R"(
            QWidget {
                background-color: #ffffff;
                color: #2e3338;
                font-family: "Segoe UI", Arial, sans-serif;
                font-size: 11pt;
            }
            QMainWindow {
                background-color: #f2f3f5;
            }
            QMenuBar {
                background-color: #ffffff;
                color: #2e3338;
                border-bottom: 1px solid #e3e5e8;
            }
            QMenuBar::item:selected {
                background-color: #e3e5e8;
            }
            QMenu {
                background-color: #ffffff;
                color: #2e3338;
                border: 1px solid #e3e5e8;
            }
            QMenu::item:selected {
                background-color: #e3e5e8;
            }
            QLineEdit {
                background-color: #f2f3f5;
                color: #2e3338;
                border: 2px solid: #e3e5e8;
                border-radius: 5px;
                padding: 10px;
                font-size: 18pt;
                font-weight: bold;
            }
            QLineEdit:focus {
                border: 2px solid #5865f2;
            }
            QPushButton:!property(buttonType) {
                background-color: #5865f2;
                color: #ffffff;
                border: 1px solid #4752c4;
                border-radius: 5px;
                padding: 8px;
                font-size: 11pt;
                font-weight: bold;
            }
            QPushButton:!property(buttonType):hover {
                background-color: #4752c4;
            }
            QGroupBox {
                background-color: #ffffff;
                border: 2px solid #e3e5e8;
                border-radius: 5px;
                margin-top: 10px;
                padding-top: 10px;
                font-weight: bold;
            }
            QListWidget {
                background-color: #f2f3f5;
                color: #2e3338;
                border: 1px solid #e3e5e8;
                border-radius: 3px;
                padding: 5px;
            }
            QListWidget::item:hover {
                background-color: #e3e5e8;
            }
            QPushButton {
                background-color: #e3e5e8;
                color: #2e3338;
                border: 1px solid #d1d4d8;
                border-radius: 5px;
                padding: 10px;
                font-size: 12pt;
                font-weight: bold;
                min-width: 60px;
                min-height: 50px;
            }
            QPushButton:hover {
                background-color: #d1d4d8;
            }
            QPushButton[buttonType="digit"] {
                background-color: #e3e5e8;
                color: #2e3338;
            }
            QPushButton[buttonType="operator"] {
                background-color: #5865f2;
                color: #ffffff;
            }
            QPushButton[buttonType="operator"]:hover {
                background-color: #4752c4;
            }
            QPushButton[buttonType="function"] {
                background-color: #3ba55d;
                color: #ffffff;
            }
            QPushButton[buttonType="function"]:hover {
                background-color: #2d7d46;
            }
            QPushButton[buttonType="constant"] {
                background-color: #9b59b6;
                color: #ffffff;
            }
            QPushButton[buttonType="constant"]:hover {
                background-color: #8e44ad;
            }
            QPushButton[buttonType="special"] {
                background-color: #ed4245;
                color: #ffffff;
            }
            QPushButton[buttonType="special"]:hover {
                background-color: #c03537;
            }
            QPushButton[buttonType="bracket"] {
                background-color: #faa61a;
                color: #ffffff;
            }
            QPushButton[buttonType="bracket"]:hover {
                background-color: #df8f17;
            }
        )";
    }
    
    qApp->setStyleSheet(styleSheet);
}

} // namespace calc
