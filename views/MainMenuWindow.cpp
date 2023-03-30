#include <QPushButton>
#include <QLineEdit>
#include <QInputDialog>
#include "MainMenuWindow.hpp"
#include "GameWindow.hpp"

MainMenuWindow::MainMenuWindow(QWidget *parent) : QWidget(parent) {
    const int WIDTH = 900;
    const int HEIGHT = 400;

    setFixedSize(WIDTH, HEIGHT);
    setWindowTitle("Menu principal - À prendre ou à laisser");

    // Add play button
    QPushButton *playButton = new QPushButton("Jouer", this);
    playButton->setGeometry(WIDTH / 2 - 100, HEIGHT / 2 - 25, 200, 50);
    connect(playButton, &QPushButton::clicked, this, &MainMenuWindow::inputName);
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
    MainMenuWindow::boxChoice(playerName);

    // Hide the main menu
    hide();

    // Create a new game
    GameWindow *gameWindow = new GameWindow();
    gameWindow->show();
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
