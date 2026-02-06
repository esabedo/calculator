#pragma once

#include <QMainWindow>
#include <QAction>
#include <QMenu>

namespace calc {

class CalculatorWidget;

/**
 * @brief Главное окно приложения калькулятора
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onAbout();
    void onToggleTheme();

private:
    void setupUI();
    void createMenus();
    void loadStyleSheet(const QString& theme);
    
    CalculatorWidget* calculator_;
    QAction* toggleThemeAction_;
    bool isDarkTheme_;
};

} // namespace calc
