#include "GameWindow.hpp"
#include "MainMenuWindow.hpp"

#include <unistd.h>
#include <random>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>

#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QPicture>
#include <QPropertyAnimation>
#include <QStyle>
#include <QDesktopWidget>

QString boxStyle = "QPushButton { background-color: #ffba20; color: #000000; font-weight: bold; }";

GameWindow::GameWindow(QWidget *parent) : QWidget(parent) {
    // Set size of the window
    setFixedSize(900, 400);

    // Set window position
    setGeometry(
            QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    size(),
                    qApp->desktop()->availableGeometry()
            )
    );


    // Set background color
    setStyleSheet("background-color: #000000;");

    // Get current path
    QString path = QCoreApplication::applicationDirPath();

    // Add logo
    QLabel *logo = new QLabel(this);
    logo->setGeometry(750, 0, 150, 80);
    QPixmap pixmapLogo(path + "/assets/logo.png");
    logo->setPixmap(pixmapLogo.scaled(150, 80, Qt::KeepAspectRatio));

    // Add Arthur
    arthur = new QLabel(this);
    arthur->setGeometry(600, 0, 500, 500);
    QPixmap pixmapArthur(path + "/assets/arthur.png");
    arthur->setPixmap(pixmapArthur.scaled(500, 500, Qt::KeepAspectRatio));
    arthur->setStyleSheet("QLabel { background-color : transparent; }");
    arthurPos = arthur->pos();

    int LINE = 4;
    int COLUMN = 6;
    int BOXES = LINE * COLUMN;

    const QString EXISTING_PRICES[] = {
            "PLUME",
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
            "2 000 €",
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
    // Randomize prices
    std::shuffle(std::begin(prices), std::end(prices), std::mt19937(std::random_device()()));

    // Show prices
    const int PRICE_WIDTH = 110;
    const int PRICE_HEIGHT = 30;
    const int PRICE_MARGIN = 3;
    this->boxesRemaining = BOXES;
    for (int i = 0; i < BOXES; ++i) {
        QLabel *label = new QLabel(this);
        label->setText(EXISTING_PRICES[i]);
        label->setAlignment(Qt::AlignCenter);
        if (i < BOXES / 2) {
            label->setStyleSheet("QLabel { background-color : red; color : white; font-weight: bold; }");
            label->setGeometry(PRICE_MARGIN, PRICE_MARGIN + (i * (PRICE_HEIGHT + PRICE_MARGIN)), PRICE_WIDTH,
                               PRICE_HEIGHT);
        } else {
            label->setStyleSheet("QLabel { background-color : gold; color : black; font-weight: bold; }");
            label->setGeometry(PRICE_WIDTH + PRICE_MARGIN * 2,
                               PRICE_MARGIN + ((i - BOXES / 2) * (PRICE_HEIGHT + PRICE_MARGIN)), PRICE_WIDTH,
                               PRICE_HEIGHT);
        }
        labels.push_back(label);
    }

    // Create the buttons
    const int BTN_WIDTH = 105;
    const int BTN_HEIGHT = 59;
    const int BTN_MARGIN = 5;
    for (int i = 0; i < LINE; ++i) {
        for (int j = 0; j < COLUMN; ++j) {
            int boxId = i + (j * 4) + 1;
            QPushButton *m_button = new QPushButton(QString::number(boxId), this);
            m_button->setGeometry(250 + ((BTN_WIDTH + BTN_MARGIN) * i), 10 + ((BTN_HEIGHT + BTN_MARGIN) * j), BTN_WIDTH,
                                  BTN_HEIGHT);

            // Set the style of the button
            m_button->setStyleSheet(boxStyle);

            // Add the button to the list
            boxes.push_back(m_button);

            // Connect the button to the openBox function
            connect(m_button, &QPushButton::clicked, [=] { openBox(m_button); });
        }
    }

    // Add "Vous avez gagné" label
    winLabel = new QLabel(this);
    winLabel->setText("Vous avez remporté :");
    winLabel->setAlignment(Qt::AlignCenter);
    winLabel->setStyleSheet(
            "QLabel { background-color: transparent; color: #ffffff; font-weight: bold; font-size: 32px; }");
    winLabel->setGeometry(0, 75, 900, 100);
    winLabel->hide();

    show();

    // Ask for select a box
    askForSelectBox();
}

