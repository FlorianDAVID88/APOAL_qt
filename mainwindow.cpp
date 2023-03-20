#include "mainwindow.hpp"
#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    // Set size of the window
    setFixedSize(900, 400);

    int LINE = 4;
    int COLUMN = 6;
    int BTN_WIDTH = 90;
    int BTN_HEIGHT = 50;
    int BTN_MARGIN = 15;
    int BOXES = LINE * COLUMN;

    const QString EXISTING_PRICES[] = {
            "UNE PLUME",
            "0,05 €",
            "0,10 €",
            "0,50 €",
            "1 €",
            "5 €",
            "10 €",
            "25 €",
            "50 €",
            "100 €",
            "250 €",
            "500 €",
            "1 000 €",
            "1 500 €",
            "3 000 €",
            "4 000 €",
            "5 000 €",
            "10 000 €",
            "15 000 €",
            "20 000 €",
            "30 000 €",
            "50 000 €",
            "100 000 €",
            "250 000 €",
    };

    // Create prices
    for (int i = 0; i < BOXES; ++i) {
        prices.push_back(EXISTING_PRICES[i]);
    }
    // Shuffle the prices
    std::random_shuffle(prices.begin(), prices.end());

    // Show prices
    for (int i = 0; i < BOXES; ++i) {
        QLabel *label = new QLabel(this);
        label->setText(EXISTING_PRICES[i]);
        label->setAlignment(Qt::AlignCenter);
        if(i < BOXES / 2) {
            label->setStyleSheet("QLabel { background-color : red; color : white; }");
            label->setGeometry(10, 10 + (i * (20 + 2)), 90, 20);
        } else {
            label->setStyleSheet("QLabel { background-color : gold; color : black; }");
            label->setGeometry(110, 10 + ((i - BOXES / 2) * (20 + 2)), 90, 20);
        }
        labels.push_back(label);
    }


    // Create the buttons
    for (int i = 0; i < LINE; ++i) {
        for (int j = 0; j < COLUMN; ++j) {
            int boxId = i + (j * 4) + 1;
            QPushButton *m_button = new QPushButton(QString::number(boxId), this);
            m_button->setGeometry(200 + ((BTN_WIDTH + BTN_MARGIN) * i), 10 + ((BTN_HEIGHT + BTN_MARGIN) * j), BTN_WIDTH,
                                  BTN_HEIGHT);

            // Add the button to the list
            boxes.push_back(m_button);

            // Connect the button to the openBox function
            connect(m_button, &QPushButton::clicked, [=] { openBox(m_button); });
        }
    }
}

void MainWindow::openBox(QPushButton *button) {
    // Open the box = hide the button
    button->hide();
    // Get price
    QString price = prices[button->text().toInt() - 1];

    // Hide the label price
    for (int i = 0; i < labels.size(); ++i) {
        if(labels[i]->text() == price) {
            labels[i]->hide();
        }
    }

    // Show dialog box in the middle of the screen
    QMessageBox msgBox;
    msgBox.setText("Vous avez ouvert une boite. Elle contient " + price);
    msgBox.exec();
}

void MainWindow::createView() {

}