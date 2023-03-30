#include "GameWindow.hpp"
#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QPicture>
#include <QPropertyAnimation>
#include <QStyle>
#include <QDesktopWidget>

QString boxStyle = "QPushButton { background-color: #ffffff; color: #000000; font-weight: bold; }";

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
    const int PRICE_WIDTH = 110;
    const int PRICE_HEIGHT = 30;
    const int PRICE_MARGIN = 3;
    this->boxesRemaining = BOXES;
    for (int i = 0; i < BOXES; ++i) {
        QLabel *label = new QLabel(this);
        label->setText(EXISTING_PRICES[i]);
        label->setAlignment(Qt::AlignCenter);
        if(i < BOXES / 2) {
            label->setStyleSheet("QLabel { background-color : red; color : white; font-weight: bold; }");
            label->setGeometry(PRICE_MARGIN, PRICE_MARGIN + (i * (PRICE_HEIGHT + PRICE_MARGIN)), PRICE_WIDTH, PRICE_HEIGHT);
        } else {
            label->setStyleSheet("QLabel { background-color : gold; color : black; font-weight: bold; }");
            label->setGeometry(PRICE_WIDTH + PRICE_MARGIN * 2, PRICE_MARGIN + ((i - BOXES / 2) * (PRICE_HEIGHT + PRICE_MARGIN)), PRICE_WIDTH, PRICE_HEIGHT);
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
    winLabel->setStyleSheet("QLabel { background-color: transparent; color: #ffffff; font-weight: bold; font-size: 32px; }");
    winLabel->setGeometry(0, 75, 900, 100);
    winLabel->hide();

    show();

    // Ask for select a box
    askForSelectBox();
}

void GameWindow::openBox(QPushButton *button) {
    // Check if not in selection mode
    if(waitingForBoxSelection) {
        selectBox(button);
        return;
    }

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
    if(this->boxesRemaining <= 1) {
        endGame();
        return;
    }

    // Show dialog box in the middle of the screen
    QMessageBox msgBox;
    msgBox.setText("Vous avez ouvert une boite. Elle contient " + price);
    msgBox.exec();


    // Bank
    if(boxesRemaining == 18 || boxesRemaining == 13 || boxesRemaining == 9 || boxesRemaining == 6 || boxesRemaining == 4 || boxesRemaining == 3 || boxesRemaining == 2) {
        bankCalling();
    }

}

void GameWindow::endGame() {
    QString price;
    // Hide all buttons
    for (auto & boxe : boxes) {
        if(!boxe->isHidden()) {
            price = prices[boxe->text().toInt() - 1];
        }
        boxe->hide();
    }

    QLabel *lastPrice;
    for (auto & label : labels) {
        if(label->text() == price) {
            lastPrice = label;
            break;
        }
    }

    // Zoom last price label in the middle of the screen
     lastPrice->show();
     lastPrice->setGeometry(900 / 2 - 200, 500 / 2 - 100, 400, 100);

    // Add style
     lastPrice->setStyleSheet("QLabel { background-color : #0090D8; color : black; font-weight: bold; font-size: 50px; }");

    // Create animation
    QPropertyAnimation *animation = new QPropertyAnimation(lastPrice, "pos");
    animation->setDuration(3000);
    animation->setStartValue(lastPrice->pos() + QPoint(-900 - 500, 0));
    animation->setEndValue(lastPrice->pos());
    animation->setEasingCurve(QEasingCurve::OutCirc);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    // Show "Vous avez gagné" label
    winLabel->show();
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
    for (auto & b : boxes) {
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
    msgBox.setText("Dring dring dring !!!\nLa banque vous appelle. Le banquier vous propose de changer de boite. Voulez-vous changer de boite ?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.exec();

    if(msgBox.clickedButton() == msgBox.button(QMessageBox::Yes)) {
        // Ask for select a box
        askForSelectBox();
    }
}