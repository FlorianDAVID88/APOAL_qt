#include <QPlainTextEdit>
#include <QCloseEvent>
#include <QSessionManager>
#include <QPushButton>
#include <QLabel>

class GameWindow : public QWidget {
Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0);

private:
    void openBox(QPushButton* button);
    void endGame();
    void askForSelectBox();
    void selectBox(QPushButton *box);
    void bankCalling();
    QLabel *winLabel;
    std::vector<QPushButton *> boxes;
    std::vector<QString> prices;
    std::vector<QLabel *> labels;
    QPoint arthurPos;
    int boxesRemaining;
    QLabel *arthur;
    int selectedBox;
    bool waitingForBoxSelection;
    void backToMenuClicked();
};