void GameWindow::openBox(QPushButton *button) {
    // Check if not in selection mode
    if (waitingForBoxSelection) {
        selectBox(button);
        return;
    }

    // Open the box = hide the button
    button->hide();
    // Get price
    QString price = prices[button->text().toInt() - 1];

    // Hide the label price
    for (int i = 0; i < labels.size(); ++i) {
        if (labels[i]->text() == price) {
            labels[i]->hide();
        }
    }

    // Get current path
    QString path = QCoreApplication::applicationDirPath();

    // Shake Arthur label
    QLabel *arthur = this->arthur;

    // Create animation
    QPropertyAnimation *animation = new QPropertyAnimation(arthur, "pos");
    animation->setDuration(3000);
    animation->setStartValue(arthurPos + QPoint(-900 - 500, 0));
    animation->setEndValue(arthurPos);
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    this->boxesRemaining--; // Decrement the number of boxes remaining

    // Check if the game finished
    if (this->boxesRemaining <= 1) {
        endGame();
        return;
    }

    // Show dialog box in the middle of the screen
    QMessageBox msgBox;
    if (isdigit(price[0].toLatin1())) {
        msgBox.setText("Vous avez ouvert une boite. Elle contient " + price);
    } else {
        msgBox.setText("Vous avez ouvert une boite. Elle contient une plume");
    }
    msgBox.exec();


    // Bank
    if (boxesRemaining == 18 || boxesRemaining == 13 || boxesRemaining == 9 || boxesRemaining == 6 ||
        boxesRemaining == 4 || boxesRemaining == 3 || boxesRemaining == 2) {
        bankCalling();
    }

}
/**
 * 25th box choice
 */
void GameWindow::box25() {
    QLabel *zero = new QLabel(this);
    zero->setText("0 €");
    zero->setAlignment(Qt::AlignCenter);
    zero->setStyleSheet("QLabel { background-color: #1f1f1f; color: white; font-weight: bold; font-size: 32px; }");
    zero->setGeometry(40, 40, 160, 40);
    zero->show();

    QLabel *divide = new QLabel(this);
    divide->setText("÷2");
    divide->setAlignment(Qt::AlignCenter);
    divide->setStyleSheet("QLabel { background-color: red; color: white; font-weight: bold; font-size: 32px; }");
    divide->setGeometry(215, 40, 160, 40);
    divide->show();

    QLabel *more1000 = new QLabel(this);
    more1000->setText("+ 1 000 €");
    more1000->setAlignment(Qt::AlignCenter);
    more1000->setStyleSheet("QLabel { background-color: yellow; color: black; font-weight: bold; font-size: 32px; }");
    more1000->setGeometry(390, 40, 160, 40);
    more1000->show();

    QLabel *multiply = new QLabel(this);
    multiply->setText("x2");
    multiply->setAlignment(Qt::AlignCenter);
    multiply->setStyleSheet("QLabel { background-color: gold; color: black; font-weight: bold; font-size: 32px; }");
    multiply->setGeometry(565, 40, 160, 40);
    multiply->show();

    QPushButton *buttonAccept = new QPushButton("OUVRIR", this);
    buttonAccept->setGeometry(250, 350, 100, 20);
    buttonAccept->setStyleSheet("QPushButton { background-color: gold; color: #000000; font-weight: bold; }");
    buttonAccept->show();

    QPushButton *buttonRefuse = new QPushButton("NE PAS OUVRIR", this);
    buttonRefuse->setGeometry(370, 350, 100, 20);
    buttonRefuse->setStyleSheet("QPushButton { background-color: red; color: white; font-weight: bold; }");
    buttonRefuse->show();

    QLabel *qu25 = new QLabel(this);
    qu25->setText("Ouvrir la 25e boîte ?");
    //qu25->setGeometry(200, 200, 200, 200);
    qu25->setAlignment(Qt::AlignCenter);
    qu25->setStyleSheet(
            "QLabel { background-color: transparent; color: #ffffff; font-weight: bold; font-size: 32px; }");
    qu25->show();

    connect(buttonAccept, &QPushButton::clicked, [=] {
        zero->hide();
        divide->hide();
        more1000->hide();
        multiply->hide();
        qu25->hide();
        buttonAccept->hide();
        buttonRefuse->hide();
        endAPOAL(buttonAccept);
    });
    connect(buttonRefuse, &QPushButton::clicked, [=] {
        zero->hide();
        divide->hide();
        more1000->hide();
        multiply->hide();
        qu25->hide();
        buttonAccept->hide();
        buttonRefuse->hide();
        endAPOAL(buttonRefuse);
    });
}

/**
 * End the APOAL game
 * @param button : accept or refused button for the 25th box
 */
