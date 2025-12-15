#include <QApplication>
#include "MainWindow.h"
#include "StyleHelper.h"

int main(int argc, char *argv[]) 
{
    QApplication app(argc, argv);
    app.setStyleSheet(StyleHelper::getStyle());
    
    MainWindow w;
    w.show();

    return app.exec();
}
