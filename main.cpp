#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "views/MainMenuWindow.hpp"
#include "views/GameWindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("QtProject");
    QCoreApplication::setApplicationName("À prendre ou à laisser");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    MainMenuWindow *mainMenuWindow = new MainMenuWindow();
    mainMenuWindow->show();

    return app.exec();
}
