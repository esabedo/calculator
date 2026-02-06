#include "gui/MainWindow.hpp"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    app.setApplicationName("Инженерный калькулятор");
    app.setApplicationVersion("2.0");
    app.setOrganizationName("Calculator");
    
    calc::MainWindow window;
    window.show();
    
    return app.exec();
}
