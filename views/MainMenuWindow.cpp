#include <QPushButton>
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
    connect(playButton, &QPushButton::clicked, this, &MainMenuWindow::play);
}

void MainMenuWindow::play() {
    // Hide the main menu
    hide();

    // Create a new game
    GameWindow *gameWindow = new GameWindow();
    gameWindow->show();

}