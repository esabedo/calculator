#include "MainWindow.hpp"
#include "CalculatorWidget.hpp"
#include <QMenuBar>
#include <QMessageBox>
#include <QFile>
#include <QApplication>
#include <QDebug>

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
    QString fileName = (theme == "dark") ? "styles_dark.qss" : "styles_light.qss";
    
    QFile file(fileName);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QString styleSheet = QString::fromUtf8(file.readAll());
        file.close();
        qApp->setStyleSheet(styleSheet);
    } else {
        qDebug() << "Не удалось загрузить файл стилей:" << fileName;
    }
}


} // namespace calc