void GameWindow::endAPOAL(QPushButton *button) {
    srand(time(0));
    const int random = rand() % 4 + 1;

    if (button->text() == "OUVRIR") {
        std::string resp = finalPrice->text().toStdString();

        // Remove spaces and replace commas
        resp.erase(std::remove(resp.begin(), resp.end(), ' '), resp.end());
        if (resp.find(',') != std::string::npos) {
            resp.replace(resp.find(','), 1, ".");
        }

        if (!isdigit(resp[0])) {
            switch (random) {
                case 1 :
                    resp = "0 €";
                    break;
                case 2 :
                    resp = "1/2 " + resp;
                    break;
                case 3 :
                    resp = "1 000 €";
                    break;
                case 4 :
                    resp = "2 " + resp + "S";
                    break;
            }
        } else {
            double value = std::stod(resp);
            switch (random) {
                case 1 :
                    value *= 0.0;
                    break;
                case 2 :
                    value /= 2.0;
                    break;
                case 3 :
                    value += 1000.0;
                    break;
                case 4 :
                    value *= 2.0;
                    break;
            }

            if (value < 1) {
                std::ostringstream oss;
                oss << std::fixed << std::setprecision(2) << value;
                resp = oss.str() + " €";
                std::replace(resp.begin(), resp.end(), '.', ',');
            } else {
                std::string strVal = std::to_string((int) value);
                if (strVal.length() > 3) {
                    strVal.insert(strVal.length() - 3, " ");
                }
                resp = strVal + " €";
            }
        }
        finalPrice->setText(QString::fromStdString(resp));
    }

    const std::string all[] = {"0 €", "la moitié", "1 000 € en plus", "le double de vos gains"};
    QMessageBox msgBox;
    msgBox.setText(QString("La 25e boîte contenait %1 !").arg(QString::fromStdString(all[random - 1])));

    winLabel->show();
    finalPrice->show();
    msgBox.exec();

    // Add "Go back to menu" button
    QPushButton *backToMenu = new QPushButton("Retour au menu", this);
    backToMenu->setGeometry(900 / 2 - 100, 500 / 2 + 100, 200, 50);
    backToMenu->setStyleSheet("QPushButton { background-color: #0090D8; color: #ffffff; font-weight: bold; }");
    backToMenu->setCursor(Qt::PointingHandCursor);
    connect(backToMenu, &QPushButton::clicked, [=] { this->backToMenuClicked(); });
    backToMenu->show();
}

void GameWindow::endGame() {
    QString price;
    // Hide all buttons
    for (auto &boxe: boxes) {
        if (!boxe->isHidden()) {
            price = prices[boxe->text().toInt() - 1];
        }
        boxe->hide();
    }

    for (auto &label: labels) {
        if (label->text() == price) {
            finalPrice = label;
            break;
        }
    }

    // Zoom last price label in the middle of the screen
    finalPrice->show();
    finalPrice->setGeometry(900 / 2 - 200, 500 / 2 - 100, 400, 100);

    // Add style
    finalPrice->setStyleSheet(
            "QLabel { background-color : #0090D8; color : black; font-weight: bold; font-size: 50px; }");


    QPushButton *goTo25 = new QPushButton("25E BOÎTE", this);
    goTo25->setGeometry(50, 25, 100, 20);
    goTo25->setStyleSheet("QPushButton { background-color: gold; color: black; font-weight: bold; }");
    goTo25->show();

    // Create animation
    QPropertyAnimation *animation = new QPropertyAnimation(finalPrice, "pos");
    animation->setDuration(3000);
    animation->setStartValue(finalPrice->pos() + QPoint(-900 - 500, 0));
    animation->setEndValue(finalPrice->pos());
    animation->setEasingCurve(QEasingCurve::OutCirc);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    QPropertyAnimation *animationButton = new QPropertyAnimation(goTo25, "pos");
    animationButton->setDuration(3000);
    animationButton->setStartValue(goTo25->pos() + QPoint(-900 - 500, 0));
    animationButton->setEndValue(goTo25->pos());
    animationButton->setEasingCurve(QEasingCurve::OutCirc);
    animationButton->start(QAbstractAnimation::DeleteWhenStopped);

    // Show "Vous avez gagné" label
    winLabel->show();

    // Connect the button to the box25 function
    connect(goTo25, &QPushButton::clicked, [=] {
        winLabel->hide();
        finalPrice->hide();
        goTo25->hide();
        box25();
    });
}

void GameWindow::askForSelectBox() {
    // Show dialog box in the middle of the screen
    QMessageBox msgBox;
    msgBox.setText("Veuillez sélectionner une boite");
    msgBox.exec();

    waitingForBoxSelection = true;
}

void GameWindow::selectBox(QPushButton *box) {
    waitingForBoxSelection = false;

    // Set the style of buttons
    for (auto &b: boxes) {
        b->setStyleSheet(boxStyle);
        b->setEnabled(true);
        b->setCursor(Qt::PointingHandCursor);
    }

    box->setStyleSheet("QPushButton { background-color: #0090D8; color: #ffffff; font-weight: bold; }");
    box->setEnabled(false);
    box->setCursor(Qt::ForbiddenCursor);
}

void GameWindow::bankCalling() {
    // Show dialog box in the middle of the screen
    QMessageBox msgBox;
    msgBox.setText(
            "Dring dring dring !!!\nLa banque vous appelle. Le banquier vous propose de changer de boite. Voulez-vous changer de boite ?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.exec();

    if (msgBox.clickedButton() == msgBox.button(QMessageBox::Yes)) {
        // Ask for select a box
        askForSelectBox();
    }
}

void GameWindow::backToMenuClicked() {
    // Open the menu window
    MainMenuWindow *menuWindow = new MainMenuWindow();
    menuWindow->show();

    // Close the game window
    // this->close();
}
