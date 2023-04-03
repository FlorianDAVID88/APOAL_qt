#include <QApplication>
#include <QPushButton>
#include <QLineEdit>
#include <QInputDialog>
#include "MainMenuWindow.hpp"
#include "GameWindow.hpp"
#include <QStyle>
#include <QDesktopWidget>

MainMenuWindow::MainMenuWindow(QWidget *parent) : QWidget(parent) {
    const int WIDTH = 900;
    const int HEIGHT = 400;

    // Set window position
    setFixedSize(WIDTH, HEIGHT);
    setWindowTitle("Menu principal - À prendre ou à laisser");
    setGeometry(
            QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    size(),
                    qApp->desktop()->availableGeometry()
            )
    );

    // Add play button
    QPushButton *playButton = new QPushButton("Jouer", this);
    playButton->setGeometry(WIDTH / 2 - 100, HEIGHT / 2 - 25, 200, 50);
    playButton->setStyleSheet("QPushButton { background-color: #00ff00; color: #000000; font-weight: bold; }");
    connect(playButton, &QPushButton::clicked, this, &MainMenuWindow::inputName);

    // Add quit button
    QPushButton *quitButton = new QPushButton("Quitter", this);
    quitButton->setGeometry(WIDTH / 2 - 100, HEIGHT / 2 + 25, 200, 50);
    quitButton->setStyleSheet("QPushButton { background-color: #ff0000; color: #ffffff; font-weight: bold; }");
    connect(quitButton, &QPushButton::clicked, this, &MainMenuWindow::close);

    // Add logo
    QLabel *logo = new QLabel(this);
    logo->setGeometry(WIDTH / 2 - 125, 10, 250, 120);
    QPixmap pixmapLogo(QCoreApplication::applicationDirPath() + "/assets/logo.png");
    logo->setPixmap(pixmapLogo.scaled(250, 120, Qt::KeepAspectRatio));
    logo->setStyleSheet("QLabel { background-color : transparent; }");

    // Set background color
    setStyleSheet("background-color: #000000;");

}

void MainMenuWindow::inputName() {
    QInputDialog *input = new QInputDialog();
    input->setWindowTitle("Nom du joueur");
    input->setLabelText("Nom : ");

    if (input->exec() == QDialog::Accepted) {
        QString name = input->textValue();
        MainMenuWindow::play(name);
    }
}

void MainMenuWindow::play(QString playerName) {
    //Box choice
    // MainMenuWindow::boxChoice(playerName);

    // Create a new game
    GameWindow *gameWindow = new GameWindow();
    gameWindow->show();

    // Hide this window
    this->hide();
}

void MainMenuWindow::boxChoice(QString playerName) {
    QInputDialog *input = new QInputDialog();
    input->setWindowTitle("Choix de la boîte");
    input->setLabelText("Choisissez la boîte avec laquelle vous voulez jouer, " + playerName + " :");
    input->exec();

    bool ok;
    int value = input->textValue().toInt(&ok);

    while (!ok || QString::number(value) != input->textValue() || value > 24 || value < 1) {
        input->exec();
        value = input->textValue().toInt(&ok);
    }
}