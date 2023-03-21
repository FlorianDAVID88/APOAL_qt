#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "views/MainMenuWindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("QtProject");
    QCoreApplication::setApplicationName("À prendre ou à laisser");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    MainMenuWindow mainMenuWindow;
    mainMenuWindow.show();


    return app.exec();
}
